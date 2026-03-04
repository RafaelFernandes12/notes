# Banco de Dados (Introducao a SGBD)

______________________________________________________________________

**Date:** 2026-03-03
**Subject:** [BancoDeDados](../tags/BancoDeDados.md)
**Tags:**

- [Uf](../tags/Uf.md)
- [BancoDeDados](../tags/BancoDeDados.md)
- [SGBD](../tags/SGBD.md)
- [SQL](../tags/SQL.md)
- [ModelagemDeDados](../tags/ModelagemDeDados.md)
- [Transacoes](../tags/Transacoes.md)

**Assets:**

- [Aula_01_-_BD_-_2026.1.pdf](../assets/Aula_01_-_BD_-_2026.1.pdf)

**URL:**

- https://en.wikipedia.org/wiki/Database
- https://en.wikipedia.org/wiki/Database_management_system
- https://www.postgresql.org/docs/current/
- https://dev.mysql.com/doc/

______________________________________________________________________

## Summary

- **Banco de dados**: colecao de dados relacionados sobre um “minimundo”, com significado e finalidade.
- **SGBD/DBMS**: software que ajuda a **definir** (metadados), **construir**, **manipular** e **compartilhar** um banco de dados.
- Um SGBD tipicamente oferece: **catalogo/metadados**, **independencia de dados**, **multiplas visoes** e **controle de concorrencia** (transacoes).

---

## O que e um Banco de Dados?

Um **banco de dados** e uma colecao de dados que:

1) representa algum aspecto do mundo real (**minimundo**)
2) e logicamente coerente (tem significado)
3) tem uma finalidade (e util para um grupo de usuarios)

Exemplos (minimundo):

- Uma **universidade**: alunos, disciplinas, turmas, notas, pre-requisitos.
- Uma **loja**: produtos, pedidos, clientes, pagamentos.

> Um banco de dados nao precisa “ser grande” para ser BD: pode ser pequeno (ex.: lista de contatos), ou enorme (ex.: plataformas de video).

---

## O que e um SGBD (DBMS)?

Um **SGBD** e um sistema computadorizado que permite criar e manter um banco de dados.

Ele oferece (pelo menos):

- **Definicao**: tipos, estruturas e restricoes (guardados como **metadados** no catalogo do SGBD)
- **Construcao**: armazenamento efetivo dos dados
- **Manipulacao**: consultas, atualizacoes, relatorios
- **Compartilhamento**: varios usuarios e programas acessando ao mesmo tempo

---

## Abordagem de arquivos vs abordagem de banco de dados

### Abordagem de arquivos (sem SGBD)

- Cada aplicacao “cuida” do seu armazenamento (CSV/JSON/arquivos proprios)
- Duas aplicacoes podem duplicar os mesmos dados
- Se a estrutura muda, voce muda codigo em varios lugares

Mini exemplo (problema de duplicacao):

- `app_financeiro.csv` guarda cliente e endereco
- `app_entregas.csv` guarda cliente e endereco

Se o endereco muda e so uma app atualiza, voce passa a ter **inconsistencia**.

### Abordagem de banco de dados (com SGBD)

- Os dados ficam centralizados
- Varios usuarios/sistemas compartilham
- Mudancas no banco nem sempre exigem mudar programas (depende do caso) → **independencia de dados**

---

## Caracteristicas importantes de um SGBD (do PDF)

### 1) Natureza de autodescricao

O sistema guarda nao so os dados, mas tambem a **descricao** do que existe (tabelas, colunas, tipos, restricoes) no **catalogo** (metadados).

### 2) Isolamento entre programas e dados (abstracao)

Programas usam operacoes (ex.: “consultar alunos”) sem depender de detalhes internos de armazenamento.

Termos comuns:

- **Independencia de dados do programa**
- **Independencia da operacao do programa**

### 3) Suporte a multiplas visoes

Usuarios diferentes veem “fatias” diferentes dos mesmos dados.

### 4) Compartilhamento + transacoes multiusuario

Varios usuarios podem acessar ao mesmo tempo. O SGBD precisa garantir que operacoes concorrentes parecam **isoladas** (controle de concorrencia).

---

## Exemplos (com SQL)

### Exemplo 1: “minimundo universidade” (tabelas + consultas)

