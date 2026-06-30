# PostgreSQL CLI (psql) Navigation

______________________________________________________________________

**Date:** 2026-04-30
**Tags:** [Postgres](../tags/Postgres.md), [SQL](../tags/SQL.md), [Bash](../tags/Bash.md), [Docker](../tags/Docker.md)
**URL:** https://www.postgresql.org/docs/current/app-psql.html

______________________________________________________________________

## Concept (what it is)
`psql` is the official PostgreSQL command‑line client. It lets you connect to a database, run SQL, and use **meta-commands** (commands that start with `\`) to inspect databases, schemas, tables, columns, indexes, and more. Think of it as your “database shell.”

Two types of commands inside `psql`:
- **SQL**: `SELECT`, `INSERT`, `UPDATE`, `DELETE`, etc.
- **Meta-commands**: `\l`, `\dt`, `\d`, `\c`, `\x`, etc. (only inside `psql`)

## How to use it (with examples)

### Example 1: Connect and explore (databases, schemas, tables)
Connect locally or from Docker, then list databases and tables.

```bash
# Local connection
psql -U postgres

# Docker: connect inside a running container named "postgres"
docker exec -it postgres psql -U postgres
```

Once inside `psql`:

```sql
\l               -- list databases
\c mydb          -- connect to database "mydb"
\dn              -- list schemas
\dt              -- list tables in search_path
\dt public.*     -- list tables in the public schema
```

Why it matters: before writing queries you usually need to confirm **which DB and schema** you’re in.

### Example 2: Inspect a table (columns, types, indexes)
Use `\d` to describe tables and see structure quickly.

```sql
\d users         -- columns + types
\d+ users        -- extended info (storage, size, etc.)
\di             -- list indexes
\z users         -- table privileges
```

Example query based on the structure you just inspected:

```sql
SELECT id, email, created_at
FROM users
ORDER BY created_at DESC
LIMIT 5;
```

Why it matters: `\d` helps you avoid guessing column names and types.

### Example 3: Make output readable and debug queries
These are `psql` meta-commands for developer ergonomics.

```sql
\x on            -- expanded output (great for wide rows)
\timing on       -- show query execution time
\pset pager off  -- disable paging (handy for scripts/logs)
```

Then run a query:

```sql
SELECT *
FROM orders
WHERE status = 'pending'
ORDER BY created_at DESC
LIMIT 10;
```

Why it matters: readable output and timing make investigation faster.

## Practical cheatsheet (psql meta-commands)
- **Databases**: `\l`
- **Connect**: `\c dbname`
- **Schemas**: `\dn`
- **Tables**: `\dt` or `\dt schema.*`
- **Describe**: `\d table`, `\d+ table`
- **Indexes**: `\di`
- **Functions**: `\df`
- **Users/Roles**: `\du`
- **Quit**: `\q`

## Notes about SQL vs meta-commands
- `SELECT * FROM users;` is SQL and runs in any SQL client.
- `\dt` is a `psql` meta-command and only works inside `psql`.

## Extra reference
- `psql` reference: https://www.postgresql.org/docs/current/app-psql.html
