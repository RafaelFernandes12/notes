# Python venv and requirements.txt

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [Python](../tags/Python.md), [Pip](../tags/Pip.md), [Venv](../tags/Venv.md), [Requirements](../tags/Requirements.md), [Bash](../tags/Bash.md)
**URL:** https://docs.python.org/3/library/venv.html | https://pip.pypa.io/en/stable/user_guide/ | https://pip.pypa.io/en/stable/reference/requirements-file-format/

______________________________________________________________________

## Concept (what is `venv`? what is `requirements.txt`?)

### `venv`

`venv` is Python’s built-in way to create an **isolated environment** per project.

In practice, a venv gives you:

- a project-local `python`
- a project-local `pip`
- project-local installed dependencies (so they don’t conflict with other projects or system packages)

### `requirements.txt`

`requirements.txt` is just a **list of Python packages** (optionally pinned versions) that you can install with pip.

It answers: “what does this project need installed?”

Important:

- The conventional name is `requirements.txt` (plural), but **pip can read any filename** with `-r <file>`.

______________________________________________________________________

## Why use them together

- `venv` isolates *where packages are installed*
- `requirements.txt` documents *what packages to install*

Together they make “clone repo → install deps → run app” predictable.

______________________________________________________________________

## How to use (common commands)

### 1) Create a venv

```bash
python3 -m venv .venv
```

### 2) Activate it

```bash
source .venv/bin/activate
```

### 3) Install from requirements

```bash
python -m pip install -U pip
python -m pip install -r requirements.txt
```

Why `python -m pip`?

- It guarantees you’re installing into the same interpreter you’ll run (inside the venv).

### 4) Freeze dependencies

```bash
python -m pip freeze > requirements.txt
```

This produces a fully pinned list (including transitive deps). Good for reproducibility, but can be noisy.

______________________________________________________________________

## Examples

### Example 1) FastAPI project setup

```bash
python3 -m venv .venv
source .venv/bin/activate

python -m pip install -U pip
python -m pip install -r requirements.txt

python -m uvicorn main:app --reload
```

### Example 2) “It installed, but import fails” (pip/python mismatch)

Symptom:

```bash
pip install fastapi
python -c "import fastapi"  # fails
```

Fix: use the right pip for the interpreter:

```bash
python -m pip install fastapi
python -c "import fastapi"  # ok
```

In a venv, always prefer:

```bash
python -m pip ...
```

### Example 3) Separate dev dependencies

`requirements.txt` (runtime deps):

```text
fastapi
uvicorn
motor
```

`requirements-dev.txt` (dev/test tooling):

```text
-r requirements.txt
pytest
ruff
```

Install dev deps:

```bash
python -m pip install -r requirements-dev.txt
```

______________________________________________________________________

## Practical tips

- Don’t commit `.venv/` (add it to `.gitignore`).
- Use `python -m pip --version` to confirm which Python pip is using.
- When moving machines/containers: recreate the venv and `pip install -r ...`.

Related notes:

- [Python -m (Running Modules) and pip](./Python%20-m%20%28Running%20Modules%29%20and%20pip.md)
- [System Packages vs Python Packages (DNF vs pip)](./System%20Packages%20vs%20Python%20Packages%20%28DNF%20vs%20pip%29.md)