Um schema minimo (simplificado):

```sql
CREATE TABLE aluno (
  numero_aluno  INT PRIMARY KEY,
  nome          TEXT NOT NULL,
  curso         TEXT NOT NULL
);

CREATE TABLE disciplina (
  numero_disciplina TEXT PRIMARY KEY,
  nome_disciplina   TEXT NOT NULL,
  creditos          INT NOT NULL
);

CREATE TABLE pre_requisito (
  numero_disciplina    TEXT REFERENCES disciplina(numero_disciplina),
  numero_pre_requisito TEXT REFERENCES disciplina(numero_disciplina),
  PRIMARY KEY (numero_disciplina, numero_pre_requisito)
);

CREATE TABLE registro_nota (
  numero_aluno       INT  REFERENCES aluno(numero_aluno),
  numero_disciplina  TEXT REFERENCES disciplina(numero_disciplina),
  nota              TEXT,
  PRIMARY KEY (numero_aluno, numero_disciplina)
);
```

Consultas parecidas com as do PDF:

```sql
-- listar pre-requisitos de uma disciplina
SELECT numero_pre_requisito
FROM pre_requisito
WHERE numero_disciplina = 'CC3380';

-- recuperar disciplinas e notas de um aluno
SELECT rn.numero_disciplina, rn.nota
FROM registro_nota rn
WHERE rn.numero_aluno = 17;
```

Alteracoes:

```sql
-- incluir novo aluno
INSERT INTO aluno (numero_aluno, nome, curso)
VALUES (19, 'Rafael', 'CC');

-- inserir nota
INSERT INTO registro_nota (numero_aluno, numero_disciplina, nota)
VALUES (19, 'CC1310', 'A');
```

### Exemplo 2: multiplas visoes (VIEW)

Um usuario pode querer so o “historico” (visao):

```sql
CREATE VIEW historico_aluno AS
SELECT a.numero_aluno, a.nome, rn.numero_disciplina, rn.nota
FROM aluno a
JOIN registro_nota rn ON rn.numero_aluno = a.numero_aluno;

-- o usuario consulta a visao, nao as tabelas
SELECT * FROM historico_aluno WHERE numero_aluno = 17;
```

### Exemplo 3: concorrencia (por que transacoes importam)

Dois usuarios alterando o mesmo dado ao mesmo tempo podem gerar “atualizacao perdida” (lost update).

Ideia (pseudo):

```text
T1 le saldo = 100
T2 le saldo = 100
T1 saldo = saldo - 10  -> grava 90
T2 saldo = saldo - 20  -> grava 80   (perdeu o -10 da T1)
```

Um SGBD usa controle de concorrencia para evitar esse tipo de erro.

---

## Vantagens e desvantagens (resumo do PDF)

Vantagens:

- controlar redundancia
- restringir acesso nao autorizado (seguranca)
- persistencia
- processamento eficiente de consultas (indices, planos, etc.)
- backup e recuperacao
- varias interfaces
- representar relacionamentos complexos
- impor integridade (restricoes)
- regras e gatilhos (triggers)

Desvantagens:

- custo mais alto
- pode nao atender requisitos rigorosos de tempo real
- pode requerer mais armazenamento que arquivos simples

---

## Atores e fases de um projeto de BD

Atores:

- **DBA**: acesso, monitoramento, recursos, falhas
- **projetistas**: requisitos, dados, estruturas
- **usuarios finais**
- **analistas/programadores**

Fases:

1) especificacao e analise de requisitos
2) projeto conceitual (ex.: diagrama ER)
3) projeto logico
4) projeto fisico

---

## Questões (do PDF) — gabarito

1) Questao 1 (SGBD integra dados, concorrencia, recuperacao, seguranca) → **Verdadeiro**
2) Questao 2 (evitar redundancia evita copias em lugares diferentes) → **Verdadeiro** (na pratica: a ideia e *controlar* redundancia)
3) Questao 3 (SGBD gerencia acesso, seguranca, integridade, desempenho, disponibilidade) → **Verdadeiro**
4) Questao 4 (alterar estruturas sem mudar programas) → **c) Independencia de dados**

---

## Related notes

- [Spring Boot PostgreSQL (JPA + Hibernate)](Spring Boot PostgreSQL (JPA + Hibernate).md)
