# Arquitetura de Tres Esquemas e Independencia de Dados Aula 2 Banco de dados(SGBD)

______________________________________________________________________

**Date:** 2026-03-07
**Subject:** [BancoDeDados.md](tags/BancoDeDados.md)
**Tags:**

- [Uf](../tags/Uf.md)
- [BancoDeDados](../tags/BancoDeDados.md)
- [SQL](../tags/SQL.md)

**Assets:** https://drive.google.com/file/d/1pZgnyMm7SHBxBHq51icM1N-cQF_RH_I-/view
**URL:**

- https://en.wikipedia.org/wiki/ANSI-SPARC_Three-Level_Architecture
- https://www.postgresql.org/docs/current/ddl-views.html
- https://www.postgresql.org/docs/current/sql-altertable.html
- https://www.postgresql.org/docs/current/sql-createindex.html

______________________________________________________________________

## Summary

- **Instancia (estado)** = os dados *agora* (muda o tempo todo). **Esquema** = a estrutura (muda raramente).
- **DDL** define estrutura (schema). **DML** consulta/insere/altera/deleta dados.
- A arquitetura **externo / conceitual / interno** explica **abstracao** e **independencia de dados**.
- **Independencia logica**: mudar o esquema conceitual sem quebrar visoes/programas. **Independencia fisica**: mudar armazenamento sem mudar o esquema conceitual.

______________________________________________________________________

## Instancia vs Esquema (estado vs estrutura)

### Exemplo (PostgreSQL): o que muda com DML e o que muda com DDL

```sql
-- DDL (define a estrutura)
CREATE TABLE aluno (
  numero_aluno INT PRIMARY KEY,
  nome         TEXT NOT NULL,
  curso        TEXT NOT NULL
);

-- DML (muda a instancia/estado)
INSERT INTO aluno (numero_aluno, nome, curso)
VALUES (17, 'Silva', 'CC');

SELECT * FROM aluno;
-- aqui a INSTANCIA mudou (tem uma linha nova)

-- esquema (estrutura) ainda e o mesmo: 3 colunas
SELECT column_name, data_type
FROM information_schema.columns
WHERE table_schema = 'public' AND table_name = 'aluno'
ORDER BY ordinal_position;

-- DDL muda o ESQUEMA
ALTER TABLE aluno ADD COLUMN email TEXT;

-- agora o esquema mudou (tem uma coluna a mais)
SELECT column_name, data_type
FROM information_schema.columns
WHERE table_schema = 'public' AND table_name = 'aluno'
ORDER BY ordinal_position;
```

Ideia-chave:

- `INSERT/UPDATE/DELETE` mudam **dados** (instancia).
- `CREATE/ALTER/DROP` mudam **estrutura** (esquema).

______________________________________________________________________

## DDL vs DML (linguagens de BD)

- **DDL (Data Definition Language)**: descreve o BD (tabelas, colunas, tipos, restricoes, views, indices).
  - exemplos: `CREATE TABLE`, `ALTER TABLE`, `DROP TABLE`, `CREATE VIEW`, `CREATE INDEX`.
- **DML (Data Manipulation Language)**: recupera e altera dados.
  - exemplos: `SELECT`, `INSERT`, `UPDATE`, `DELETE`.

______________________________________________________________________

## Abstracao de dados: arquitetura de tres esquemas

Modelo mental (ANSI/SPARC):

```text
Usuarios / Apps
   |
   |  (Niveis EXTERNOS = visoes)
   v
Esquemas externos (views, APIs, relatorios)
   |
   |  mapeamento externo <-> conceitual
   v
Esquema conceitual (modelo logico: tabelas, relacoes, restricoes)
   |
   |  mapeamento conceitual <-> interno
   v
Esquema interno/fisico (arquivos, paginas, indices, particionamento, compressao)
```

O que cada nivel responde:

- **Externo (visao)**: “que parte do BD este usuario precisa ver?”
- **Conceitual (logico)**: “quais dados existem e como se relacionam?”
- **Interno (fisico)**: “como isso esta armazenado e otimizado?”

______________________________________________________________________

## Independencia de dados

### Independencia logica (mais dificil)

Capacidade de alterar o **esquema conceitual** sem alterar **visoes externas** e programas.

#### Exemplo: VIEW isola a aplicacao de mudancas no esquema conceitual

Suponha que a aplicacao so precisa do “historico” (visao externa):

