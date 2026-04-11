# FastAPI + MongoDB (PyMongo)

______________________________________________________________________

**Date:** 2026-03-11
**Tags:** [Python](../tags/Python.md), [FastAPI](../tags/FastAPI.md), [Mongo](../tags/Mongo.md), [Rest](../tags/Rest.md)
**URL:** https://fastapi.tiangolo.com/ | https://www.uvicorn.org/ | https://pymongo.readthedocs.io/en/stable/ | https://www.mongodb.com/docs/manual/reference/connection-string/ | https://www.mongodb.com/docs/manual/core/connection-pooling/

______________________________________________________________________

## Concept (what is it?)

**PyMongo** is the official **sync** MongoDB driver for Python.

In FastAPI, the main decision is:

- If your routes are mostly `async def`, prefer **Motor** (async driver) to avoid blocking the event loop.
- If you want to use **PyMongo**, either:
  - write **sync routes** (`def`) so FastAPI runs them in a threadpool, or
  - call PyMongo from `async def` using a threadpool explicitly.

Either way, you typically want **one `MongoClient` per process** (created on startup), reused across requests (pooling), and closed on shutdown.

______________________________________________________________________

## 1) Create the project + venv + install dependencies

```bash
mkdir my-fastapi-app
cd my-fastapi-app

python -m venv .venv
source .venv/bin/activate

python -m pip install -U pip
python -m pip install fastapi uvicorn[standard] pydantic-settings
python -m pip install pymongo

python -m pip freeze > requirements.txt
```

Example `.env`:

```bash
APP_NAME="my-fastapi-app"
MONGODB_URI="mongodb://localhost:27017"
MONGODB_DB="appdb"
```

Run:

```bash
uvicorn app.main:app --reload
```

______________________________________________________________________

## 2) Minimal project structure

```text
my-fastapi-app/
  app/
    main.py
    core/config.py
    db/mongo.py
    routers/users.py
  .env
  requirements.txt
```

______________________________________________________________________

## Example 1) Lifespan + app.state (sync routes recommended)

This is the simplest way to use PyMongo: keep your endpoints `def`.

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

`app/db/mongo.py`

```py
from contextlib import asynccontextmanager

from fastapi import FastAPI, Request
from pymongo import MongoClient
from pymongo.database import Database

from app.core.config import settings


@asynccontextmanager
async def mongo_lifespan(app: FastAPI):
    client = MongoClient(settings.mongodb_uri)
    app.state.mongo_db = client[settings.mongodb_db]
    yield
    client.close()


def get_mongo_db(request: Request) -> Database:
    return request.app.state.mongo_db
```

`app/main.py`

```py
from fastapi import FastAPI

from app.core.config import settings
from app.db.mongo import mongo_lifespan
from app.routers.users import router as users_router

app = FastAPI(title=settings.app_name, lifespan=mongo_lifespan)
app.include_router(users_router)
```

`app/routers/users.py`

```py
from fastapi import APIRouter, Depends
from pymongo.database import Database

from app.db.mongo import get_mongo_db

router = APIRouter(prefix="/users", tags=["users"])


@router.post("/")
def create_user(email: str, db: Database = Depends(get_mongo_db)):
    db.users.insert_one({"email": email})
    return {"created": True}


@router.get("/")
def list_users(db: Database = Depends(get_mongo_db)):
    # PyMongo returns cursors; convert to plain JSON-safe objects.
    docs = list(db.users.find({}, {"email": 1}))
    return [{"id": str(d["_id"]), "email": d.get("email")} for d in docs]
```

Why `def` routes here? Because PyMongo is sync; `def` endpoints won’t block the event loop.

______________________________________________________________________

## Example 2) Calling PyMongo from `async def` (threadpool)

If you need `async def` routes (other async I/O), run blocking PyMongo calls in a threadpool:

```py
from fastapi import APIRouter, Depends
from fastapi.concurrency import run_in_threadpool
from pymongo.database import Database

from app.db.mongo import get_mongo_db

router = APIRouter(prefix="/stats", tags=["stats"])


@router.get("/users-count")
async def users_count(db: Database = Depends(get_mongo_db)):
    count = await run_in_threadpool(db.users.count_documents, {})
    return {"count": count}
```

Rule of thumb: if it’s PyMongo and you are in `async def`, assume it’s blocking and push it to a thread.

______________________________________________________________________

## Example 3) ObjectId (read by id)

Mongo `_id` is usually an `ObjectId`. Convert string → `ObjectId` and validate:

```py
from bson import ObjectId
from fastapi import APIRouter, Depends, HTTPException
from pymongo.database import Database

from app.db.mongo import get_mongo_db

router = APIRouter(prefix="/items", tags=["items"])


@router.get("/{item_id}")
def get_item(item_id: str, db: Database = Depends(get_mongo_db)):
    try:
        oid = ObjectId(item_id)
    except Exception:
        raise HTTPException(status_code=400, detail="invalid id")

    doc = db.items.find_one({"_id": oid})
    if not doc:
        raise HTTPException(status_code=404, detail="not found")

    return {"id": str(doc["_id"]), **{k: v for k, v in doc.items() if k != "_id"}}
```

______________________________________________________________________

## Quick local DB (Docker Compose)

```yaml
services:
  mongo:
    image: mongo:7
    ports:
      - "27017:27017"
```

______________________________________________________________________

## PyMongo vs Motor in FastAPI

- **PyMongo (sync)**: simplest if you’re fine with `def` routes, or if you wrap calls in a threadpool.
- **Motor (async)**: best fit for fully-async FastAPI apps.
