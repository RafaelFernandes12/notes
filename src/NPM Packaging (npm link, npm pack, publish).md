# NPM Packaging (npm link, npm pack, publish)

______________________________________________________________________

**Date:** 2026-03-06
**Tags:** [Npm](../tags/Npm.md), [Js](../tags/Js.md)
**URL:** https://docs.npmjs.com/cli/v10/commands/npm-pack

______________________________________________________________________

## O que é

“Empacotar para NPM” é transformar um diretório com código (e um `package.json`) em um **artefato instalável** (`.tgz`) e publicável em um registry (npmjs, Nexus, etc.).

Na prática, você quer:

- Um **entrypoint** estável (`main`/`exports`) para importação.
- Um conjunto de arquivos publicado **controlado** (`files` / `.npmignore`).
- Uma forma rápida de testar **localmente** (sem publicar): `npm link` e `npm pack`.

## Como criar um pacote npm (o mínimo que funciona)

### Estrutura típica

```text
my-lib/
  package.json
  index.js
  README.md
```

### `package.json` mínimo (CommonJS)

```json
{
  "name": "@minha-org/minha-lib",
  "version": "1.0.0",
  "main": "./index.js",
  "exports": {
    ".": "./index.js"
  },
  "files": [
    "index.js",
    "README.md"
  ]
}
```

Pontos importantes:

- **`name`**: se for interno, prefira **escopo** (`@minha-org/...`) para controlar registry por scope.
- **`main`**: entrypoint para `require('pkg')` (CommonJS).
- **`exports`**: mapa moderno de entradas (evita importações “por dentro” do pacote).
- **`files`**: o que realmente vai para o tarball/publicação.

### Exemplo de export (index.js)

```js
"use strict";

function hello(name) {
  return `hello ${name}`;
}

module.exports = { hello };
```

### Como importar no consumidor

CommonJS:

```js
const { hello } = require("@minha-org/minha-lib");
console.log(hello("world"));
```

ESM (se o pacote suportar ESM ou se seu bundler resolver):

```js
import { hello } from "@minha-org/minha-lib";
console.log(hello("world"));
```

> Dica: se você quer compatibilidade ampla, o caminho “profissional” é publicar **dual build** (CJS + ESM) com `exports` mapeando `require`/`import`.

## `npm pack`: testar exatamente o que seria publicado

### O que ele faz

`npm pack` gera um arquivo `.tgz` com o conteúdo do pacote **exatamente** como seria publicado em um registry.

Use isso para validar:

- se `dist/` entrou,
- se você não está publicando `node_modules/` sem querer,
- se o entrypoint funciona quando instalado “de verdade”.

### Workflow produtivo (tarball)

No repo da lib:

```bash
npm pack
```

Isso cria algo como:

```text
minha-lib-1.0.0.tgz
```

No repo consumidor:

```bash
npm i /caminho/absoluto/minha-lib-1.0.0.tgz
```

Depois, teste importando:

```bash
node -e "const { hello } = require('@minha-org/minha-lib'); console.log(hello('ok'))"
```

### Por que `npm pack` é melhor que “copiar pasta”

- Você testa **resolução de módulo** como produção.
- Você testa se seu `files`/`.npmignore` está correto.
- Você detecta rápido “publiquei sem `dist/`”.

## `npm link`: desenvolvimento local (symlink)

### O que é

`npm link` cria um **symlink global** do seu pacote e depois liga esse pacote em outro projeto, apontando para o seu código local.

### Como usar

No repo da lib:

```bash
npm link
```

No repo consumidor:

```bash
npm link @minha-org/minha-lib
```

Agora o consumidor resolve `@minha-org/minha-lib` para seu diretório local.

### Quando usar / quando evitar

Use quando:

- Você quer iterar rápido (edita a lib e testa no app).

Evite quando:

- Você tem tooling que não lida bem com symlink (alguns watchers, monorepos, builds).
- Você quer garantir “igual produção” (use `npm pack`).

### Como desfazer

No consumidor:

```bash
npm unlink @minha-org/minha-lib
```

E (se quiser) remova o link global:

```bash
npm unlink -g @minha-org/minha-lib
```

## Publicar no Nexus (npm registry)

### Conceito

Nexus funciona como um registry npm. O comum é ter:

- **hosted**: onde você publica seus pacotes internos
- **proxy**: cache do registry público
- **group**: URL única combinando hosted + proxy

### Configurar registry por scope (recomendado)

No `.npmrc` (do usuário ou do projeto):

```ini
@minha-org:registry=https://nexus.minha-empresa.com/repository/npm-group/
always-auth=true

; token/credenciais variam por setup do Nexus
;//nexus.minha-empresa.com/repository/npm-group/:_authToken=XXXX
```

Publicação normalmente vai para o **hosted** (depende do seu setup):

```bash
npm publish --registry=https://nexus.minha-empresa.com/repository/npm-hosted/
```

## Erros comuns ao testar em outro repo

### 1) `ERESOLVE` ao instalar um `.tgz`

Mesmo instalando um tarball local, o npm recalcula a árvore toda. Se o repo consumidor já tem **conflito de peer dependencies**, qualquer `npm i ...` pode falhar.

Saídas rápidas (só para testar):

```bash
npm i --legacy-peer-deps /caminho/minha-lib-1.0.0.tgz
```

### 2) `require('./index')` importando arquivo errado

Se seu projeto tiver um arquivo chamado `index` (sem extensão) e um `index.js`, `require('./index')` pode resolver para o arquivo “errado”.

Prefira:

- `require('.')` (usa `package.json` do diretório)
- `require('./index.js')` (explícito)

## Cheat sheet

```bash
# gera tarball “igual publicação”
npm pack

# instala um tarball local no projeto consumidor
npm i /abs/path/pkg-1.0.0.tgz

# cria link global da lib
npm link

# linka a lib no consumidor
npm link @scope/pkg
```

## Links úteis

- `npm pack`: https://docs.npmjs.com/cli/v10/commands/npm-pack
- `npm link`: https://docs.npmjs.com/cli/v10/commands/npm-link
- `npm publish`: https://docs.npmjs.com/cli/v10/commands/npm-publish
- `package.json` (campos): https://docs.npmjs.com/cli/v10/configuring-npm/package-json
- `exports`: https://nodejs.org/api/packages.html#exports
- SemVer: https://semver.org/