```sql
CREATE TABLE disciplina (
  codigo TEXT PRIMARY KEY,
  nome   TEXT NOT NULL
);

CREATE TABLE registro_nota (
  numero_aluno INT NOT NULL,
  codigo_disciplina TEXT NOT NULL REFERENCES disciplina(codigo),
  nota TEXT,
  PRIMARY KEY (numero_aluno, codigo_disciplina)
);

-- ESQUEMA EXTERNO: a app usa uma VIEW (contrato)
CREATE VIEW historico_aluno AS
SELECT numero_aluno, codigo_disciplina, nota
FROM registro_nota;

-- a app consulta a view
SELECT * FROM historico_aluno WHERE numero_aluno = 17;
```

Agora o BD cresce (mudanca no conceitual):

```sql
-- adiciona um novo dado no modelo conceitual
ALTER TABLE registro_nota ADD COLUMN semestre INT;
ALTER TABLE registro_nota ADD COLUMN ano INT;
```

Se a view mantiver as colunas antigas, a consulta da app continua funcionando:

```sql
SELECT * FROM historico_aluno WHERE numero_aluno = 17;
```

> Na pratica, *nem toda* mudanca logica e transparente. A ideia e reduzir o acoplamento (por exemplo, usando views, APIs e contratos de dados).

### Independencia fisica (mais simples)

Capacidade de alterar o **esquema interno/fisico** sem alterar o esquema conceitual.

#### Exemplo: criar indice (otimizacao) sem mudar consultas

```sql
-- consulta (DML) nao muda
SELECT * FROM registro_nota WHERE numero_aluno = 17;

-- mudanca fisica: criar um indice para acelerar a busca
CREATE INDEX idx_registro_nota_numero_aluno
ON registro_nota (numero_aluno);

-- mesma consulta; o SGBD escolhe outro plano (ex.: index scan)
EXPLAIN SELECT * FROM registro_nota WHERE numero_aluno = 17;
```

______________________________________________________________________

## Modelos de dados (conceitual, logico e fisico)

Um **modelo de dados** e um conjunto de conceitos para descrever:

- dados
- relacionamentos
- significado (semantica)
- restricoes

### 1) Conceitual (alto nivel): entidades, atributos e relacionamentos

Exemplo (ER, como no slide):

```text
Funcionario (CPF, Nome, Celular)
   N  trabalha_para  1
Departamento (id_departamento, Nome)
```

### 2) Logico/representativo: implementavel (modelo relacional)

Mapeando o ER para tabelas:

```sql
CREATE TABLE departamento (
  id_departamento INT PRIMARY KEY,
  nome            TEXT NOT NULL
);

CREATE TABLE funcionario (
  cpf             VARCHAR(11) PRIMARY KEY,
  nome            TEXT NOT NULL,
  celular         VARCHAR(11),
  id_departamento INT NOT NULL REFERENCES departamento(id_departamento)
);
```

### 3) Fisico (baixo nivel): decisao de armazenamento/otimizacao

Exemplos tipicos:

- criar indices (`CREATE INDEX ...`)
- escolher particionamento
- ajustar tablespaces/armazenamento
- tipos e tamanho de colunas pensando em pagina/cache

______________________________________________________________________

## Arquiteturas para SGBDs (visao de implantacao)

### 1) Centralizada

- Um “servidor grande” (mainframe) faz praticamente tudo.
- Terminais apenas exibem/mandam comandos.

### 2) Cliente/servidor (duas camadas)

```text
[Cliente (UI + alguma logica)]  <----rede---->  [Servidor de BD (SGBD)]
```

Exemplo: uma aplicacao desktop que se conecta direto no PostgreSQL.

### 3) Tres camadas (comum na web)

```text
[Cliente (browser/app)] <-> [Servidor de aplicacao (API)] <-> [Servidor de BD]
```

Beneficios comuns:

- separar responsabilidades (UI / regra de negocio / dados)
- controle de acesso (cliente nao fala direto com o BD)
- facilitar escalabilidade do servidor de aplicacao

______________________________________________________________________

## Auto-check (questoes rapidas)

1. “O esquema do banco muda a cada INSERT/UPDATE/DELETE.” → **Errado** (isso muda a instancia).

1. “Alterar o esquema conceitual sem alterar visoes externas” e independencia **logica** ou **fisica**? → **Logica**.

1. Qual costuma ser mais simples de obter? → **Independencia fisica** (otimizacoes internas sem mudar o modelo logico).

______________________________________________________________________
