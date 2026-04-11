# FastAPI + MongoDB (Motor)

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [FastAPI](../tags/FastAPI.md), [Python](../tags/Python.md), [Mongo](../tags/Mongo.md), [Rest](../tags/Rest.md)
**URL:** https://fastapi.tiangolo.com/ | https://www.uvicorn.org/ | https://docs.pydantic.dev/latest/concepts/pydantic_settings/ | https://fastapi.tiangolo.com/advanced/events/ | https://motor.readthedocs.io/en/stable/ | https://www.mongodb.com/docs/manual/reference/connection-string/

______________________________________________________________________

## Concept (what is it?)

To connect MongoDB to a FastAPI app you typically want:

1. **one Mongo client per process** (created at startup)
1. **reuse the client** across requests (connection pooling)
1. **close it** on shutdown

If your endpoints are `async def` (common in FastAPI), prefer **Motor** (`AsyncIOMotorClient`) so DB I/O doesn’t block the event loop.

The setup below is a small, repeatable baseline (similar to the Postgres note): venv + `requirements.txt`, `.env`, `uvicorn`, and a single Mongo client reused across requests.

______________________________________________________________________

## 1) Create the project + venv + install dependencies

```bash
mkdir my-fastapi-app
cd my-fastapi-app

python -m venv .venv
source .venv/bin/activate

python -m pip install -U pip
python -m pip install fastapi uvicorn[standard] pydantic-settings
python -m pip install motor

python -m pip freeze > requirements.txt
```

Later, to recreate the environment:

```bash
python -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt
```

______________________________________________________________________

## 2) Minimal project structure (simple, common)

```text
my-fastapi-app/
  app/
    main.py
    core/config.py
    db/mongo.py
    routers/health.py
  .env
  requirements.txt
```

Example `.env`:

```bash
APP_NAME="my-fastapi-app"
MONGODB_URI="mongodb://localhost:27017"
MONGODB_DB="appdb"
```

Run the app:

```bash
uvicorn app.main:app --reload
```

______________________________________________________________________

## Example 1) Settings + app entrypoint

`app/core/config.py`

```py
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    model_config = SettingsConfigDict(env_file=".env", extra="ignore")

    app_name: str = "my-fastapi-app"
    mongodb_uri: str
    mongodb_db: str = "appdb"


settings = Settings()
```

`app/routers/health.py`

```py
from fastapi import APIRouter

router = APIRouter(tags=["health"])


@router.get("/health")
async def health():
    return {"ok": True}
```

`app/main.py`

```py
from fastapi import FastAPI

from app.core.config import settings
from app.routers.health import router as health_router

app = FastAPI(title=settings.app_name)
app.include_router(health_router)
```

______________________________________________________________________

## Example 2) Lifespan startup/shutdown + `app.state` (Mongo client once)

Pattern:

- Create the client once in FastAPI lifespan
- Store DB handle in `app.state`
- Read it in your routes

`app/db/mongo.py`

```py
from contextlib import asynccontextmanager

from fastapi import FastAPI, Request
from motor.motor_asyncio import AsyncIOMotorClient, AsyncIOMotorDatabase

from app.core.config import settings


@asynccontextmanager
async def mongo_lifespan(app: FastAPI):
    client = AsyncIOMotorClient(settings.mongodb_uri)
    app.state.mongo_db = client[settings.mongodb_db]
    yield
    client.close()


def get_mongo_db(request: Request) -> AsyncIOMotorDatabase:
    return request.app.state.mongo_db
```

Wire lifespan into the app:

```py
from fastapi import FastAPI

from app.core.config import settings
from app.db.mongo import mongo_lifespan
from app.routers.health import router as health_router

app = FastAPI(title=settings.app_name, lifespan=mongo_lifespan)
app.include_router(health_router)
```

Optional: health that also pings Mongo:

```py
from fastapi import APIRouter, Depends
from motor.motor_asyncio import AsyncIOMotorDatabase

from app.db.mongo import get_mongo_db

router = APIRouter(tags=["health"])


@router.get("/health/db")
async def health_db(db: AsyncIOMotorDatabase = Depends(get_mongo_db)):
    await db.command("ping")
    return {"ok": True}
```

Notes:

- `AsyncIOMotorClient` is safe to reuse and manages a pool internally.
- `client.close()` should happen on shutdown (important in dev reload too).

______________________________________________________________________

## Example 3) Dependency injection with `Depends`

Use a dependency so your routes don’t touch `request.app.state` directly:

```py
from fastapi import Depends
from motor.motor_asyncio import AsyncIOMotorDatabase

from app.db.mongo import get_mongo_db


@app.get("/users/{email}")
async def get_user(email: str, db: AsyncIOMotorDatabase = Depends(get_mongo_db)):
    doc = await db.users.find_one({"email": email})
    if not doc:
        return {"error": "not found"}
    doc["_id"] = str(doc["_id"])
    return doc
```

______________________________________________________________________

## Example 4) ObjectId in routes (very common pitfall)

Mongo uses `_id: ObjectId(...)` by default. If you receive an id as string:

```py
from bson import ObjectId
from fastapi import Depends, HTTPException

from app.db.mongo import get_mongo_db


@app.get("/items/{item_id}")
async def get_item(item_id: str, db=Depends(get_mongo_db)):
    try:
        oid = ObjectId(item_id)
    except Exception:
        raise HTTPException(status_code=400, detail="invalid id")

    doc = await db.items.find_one({"_id": oid})
    if not doc:
        raise HTTPException(status_code=404, detail="not found")

    doc["_id"] = str(doc["_id"])
    return doc
```

______________________________________________________________________

## Motor vs PyMongo (`MongoClient`) in FastAPI

- **Motor** is async → best match for `async def` endpoints.
- **PyMongo** is sync → fine for sync code, but if called directly inside `async def` it can block the event loop (hurts concurrency).

Rule of thumb: **FastAPI async endpoints → Motor**.

______________________________________________________________________

## Quick local DB (Docker Compose)

```yaml
services:
  mongo:
    image: mongo:7
    ports:
      - "27017:27017"
```
