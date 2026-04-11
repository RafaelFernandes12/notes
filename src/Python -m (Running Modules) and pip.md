# Python -m (Running Modules) and pip

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [Python](../tags/Python.md), [Pip](../tags/Pip.md), [Bash](../tags/Bash.md), [Linux](../tags/Linux.md)
**URL:** https://docs.python.org/3/using/cmdline.html#cmdoption-m | https://pip.pypa.io/en/stable/user_guide/

______________________________________________________________________

## Concept (what is `python -m`?)

`python -m <module>` runs a **Python module as a script** using *that exact Python interpreter*.

Examples:

```bash
python -m pip --version
python -m http.server 8000
python -m uvicorn main:app --reload
```

So `python -m pip ...` means: “run pip that belongs to **this** `python`”.

______________________________________________________________________

## Difference: `python -m pip install motor` vs `pip install motor`

### `python -m pip install motor`

- Guarantees you install into the environment for that interpreter:
  - the active venv’s `python`, or
  - your system `python`, or
  - any specific Python you call (`python3.12 -m pip ...`).

This avoids the classic mismatch:

> you install with one pip, but run with a different python.

### `pip install motor`

- Runs the `pip` executable found first in your `PATH`.
- That `pip` might point to a different Python than the one you run your app with.

Example of how it goes wrong:

```bash
pip install fastapi
python -c "import fastapi"   # can fail if pip/python are different
```

______________________________________________________________________

## How to confirm if `pip` matches your `python`

### Check which executables you’re using

```bash
which -a python
which -a pip
```

### Check versions and paths

```bash
python -V
python -m pip --version
pip --version
```

`pip --version` prints something like:

```text
pip X.Y from /some/path/site-packages/pip (python 3.Z)
```

If the `(python 3.Z)` or paths differ from your intended interpreter, you have a mismatch.

______________________________________________________________________

## Practical examples (common scenarios)

### Example A) Inside a venv (recommended for projects)

```bash
python3 -m venv .venv
source .venv/bin/activate

python -m pip install motor
python -m pip list
```

Why prefer this: it’s explicit, reproducible, and avoids system/user-site conflicts.

### Example B) Multiple Pythons installed

```bash
python3.13 -m pip install motor
python3.12 -m pip install motor
```

Each command installs into a different interpreter’s environment.

### Example C) User-site installs (no venv)

```bash
python -m pip install --user motor
```

This puts packages under `~/.local/...` and scripts under `~/.local/bin`.

If commands “don’t exist”, check your PATH:

```bash
echo $PATH
ls ~/.local/bin
```

______________________________________________________________________

## Rule of thumb

- For anything important/project-related: **always use** `python -m pip ...`.
- If you must use `pip ...`, at least verify `pip --version` matches the Python you’ll run.
