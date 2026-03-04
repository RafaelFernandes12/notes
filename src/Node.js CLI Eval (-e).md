# Node.js CLI Eval (-e)

---

**Date:** 2026-03-04
**Tags:** [Js](../tags/Js.md), [Bash](../tags/Bash.md), [Processos](../tags/Processos.md)
**URL:** https://nodejs.org/api/cli.html#-e---eval-script

---

## O que é

`node -e` (ou `node --eval`) executa uma string de JavaScript diretamente no Node.js, sem precisar criar um arquivo. É ideal para testes rápidos, inspeções de ambiente e scripts “one‑off”.

## Como funciona (e onde roda)

- **Roda em um novo processo do Node** no mesmo terminal onde você executou o comando.
- **Diretório atual**: usa o `process.cwd()` do seu shell, e resolve módulos a partir dali (ex.: `node_modules` do projeto atual).
- **Variáveis de ambiente**: herda o `process.env` do shell. Se quiser carregar `.env`, precisa chamar `dotenv` manualmente.
- **Encerramento**: o processo termina quando o event loop fica vazio. Se você iniciar algo assíncrono, precisa aguardar a Promise ou manter o processo vivo até finalizar.

## Exemplos

### 1) Inspecionar onde está rodando

```bash
node -e "console.log('cwd:', process.cwd()); console.log('node:', process.version)"
```

Use isso para confirmar o diretório e a versão do Node usada pelo comando.

### 2) Transformação rápida de dados

```bash
node -e "const input='{\"a\":1,\"b\":2}'; const obj=JSON.parse(input); obj.c=obj.a+obj.b; console.log(JSON.stringify(obj))"
```

Útil para validar lógica rapidamente sem criar arquivo.

### 3) Envio de e‑mail isolado (mesma configuração do app)

```bash
node -e "require('dotenv').config({ path: '.env' }); const nodemailer=require('nodemailer'); const transporter=nodemailer.createTransport({ pool:true, host:process.env.SMTP_HOST, port:Number(process.env.SMTP_PORT), secure:false, auth:{ user:process.env.MAIL_USERNAME, pass:process.env.MAIL_PASSWORD }, tls:{ rejectUnauthorized:false } }); transporter.sendMail({ to:'<EMAIL>', from:process.env.MAIL_FROM, subject:'SMTP test '+new Date().toISOString(), text:'Isolated SMTP test' }).then(info=>{ console.log('INFO', info); }).catch(console.error);"
```

Esse teste roda **fora** do Nest/Bull e isola o SMTP (o mesmo `nodemailer` usado no app).

## Variações úteis

### 1) Imprimir retorno diretamente (`-p`)

```bash
node -p "1 + 2"
```

O `-p` avalia a expressão e já imprime o resultado (atalho para `console.log`).

### 2) Usar `import` ESM sem arquivo (`--input-type=module`)

```bash
node --input-type=module -e "import os from 'node:os'; console.log(os.platform())"
```

Útil para testar módulos ES direto no terminal.

### 3) Top‑level `await` (com ESM)

```bash
node --input-type=module -e "const value = await Promise.resolve('ok'); console.log(value)"
```

Permite testar código assíncrono rapidamente sem função `async` explícita.

### 4) Pré‑carregar módulos (`-r/--require`)

```bash
node -r dotenv/config -e "console.log(process.env.APP_NAME)"
```

Esse padrão injeta um módulo antes do `-e` rodar (útil para `dotenv`, instrumentação, etc).

### 5) Definir caminho do `.env` sem tocar no código

```bash
DOTENV_CONFIG_PATH=.env node -r dotenv/config -e "console.log(process.env.APP_NAME)"
```

### 6) Passar argumentos para o script (separador `--`)

```bash
node -e "console.log(process.argv.slice(2))" -- foo bar
```

Tudo após `--` vira argumento do seu script avaliado.

### 7) Entrar no REPL depois de executar (`-i`)

```bash
node -i -e "const total = 42; console.log(total)"
```

Bom para executar algo e continuar explorando o estado no REPL.

### 8) Definir variáveis de ambiente inline

```bash
API_URL=https://api.exemplo.com node -e "console.log(process.env.API_URL)"
```

### 9) Ler dados do `stdin`

```bash
printf "hello" | node -e "const fs=require('fs'); const input=fs.readFileSync(0,'utf8'); console.log(input.toUpperCase())"
```

Útil para pipelines rápidos no terminal.

### 10) Controlar o código de saída

```bash
node -e "console.error('falhou'); process.exit(2)"
```

Se preferir não abortar imediatamente, use `process.exitCode = 1`.

### 11) Debug rápido (`--inspect` / `--inspect-brk`)

```bash
node --inspect-brk -e "console.log('debug')"
```

Abre o inspector e pausa no início para você anexar um debugger.

### 12) Diagnóstico de warnings e rejeições

```bash
node --trace-warnings -e "process.emitWarning('teste')"
node --unhandled-rejections=strict -e "Promise.reject(new Error('boom'))"
```

Bom para descobrir onde warnings e rejeições estão acontecendo.

### 13) Usar `NODE_OPTIONS` para repetir flags

```bash
NODE_OPTIONS="--trace-warnings" node -e "process.emitWarning('x')"
```

### 14) Evitar escape de aspas no shell

```bash
node -e 'console.log("aspas duplas ficam fáceis aqui")'
```

Uma dica prática para deixar o comando mais legível.

## Quando usar

- Testes rápidos de configuração (SMTP, Redis, variáveis de ambiente).
- Inspeções locais sem alterar o código do projeto.
- Debug de pequenos trechos de lógica.

## Quando evitar

- Scripts longos ou complexos (prefira um arquivo `.js`).
- Quando precisar de controle maior de logs, tratamento de erro ou testes.
