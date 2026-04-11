# Node.js Modules (CommonJS vs ESM)

______________________________________________________________________

**Date:** 2026-03-06
**Tags:** [NodeJS](../tags/NodeJS.md), [Js](../tags/Js.md), [TypeScript](../tags/TypeScript.md), [Npm](../tags/Npm.md)
**URL:** https://nodejs.org/api/packages.html

______________________________________________________________________

## What it is

In Node.js there are two main module systems:

- **CommonJS (CJS)**: `require(...)` / `module.exports` (traditional Node style)
- **ECMAScript Modules (ESM)**: `import ... from ...` / `export ...` (modern JS standard)

They resolve modules differently, have different interop rules, and directly affect how you **publish** and **consume** packages (including internal libs).

## Key differences

### Syntax and runtime

**CommonJS** (mostly synchronous loading):

```js
const fs = require("node:fs");
module.exports = { read: fs.readFileSync };
```

**ESM** (more static, async module loading semantics):

```js
import fs from "node:fs";
export const read = fs.readFileSync;
```

### How Node decides whether a `.js` file is CJS or ESM

- If the file is **`.mjs`**: always **ESM**
- If the file is **`.cjs`**: always **CommonJS**
- If the file is **`.js`**:
  - if `package.json` has **`"type": "module"`** => `.js` is treated as **ESM**
  - otherwise => `.js` is treated as **CommonJS**

This directly impacts libraries: if you set `"type":"module"`, your `index.js` becomes ESM and `require("your-lib")` can break if you don't provide a CJS entry.

### Top-level await

- **ESM** allows `await` at the top level.
- **CJS** does not (without wrapping in an `async` function).

### Available globals

In **CJS** you have `__dirname` / `__filename`.

In **ESM** you use `import.meta.url`:

```js
import { fileURLToPath } from "node:url";
import path from "node:path";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
```

## Interop (what works / what breaks)

### 1) Importing CommonJS from ESM

ESM can import CJS, but the shape often becomes “default-like”:

```js
// lib-cjs.cjs
module.exports = { Logger: class Logger {}, createLogger() {} };

// consumer.mjs
import pkg from "./lib-cjs.cjs";
const { Logger } = pkg;
```

If you want to support `import { Logger } from "pkg"`, the package must expose **real ESM named exports** (or an ESM wrapper that re-exports them).

### 2) Using `require()` to load ESM

`require()` does **not** load ESM directly. Usually you need to:

- use dynamic `import()` (async), or
- ship a CJS build (`.cjs`) in your package.

Example using dynamic import (from CJS):

```js
(async () => {
  const mod = await import("some-esm-only-package");
  mod.doSomething();
})();
```

## Publishing a package that works in both CJS and ESM (dual package)

If your team has a mix of projects, the lowest-friction approach is to publish:

- a **CJS** entry (e.g. `index.cjs`) for `require(...)`
- an **ESM** entry (e.g. `index.js` with `type: module` or `index.mjs`) for `import ...`
- and map both via `exports`.

Example `package.json`:

```json
{
  "name": "@minha-org/logger",
  "type": "module",
  "main": "./index.cjs",
  "exports": {
    ".": {
      "import": "./index.js",
      "require": "./index.cjs",
      "types": "./index.d.ts"
    }
  }
}
```

This enables:

```js
// ESM
import { Logger } from "@minha-org/logger";

// CJS
const { Logger } = require("@minha-org/logger");
```

## What to consider when migrating from CommonJS to ESM

### Checklist of common breakages

1. **Imports often need file extensions** in many ESM scenarios

   - In Node ESM, `import "./util"` may fail; prefer `import "./util.js"` (or rely on package `exports`).

1. **`__dirname` and `__filename`**

   - Replace with `import.meta.url` (see snippet above).

1. **Mocks/tests (Jest/ts-jest)**

   - Configuration changes a lot in ESM.

1. **Runtime/tooling**

   - `ts-node`, `nodemon`, ESM loaders, bundlers.

1. **ESM-only dependencies**

   - If your app is CJS, an ESM-only dependency may force dynamic `import()`.

1. **Default export vs named export**

   - CJS can look like it has a default export when imported from ESM, but it’s not the same as real ESM named exports.

### Practical strategy (migrate without breaking everyone)

1. Start by enabling ESM in your app (or library) via:
   - `"type": "module"` or `.mjs` files.
1. Gradually convert `require` -> `import`.
1. For libraries consumed by many projects, publish a **dual package** via `exports`.
1. Before publishing: use `npm pack` and test in a consumer project.

## Quick examples

### Example A) ESM project from scratch

`package.json`:

```json
{ "type": "module" }
```

`main.js`:

```js
import { Logger } from "logger_interas";

const logger = new Logger();
logger.info("ok");
```

### Example B) CommonJS project consuming a dual package

```js
const { createLogger } = require("logger_interas");
createLogger("api").warn("warn");
```

### Example C) Avoid headaches: use `.cjs`/`.mjs` when needed

- `scripts/build.cjs`: scripts de build em CJS mesmo num repo ESM
- `scripts/run.mjs`: scripts ESM mesmo num repo CJS

## Useful links

- Node.js Packages (CJS/ESM, exports, type): https://nodejs.org/api/packages.html
- `"type"` no `package.json`: https://nodejs.org/api/packages.html#type
- `exports`: https://nodejs.org/api/packages.html#exports
- `npm pack`: https://docs.npmjs.com/cli/v10/commands/npm-pack
