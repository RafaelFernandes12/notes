# Modelo Relacional (BD) — Instância, Esquema, Chaves e Restrições

______________________________________________________________________

**Date:** 2026-03-11
**Tags:** [Uf](../tags/Uf.md), [BancoDeDados](../tags/BancoDeDados.md), [SQL](../tags/SQL.md)
**Subject:** Banco de Dados — Introdução ao modelo relacional (recapitulação)
**Assets:**
**URL:** https://drive.google.com/file/d/18BxNNQBfRC65fbJ46wunA-3VqdwfibMY/view

______________________________________________________________________

## Conceitos principais

### Instância (estado) vs Esquema

- **Instância/estado**: dados armazenados *num momento do tempo* (muda).
- **Esquema**: “projeto” do banco (tabelas, colunas, tipos, restrições) (muda raramente).

Exemplo:

```sql
-- Esquema
CREATE TABLE aluno (
  matricula CHAR(12) PRIMARY KEY,
  nome      VARCHAR(100) NOT NULL
);

-- Instância
INSERT INTO aluno (matricula, nome) VALUES
('202000001000', 'Fulano'),
('202000002000', 'Sicrana');
```

### DDL vs DML

- **DDL** (*Data Definition Language*): define o esquema.
  - exemplos: `CREATE`, `ALTER`, `DROP`
- **DML** (*Data Manipulation Language*): manipula dados.
  - exemplos: `SELECT`, `INSERT`, `UPDATE`, `DELETE`

## Terminologia do modelo relacional

- **Relação**: tabela
- **Tupla**: linha
- **Atributo**: coluna
- **Domínio**: conjunto de valores permitidos para um atributo (no modelo relacional formal, deve ser **atômico/indivisível**)
- **Grau** da relação: número de atributos
- **NULL**: valor desconhecido ou inexistente

### Domínio atômico vs não atômico

Exemplo de atributo **não atômico** (ruim para o modelo relacional formal):

- `telefone = "(84) 99999-8888"` (mistura DDD + número)
- `endereco = "Rua X, 1001"` (vários componentes)

Alternativas (mais atômicas):

```sql
CREATE TABLE contato (
  id INT PRIMARY KEY,
  ddd CHAR(2) NOT NULL,
  numero CHAR(9) NOT NULL
);
```

## Características importantes

- A ordem das **tuplas** é irrelevante (tabela é conjunto).
- A ordem dos **atributos** também é irrelevante *se* a correspondência atributo→valor for mantida.
- Em SQL, a ordem **só existe** quando você usa `ORDER BY`.

Exemplo:

```sql
SELECT * FROM aluno;              -- sem ordem garantida
SELECT * FROM aluno ORDER BY nome; -- ordem explícita
```

## Restrições (constraints)

### Tipos

- **Implícitas** do modelo (ex.: ideia de relação/tupla/domínio atômico)
- **Semânticas / regras de negócio** (muitas vezes ficam na aplicação)
- **Explícitas no esquema** (via DDL):
  - domínio (`CHECK`, tipos)
  - chave (`PRIMARY KEY`, `UNIQUE`)
  - `NOT NULL`
  - integridade de entidade (PK não pode ser `NULL`)
  - integridade referencial (`FOREIGN KEY`)

## Chaves (superchave, candidata, primária, estrangeira)

### Superchave

Um conjunto de atributos **K** é superchave se identifica unicamente qualquer tupla.

Exemplo (ALUNO):

- Superchaves possíveis: `{matricula}`, `{cpf}`, `{matricula, cpf}`, `{matricula, nome}`, ...
- O projetista deve evitar superchaves com atributos supérfluos.

### Chave candidata (mínima)

Uma superchave é **candidata** se for **mínima** (nenhum subconjunto dela ainda é superchave).

Exemplo:

- `{matricula}` e `{cpf}` podem ser candidatas.
- `{matricula, cpf}` não é candidata (tem atributo extra).

### Chave primária

Uma das chaves candidatas escolhida como **identificador principal**.

Critérios comuns: estabilidade, simplicidade, tamanho pequeno, não-nula.

### Chave estrangeira

Um atributo (ou conjunto) em uma tabela que referencia a chave primária (ou `UNIQUE`) de outra (ou da mesma) tabela.

Exemplo (CURSO e PRE_REQUISITO):

```sql
CREATE TABLE curso (
  id_curso INT PRIMARY KEY,
  nome     VARCHAR(100) NOT NULL
);

CREATE TABLE pre_requisito (
  id_curso  INT NOT NULL,
  id_prereq INT NOT NULL,
  PRIMARY KEY (id_curso, id_prereq),
  FOREIGN KEY (id_curso)  REFERENCES curso(id_curso),
  FOREIGN KEY (id_prereq) REFERENCES curso(id_curso)
);
```

## Operações de atualização (DML)

### INSERT

```sql
INSERT INTO curso (id_curso, nome) VALUES (101, 'Matemática 1');
```

### UPDATE

```sql
UPDATE curso
SET nome = 'Matemática Básica'
WHERE id_curso = 101;
```

### DELETE

```sql
DELETE FROM curso
WHERE id_curso = 101;
```

## Transações (unidade de trabalho atômica)

Uma transação agrupa leituras/atualizações e deve deixar o BD em um **estado consistente** ao final.

Exemplo (MySQL):

```sql
START TRANSACTION;

INSERT INTO curso (id_curso, nome) VALUES (201, 'Física 1');
INSERT INTO pre_requisito (id_curso, id_prereq) VALUES (201, 101);

COMMIT;   -- ou ROLLBACK;
```

## Criando um esquema vazio no MySQL (mínimo)

```sql
CREATE DATABASE meu_bd;
USE meu_bd;
```

## Perguntas estilo prova (do PDF)

- Relação = tabela; tupla = linha.
- Chave primária pode ser composta (1+ campos) e identifica unicamente registros.
- Chave estrangeira referencia chave primária/única, criando relacionamento.
- NULL representa valor desconhecido/inexistente.
- A tabela **não** é “sempre ordenada”; ordem vem de `ORDER BY`.
