# configure eslint

______________________________________________________________________

**Date:** 2026-02-16
**Tags:** [Algoritmos.md](tags/Algoritmos.md), [Leet_code.md](tags/Leet_code.md)
**URL:**

______________________________________________________________________

## Summary

install the dependencies

```bash
yarn add -D eslint @typescript-eslint/parser @typescript-eslint/eslint-plugin
```

create this in .eslintrc.js file to configure eslint for typescript projects

```js
const tsParser = require("@typescript-eslint/parser")
const tsPlugin = require("@typescript-eslint/eslint-plugin")
module.exports = [
  {
    ignores: ["dist/**", "node_modules/**"],
  },
  {
    files: ["**/*.ts"],
    languageOptions: {
      parser: tsParser,
      parserOptions: {
        ecmaVersion: 2020,
        sourceType: "module",
      },
    },
    plugins: {
      "@typescript-eslint": tsPlugin,
    },
    rules: {
      ...tsPlugin.configs.recommended.rules,
    },
  },
]
```
