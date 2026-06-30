# Modelo Entidade-Relacionamento ER - Resumo

______________________________________________________________________

**Date:** 2026-05-20
**Tags:** [BancoDeDados](../tags/BancoDeDados.md), [ModelagemDeDados](../tags/ModelagemDeDados.md), [ModeloER](../tags/ModeloER.md)
**URL:** https://www.db-book.com/ | https://app.diagrams.net/

______________________________________________________________________

## Onde o modelo ER entra no projeto de banco de dados

- **Levantamento e análise de requisitos**: entender requisitos de dados e requisitos funcionais.
- **Projeto conceitual**: criar o esquema conceitual, normalmente usando modelo ER.
- **Projeto lógico**: mapear o esquema conceitual para um modelo de implementação, geralmente o relacional.
- O ER é bom para comunicação porque mostra entidades, atributos e relacionamentos visualmente.

## Elementos básicos do ER

- **Entidade**: coisa ou objeto do mundo real com existência independente.
- **Atributo**: propriedade que descreve uma entidade ou relacionamento.
- **Relacionamento**: associação entre entidades.
- Em narrativas de requisitos, substantivos costumam virar entidades e verbos costumam virar relacionamentos.

## Entidade, tipo e conjunto

- Uma entidade individual é uma ocorrência concreta, como `João`, `Curso de BD` ou `Departamento 10`.
- Um tipo de entidade define objetos com características comuns, como `FUNCIONARIO`.
- Um conjunto de entidades é o conjunto de ocorrências daquele tipo.
- Entidade forte tem chave própria.
- Entidade fraca não tem chave própria suficiente e depende de uma entidade proprietária.

## Exemplo rápido de entidade forte

```text
ALUNO(Cpf, Nome, Data_nascimento)
```

- `ALUNO` é entidade forte se `Cpf` identifica cada aluno sozinho.
- No diagrama ER, entidade forte é retângulo simples.

## Exemplo rápido de entidade fraca

```text
FUNCIONARIO(Cpf, Nome)
DEPENDENTE(Primeiro_nome, Data_nascimento, Parentesco)
```

- `DEPENDENTE` pode ser identificado por `(Cpf_funcionario, Primeiro_nome)`.
- `Primeiro_nome` é chave parcial.
- O relacionamento com `FUNCIONARIO` é relacionamento de identificação.
- No DER clássico: entidade fraca é retângulo duplo; relacionamento identificador é losango duplo.

## Tipos de atributos

- **Simples**: não se divide em partes úteis. Ex.: `Cpf`, `Idade`.
- **Composto**: pode ser dividido. Ex.: `Nome = Prenome + Sobrenome`.
- **Monovalorado**: um valor por entidade. Ex.: `Cpf`.
- **Multivalorado**: vários valores por entidade. Ex.: `Telefone`.
- **Armazenado**: guardado diretamente. Ex.: `Data_nascimento`.
- **Derivado**: calculado a partir de outros. Ex.: `Idade` a partir de `Data_nascimento`.

## Notação de atributos no DER clássico

- Elipse simples: atributo comum.
- Elipse sublinhada: atributo-chave.
- Elipse dupla: atributo multivalorado.
- Elipse tracejada: atributo derivado.
- Elipse ligada a outras elipses: atributo composto.

## Atributo-chave no ER

- No ER, atributo-chave é mais parecido com **chave candidata**.
- A chave primária só será escolhida no mapeamento para o modelo relacional.
- Uma entidade pode ter mais de um atributo-chave candidato.

## Relacionamentos

- Quando um atributo de uma entidade referencia outra entidade, modele como relacionamento.
- Exemplo: em vez de colocar `curso_do_aluno` como atributo textual, modele `ALUNO ESTUDA CURSO`.
- Um tipo de relacionamento tem instâncias; cada instância associa entidades individuais.

## Grau de relacionamento

- **Binário**: duas entidades. Ex.: `ALUNO ESTUDA CURSO`.
- **Ternário**: três entidades. Ex.: `FORNECEDOR FORNECE PECA PARA PROJETO`.
- **Quaternário**: quatro entidades.
- **N-ário**: n entidades.
- **Recursivo**: a mesma entidade participa mais de uma vez. Ex.: `FUNCIONARIO SUPERVISIONA FUNCIONARIO`.

## Razão de cardinalidade

Determina o número máximo de instâncias de relacionamento.

- **1:1**: um funcionário gerencia no máximo um departamento e vice-versa.
- **1:N**: um departamento tem vários funcionários; cada funcionário pertence a um departamento.
- **M:N**: funcionário participa de vários projetos; projeto tem vários funcionários.

## Participação

Determina o número mínimo de instâncias de relacionamento.

- **Total**: entidade só existe se participar do relacionamento.
- **Parcial**: entidade pode existir sem participar do relacionamento.
- Em notação `(min, max)`, `0` indica opcionalidade e `1` indica obrigatoriedade mínima.

## Exemplo de leitura de cardinalidade

```text
PROVA (1,N) POSSUI (1,1) QUESTAO
```

- Cada prova possui uma ou mais questões.
- Cada questão pertence a exatamente uma prova.
- Cuidado: algumas bancas posicionam os números de forma diferente; sempre leia a semântica.

## Ferramentas para desenhar

- diagrams.net / draw.io.
- Canva.
- Lucidchart.
- PowerPoint ou desenho manual.

## Pegadinhas de prova

- Atributo derivado pode ser calculado a partir de outros atributos.
- `TelefonesContato` tende a ser multivalorado.
- `Endereço` costuma ser composto se seus componentes forem relevantes.
- Cardinalidade não é quantidade de atributos.
- Entidade é objeto/conceito do mundo real sobre o qual guardamos dados.
- Atributo multivalorado é elipse dupla, não retângulo duplo.
- Entidade fraca não é triângulo; é retângulo duplo.
