---
name: fix-spelling
description: Fix only spelling, accentuation, capitalization and obvious typos in a note under src/, in English or Portuguese, without changing the content in any way. Use when the user gives a file in src/ and asks to correct its writing/grafia/spelling.
model: sonnet
effort: low
---

Corrija **apenas a grafia** do arquivo indicado pelo usuário (normalmente em `src/`). O conteúdo não muda em absolutamente nada.

Se o usuário não disse qual arquivo, pergunte antes de editar. Se ele deu um nome parcial, use Glob em `src/` para localizar; se houver mais de um candidato, pergunte qual.

## O que corrigir

- Palavras escritas errado / typos (`respondas` → `responds`, `recieve` → `receive`).
- Acentuação e cedilha em português (`nao` → `não`, `funcao` → `função`).
- Letras trocadas, duplicadas ou faltando; espaços duplicados; espaço antes de pontuação.
- Maiúscula em nomes próprios e siglas técnicas quando é claramente erro de digitação (`javascript` → `JavaScript`, `tcp` → `TCP`) **somente no meio de frases em prosa**, nunca em títulos, comandos ou identificadores.

O arquivo pode misturar inglês e português. Corrija cada trecho no idioma em que ele está escrito — nunca traduza.

## O que NÃO tocar

- Redação, ordem das frases, gramática, concordância, estilo, tom, pontuação de fim de frase.
- Nada de adicionar, remover, resumir, expandir ou reorganizar texto — nem uma palavra.
- Blocos de código, comandos inline, saídas de terminal, nomes de arquivos/variáveis/flags, URLs.
- Frontmatter, cabeçalhos de metadados (`**Date:**`, `**Tags:**`, `**URL:**`), links wiki/markdown e caminhos de imagem.
- Estrutura markdown: níveis de `#`, bullets, linhas em branco, separadores `___`.
- Abreviações e escrita informal intencional (`vc`, `pq`, `tbm`) — são escolhas do autor, não erros.

Em caso de dúvida se algo é erro ou intenção, **deixe como está** e mencione no relatório final.

## Como fazer

1. Read no arquivo inteiro.
2. Aplique as correções com Edit (um Edit por trecho; `replace_all` quando o mesmo erro repete de forma idêntica). Não reescreva o arquivo com Write.
3. Verifique o diff: `git diff -- <arquivo>` — cada linha alterada deve diferir só por grafia. Se alguma mudança alterou palavras/estrutura, reverta aquela edição.
4. Não faça commit, não faça stage, não mexa em outros arquivos.

## Relatório final

Liste as correções em formato compacto, uma por linha:

```
linha 14: respondas → responds
linha 22: nao → não
```

Se nada precisou de correção, diga isso em uma frase. Ao final, liste os casos duvidosos que você deixou intactos, se houver.
