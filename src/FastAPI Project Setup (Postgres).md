# FastAPI Project Setup (Postgres)

______________________________________________________________________

**Date:** 2026-03-11
**Tags:** [Python](../tags/Python.md), [FastAPI](../tags/FastAPI.md), [Postgres](../tags/Postgres.md)
**URL:** https://fastapi.tiangolo.com/ | https://www.uvicorn.org/ | https://docs.pydantic.dev/latest/concepts/pydantic_settings/ | https://docs.sqlalchemy.org/en/20/orm/extensions/asyncio.html | https://magicstack.github.io/asyncpg/current/ | https://alembic.sqlalchemy.org/ | https://www.postgresql.org/docs/

______________________________________________________________________

## Concept (what is it?)

“Setting up a FastAPI project” usually means having a **repeatable** baseline that covers:

- a Python environment (venv) + dependency management (`requirements.txt`)
- an app entrypoint you can run locally (`uvicorn`)
- configuration via environment variables (`.env`)
- a Postgres connection setup that is **safe per-request** (sessions) and **reuses pools** (engine)

Below is a practical setup for **Postgres** using **SQLAlchemy (async)** + **asyncpg**.

______________________________________________________________________

## 1) Create the project + venv + install dependencies

```bash
mkdir my-fastapi-app
cd my-fastapi-app

python -m venv .venv
source .venv/bin/activate

python -m pip install -U pip
python -m pip install fastapi uvicorn[standard] pydantic-settings
python -m pip install sqlalchemy[asyncio] asyncpg alembic

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

Example layout:

```text
my-fastapi-app/
  app/
    main.py
    core/config.py
    db/postgres.py
    routers/health.py
  .env
  requirements.txt
```

Example `.env`:

```bash
APP_NAME="my-fastapi-app"
POSTGRES_DSN="postgresql+asyncpg://postgres:postgres@localhost:5432/appdb"
```

______________________________________________________________________

## Example 1) FastAPI app + settings (env-driven)

`app/core/config.py`

```py
from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    model_config = SettingsConfigDict(env_file=".env", extra="ignore")

    app_name: str = "my-fastapi-app"
    postgres_dsn: str


settings = Settings()  # importable singleton (small projects)
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

Run:

```bash
uvicorn app.main:app --reload
```

______________________________________________________________________

## Example 2) Postgres (SQLAlchemy async) session dependency

______________________________________________________________________

`app/db/postgres.py`

```py
from typing import AsyncIterator

from sqlalchemy.ext.asyncio import AsyncEngine, AsyncSession, create_async_engine
from sqlalchemy.orm import sessionmaker

from app.core.config import settings


engine: AsyncEngine = create_async_engine(settings.postgres_dsn, pool_pre_ping=True)

AsyncSessionLocal = sessionmaker(
    bind=engine,
    class_=AsyncSession,
    expire_on_commit=False,
)


async def get_session() -> AsyncIterator[AsyncSession]:
    async with AsyncSessionLocal() as session:
        yield session
```

Minimal model + query example (for learning):

```py
from sqlalchemy import Integer, String, select
from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column
from sqlalchemy.ext.asyncio import AsyncSession
from fastapi import APIRouter, Depends

from app.db.postgres import get_session


class Base(DeclarativeBase):
    pass


class Account(Base):
    __tablename__ = "accounts"
    id: Mapped[int] = mapped_column(Integer, primary_key=True)
    email: Mapped[str] = mapped_column(String, unique=True, index=True)


router = APIRouter(prefix="/accounts", tags=["accounts"])


@router.get("/")
async def list_accounts(session: AsyncSession = Depends(get_session)):
    result = await session.execute(select(Account).order_by(Account.id))
    return [a.email for a in result.scalars().all()]
```

Notes:

- In real projects, use **Alembic** migrations for table creation/changes.
- Prefer one `engine` per process; sessions are per-request.

______________________________________________________________________

## Example 3) Postgres health check (DB ping)

If you want a health endpoint that also checks Postgres:

```py
from fastapi import APIRouter, Depends
from sqlalchemy import text
from sqlalchemy.ext.asyncio import AsyncSession

from app.db.postgres import get_session

router = APIRouter(tags=["health"])


@router.get("/health/db")
async def health_db(session: AsyncSession = Depends(get_session)):
    await session.execute(text("select 1"))
    return {"ok": True}
```

Tip: decide whether DB-down should fail readiness checks (k8s) vs just warn.

______________________________________________________________________

## Migrations with Alembic (practical commands)

Initialize once:

```bash
alembic init alembic
```

Create a migration after changing models:

```bash
alembic revision --autogenerate -m "create accounts"
```

Apply migrations:

```bash
alembic upgrade head
```

(You’ll need to wire your `Base.metadata` + async engine into Alembic’s `env.py`.)

______________________________________________________________________

## Quick local DBs (Docker Compose)

If you want a fast local setup:

```yaml
services:
  postgres:
    image: postgres:16
    environment:
      POSTGRES_PASSWORD: postgres
      POSTGRES_USER: postgres
      POSTGRES_DB: appdb
    ports:
      - "5432:5432"
```

______________________________________________________________________

## Common pitfalls

- Don’t create a new engine/sessionmaker per-request (kills pooling). Keep the engine module-level.
- Don’t use sync Postgres drivers (psycopg2) inside `async def` routes.
- Keep secrets out of git: use `.env` locally and real env vars in production.
