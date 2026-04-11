# Python __init__.py (Packages and Imports)

______________________________________________________________________

**Date:** 2026-03-10
**Tags:** [Python](../tags/Python.md), [Imports](../tags/Imports.md), [Packages](../tags/Packages.md)
**URL:** https://docs.python.org/3/tutorial/modules.html#packages | https://docs.python.org/3/reference/import.html | https://peps.python.org/pep-0420/

______________________________________________________________________

## Concept (what is `__init__.py`?)

`__init__.py` is a file that marks a directory as a **Python package** (in the “regular package” sense) and is executed the first time that package is imported.

Historically, **a directory needed `__init__.py` to be importable as a package**. Modern Python also supports **namespace packages** (PEP 420) that can exist *without* `__init__.py`, but many projects still keep `__init__.py` for clarity, tooling compatibility, and to define a package API.

## Why it exists / why we use it

### 1) Make a directory a (regular) package

This is the most common reason: you want `import mypkg.something` to work consistently.

Example layout:

```
project/
  mypkg/
    __init__.py
    service.py
```

Then:

```py
from mypkg.service import do_work
```

### 2) Define the public API of a package (re-exports)

You can expose a “nice” import path by re-exporting symbols.

```
project/
  mypkg/
    __init__.py
    service.py
    repository.py
```

`mypkg/__init__.py`:

```py
from .service import MessageService
from .repository import MessageRepository

__all__ = ["MessageService", "MessageRepository"]
```

Consumer code:

```py
from mypkg import MessageService
```

This keeps your internal file structure flexible without breaking imports.

### 3) Package-level initialization (use carefully)

Because `__init__.py` executes on import, it can run code:

```py
# mypkg/__init__.py
import logging

logger = logging.getLogger(__name__)
logger.debug("mypkg imported")
```

Common cautions:

- Avoid heavy side effects (network calls, DB connections) during import.
- Prefer doing setup in explicit functions or app startup hooks.

### 4) Better behavior with tooling and type checkers

Even though namespace packages exist, some setups (editor/LSP configs, linters, tests) become simpler when packages are explicit.

In practice, having `__init__.py` in directories like `modules/` and `modules/messages/` makes imports and static analysis more predictable.

## How imports behave with and without `__init__.py`

### Regular package (with `__init__.py`)

- Importing `import mypkg` runs `mypkg/__init__.py`.
- Relative imports inside the package (`from .service import ...`) work.

### Namespace package (without `__init__.py`)

- The directory can still be importable *if* Python finds it on `sys.path` and it’s treated as a namespace package.
- Useful when a package is split across multiple directories.
- Harder to reason about in small apps; can confuse tooling.

## Practical examples

### Example A) “Module folder” in an API app

If you create a structure like:

```
modules/
  messages/
    controller.py
    service.py
```

Adding:

- `modules/__init__.py`
- `modules/messages/__init__.py`

turns them into explicit packages, so imports like:

```py
from modules.messages.controller import router
```

are more reliable across environments and tools.

### Example B) Avoid circular imports by keeping `__init__.py` light

If `__init__.py` re-exports many things, you can accidentally create circular imports.

Rule of thumb:

- Keep `__init__.py` minimal.
- Re-export only what you truly want as “public API”.

### Example C) Making a “nice import path” for DTOs

Instead of:

```py
from mypkg.dto import MessageIn, MessageOut
```

you can re-export:

```py
# mypkg/__init__.py
from .dto import MessageIn, MessageOut
__all__ = ["MessageIn", "MessageOut"]
```

Then:

```py
from mypkg import MessageIn
```
