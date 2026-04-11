# FastAPI CRUD with MongoDB (Motor)

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [FastAPI](../tags/FastAPI.md), [Mongo](../tags/Mongo.md), [Python](../tags/Python.md), [Rest](../tags/Rest.md)
**URL:** https://fastapi.tiangolo.com/ | https://motor.readthedocs.io/en/stable/ | https://www.mongodb.com/docs/manual/crud/

______________________________________________________________________

## Concept (what is it?)

CRUD in FastAPI + MongoDB (Motor) is just calling MongoDB operations from HTTP endpoints:

- **Create**: `insert_one`
- **Read**: `find_one`, `find(...).to_list(...)`
- **Update**: `update_one`
- **Delete**: `delete_one`

The key integration detail is **how you get a DB handle** inside routes. A common pattern is:

1) connect once during app startup
2) store the DB object on `app.state`
3) read it from `Request` (or a `Depends` dependency)

Also: MongoDB documents include `_id: ObjectId(...)` by default, and **ObjectId is not JSON serializable**, so you must convert it to `str` before returning it.

______________________________________________________________________

## How we accessed the DB (startup + helper)

In our project, DB access is done like this:

1) the app sets `app.state.mongo` during lifespan startup
2) a helper reads `request.app.state.mongo.db`
3) each endpoint calls `db.get_collection("messages")`

Example helper (the idea):

```py
from fastapi import Request


def get_db(request: Request):
    db = request.app.state.mongo.db
    if db is None:
        raise RuntimeError("MongoDB is not connected")
    return db
```

Practical route snippet:

```py
from fastapi import Request


def messages_collection(request: Request):
    db = get_db(request)
    return db.get_collection("messages")
```

______________________________________________________________________

## Create (POST)

### What we do

1) build a document (`dict`)
2) `insert_one(doc)`
3) return a JSON-safe response

```py
from datetime import datetime, timezone
from pydantic import BaseModel
from fastapi import Request


class MessageIn(BaseModel):
    text: str


@app.post("/messages/")
async def create_message(request: Request, payload: MessageIn):
    messages = messages_collection(request)
    created_at = datetime.now(timezone.utc)

    doc = {"text": payload.text, "created_at": created_at}
    result = await messages.insert_one(doc)

    # Don't return `doc` (it now contains _id: ObjectId)
    return {"id": str(result.inserted_id), "text": payload.text, "created_at": created_at}
```

Why not `return doc`?

- Mongo adds `_id: ObjectId(...)` to the dict. FastAPI can’t JSON-encode that, so you’ll get an encoder exception.

______________________________________________________________________

## Read (GET)

### Get all

Motor `find()` returns a cursor. Use `to_list` to fetch results.

```py
from fastapi import Query, Request


@app.get("/messages/")
async def get_all_messages(request: Request, limit: int = Query(100, ge=1, le=500)):
    messages = messages_collection(request)
    docs = await messages.find().sort("created_at", -1).to_list(length=limit)

    return [
        {"id": str(d["_id"]), "text": d.get("text", ""), "created_at": d["created_at"]}
        for d in docs
    ]
```

### Get by id

Mongo `_id` is an `ObjectId`, so you must convert the string route param.

```py
from bson import ObjectId
from bson.errors import InvalidId
from fastapi import HTTPException, Request


@app.get("/messages/{message_id}")
async def get_message_by_id(request: Request, message_id: str):
    messages = messages_collection(request)

    try:
        oid = ObjectId(message_id)
    except (InvalidId, TypeError):
        raise HTTPException(status_code=400, detail="Invalid message id")

    doc = await messages.find_one({"_id": oid})
    if doc is None:
        raise HTTPException(status_code=404, detail="Message not found")

    return {"id": str(doc["_id"]), "text": doc.get("text", ""), "created_at": doc["created_at"]}
```

### Get by `text` and `created_at`

Exact datetime matches are possible but fragile.

```py
from datetime import datetime
from fastapi import Request


@app.get("/messages/search/")
async def get_by_text_and_created_at(request: Request, text: str, created_at: datetime):
    messages = messages_collection(request)
    docs = await messages.find({"text": text, "created_at": created_at}).to_list(length=100)
    return [{"id": str(d["_id"]), "text": d.get("text", ""), "created_at": d["created_at"]} for d in docs]
```

More practical (range query):

```py
from datetime import timedelta

start = created_at
end = created_at + timedelta(seconds=1)
docs = await messages.find({"text": text, "created_at": {"$gte": start, "$lt": end}}).to_list(100)
```

______________________________________________________________________

## Update (PUT)

Mongo updates use update operators like `$set`.

Important: `update_one(...)` does not return the updated doc; it returns counts.

```py
@app.put("/messages/{message_id}")
async def update_message(request: Request, message_id: str, payload: MessageIn):
    messages = messages_collection(request)
    oid = ObjectId(message_id)  # wrap with try/except like above

    result = await messages.update_one({"_id": oid}, {"$set": {"text": payload.text}})
    if result.matched_count == 0:
        raise HTTPException(status_code=404, detail="Message not found")

    doc = await messages.find_one({"_id": oid})
    return {"id": str(doc["_id"]), "text": doc.get("text", ""), "created_at": doc["created_at"]}
```

______________________________________________________________________

## Delete (DELETE)

```py
@app.delete("/messages/{message_id}")
async def delete_message(request: Request, message_id: str):
    messages = messages_collection(request)
    oid = ObjectId(message_id)  # wrap with try/except like above

    result = await messages.delete_one({"_id": oid})
    if result.deleted_count == 0:
        raise HTTPException(status_code=404, detail="Message not found")

    return {"deleted": True, "id": message_id}
```

______________________________________________________________________

## Quick checklist

- [ ] create client once at startup (lifespan)
- [ ] reuse `db.get_collection("...")` across requests
- [ ] convert `ObjectId` to `str` in responses
- [ ] use `to_list(...)` for cursors
- [ ] validate route ids (`ObjectId(...)` can throw)
