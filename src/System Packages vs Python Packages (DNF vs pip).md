# System Packages vs Python Packages (DNF vs pip)

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [Linux](../tags/Linux.md), [Bash](../tags/Bash.md), [Python](../tags/Python.md), [Pip](../tags/Pip.md), [DNF](../tags/DNF.md)
**URL:** https://dnf.readthedocs.io/en/latest/ | https://pip.pypa.io/en/stable/user_guide/ | https://docs.python.org/3/library/venv.html

______________________________________________________________________

## Concept (what is it?)

There are *two different ecosystems* involved when you install things on Fedora:

- **DNF/RPM (system packages)**
  - Installs **OS-managed** packages from Fedora repositories.
  - Tracks *system-wide* dependencies (libraries, Python modules, binaries, configs).
  - Files usually land under `/usr/...`.
  - Uninstall/upgrade is done with **`dnf`**.

- **pip (Python packages)**
  - Installs **Python ecosystem** packages (usually from PyPI) into a *Python environment*.
  - Depending on how you run it, it can install to:
    - your **user site** (`~/.local/...`), or
    - a **virtualenv** (`.venv/...`), or
    - (not recommended) the **system Python**.
  - Uninstall/upgrade is done with **`pip`** (in the same environment).

Mixing them can work, but it’s easy to end up running a **DNF-provided executable** that can’t see **pip-installed Python modules**.

______________________________________________________________________

## Where things get installed (Fedora mental model)

### DNF-installed Python tooling

Typical locations:

- Executables: `/usr/bin/<tool>`
- Python libs: `/usr/lib/python3.X/site-packages/...`

### pip-installed packages (user site)

Typical locations:

- Executables: `~/.local/bin/<tool>`
- Python libs: `~/.local/lib/python3.X/site-packages/...`

Useful commands:

```bash
python -m site --user-base
python -m site --user-site
python -m pip show fastapi
python -m pip show uvicorn
```

### pip-installed packages (virtual environment)

Typical locations:

- Executables: `.venv/bin/<tool>`
- Python libs: `.venv/lib/python3.X/site-packages/...`

Create and use a venv:

```bash
python3 -m venv .venv
source .venv/bin/activate

python -m pip install -U pip
python -m pip install fastapi uvicorn[standard]

uvicorn main:app --reload
```

______________________________________________________________________

## Example (real-world gotcha): DNF `uvicorn` + pip `fastapi`

Scenario:

1) You run `pip install fastapi` (often ends up in `~/.local/...`).
2) `uvicorn` command is missing because `~/.local/bin` is not on your `PATH`.
3) Fedora suggests `dnf install python3-uvicorn`.
4) Now `uvicorn` exists, but crashes with:

```text
ModuleNotFoundError: No module named 'fastapi'
```

Why this happens:

- The system `uvicorn` is `/usr/bin/uvicorn`.
- On Fedora it may run Python with flags like `-s` (ignore user-site packages).
- Your `fastapi` is in user-site (`~/.local/lib/...`) so it becomes invisible.

How to confirm what you’re actually running:

```bash
which -a uvicorn
python -c "import fastapi; print(fastapi.__file__)"
python -s -c "import fastapi"   # often fails if fastapi is only in user-site
```

Fix options:

### Fix A) Run uvicorn via the Python environment that has the packages

```bash
python -m uvicorn main:app --reload
```

### Fix B) Use the pip-installed `uvicorn` executable

```bash
~/.local/bin/uvicorn main:app --reload
```

If `~/.local/bin` is not on your PATH, add it (zsh example):

```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Fix C) Use a venv per project (recommended)

This avoids DNF/pip mixing issues and makes dependencies reproducible:

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install fastapi uvicorn[standard]
uvicorn main:app --reload
```

______________________________________________________________________

## How to list and remove DNF packages

### List installed packages

```bash
dnf list installed
dnf list installed python3-uvicorn

rpm -qa | sort
rpm -q python3-uvicorn
```

### Find which package installed a file

```bash
which uvicorn
rpm -qf "$(which uvicorn)"
```

### Search what package *would* provide something

```bash
dnf provides '*/uvicorn'
dnf provides /usr/bin/uvicorn
```

### Remove packages

```bash
sudo dnf remove python3-uvicorn

# Optional: remove dependencies that became unused
sudo dnf autoremove
```

### (Optional) Inspect DNF history

```bash
dnf history
sudo dnf history info <ID>
```

______________________________________________________________________

## How to list and remove pip packages (same idea, different tool)

Use `python -m pip` so you’re always talking to the right interpreter:

```bash
python -m pip list
python -m pip show fastapi

python -m pip uninstall fastapi uvicorn
```

______________________________________________________________________

## Rules of thumb

1) **Use DNF** for system-level tooling you want OS-managed.
2) **Use a venv** for application dependencies (FastAPI, Uvicorn, etc.).
3) Avoid relying on `pip --user` for project deps unless you understand PATH and user-site behavior.
4) If something is “installed” but the command is missing:
   - check `~/.local/bin` (pip user scripts), and
   - check `PATH` (`echo $PATH`).
