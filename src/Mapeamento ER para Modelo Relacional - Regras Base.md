# Mapeamento ER para Modelo Relacional - Regras Base

______________________________________________________________________

**Date:** 2026-05-20
**Tags:** [BancoDeDados](../tags/BancoDeDados.md), [ModelagemDeDados](../tags/ModelagemDeDados.md), [SQL](../tags/SQL.md), [ModeloER](../tags/ModeloER.md), [ModeloEER](../tags/ModeloEER.md), [MapeamentoMERMR](../tags/MapeamentoMERMR.md)
**URL:** https://www.db-book.com/ | https://dev.mysql.com/doc/refman/8.4/en/create-table-foreign-keys.html

______________________________________________________________________

## Ideia central

- MER/ER é conceitual; MR/modelo relacional é lógico.
- Mapear MER para MR significa transformar entidades, atributos e relacionamentos em tabelas, colunas, chaves primárias e chaves estrangeiras.
- O projeto lógico costuma ser: **modelo ER → modelo relacional**.

## Algoritmo clássico de mapeamento

1. Mapear entidades fortes.
2. Mapear entidades fracas.
3. Mapear relacionamentos binários 1:1.
4. Mapear relacionamentos binários 1:N.
5. Mapear relacionamentos binários M:N.
6. Mapear atributos multivalorados.
7. Mapear relacionamentos n-ários.

## Antes do mapeamento: EER rápido

- Hierarquia de especialização: cada subclasse tem apenas uma superclasse.
- Reticulado de especialização: uma subclasse pode ter várias superclasses.
- Categoria/tipo de união: uma subclasse representa subconjunto da união de superclasses diferentes.
- Agregação: trata um relacionamento como objeto de nível superior.

## Mapeando entidade forte

Regra:

- Cada entidade forte vira uma tabela.
- Atributos simples viram colunas.
- Atributos compostos viram seus componentes simples.
- Escolha um atributo-chave como chave primária.
- Atributos derivados normalmente não são armazenados.
- Atributos multivalorados ainda não entram aqui; viram tabela separada.

Exemplo:

```sql
CREATE TABLE FUNCIONARIO (
  cpf_funcionario CHAR(11) PRIMARY KEY,
  primeiro_nome VARCHAR(20) NOT NULL,
  nome_meio VARCHAR(20),
  ultimo_nome VARCHAR(40) NOT NULL,
  data_nascimento DATE,
  endereco VARCHAR(50),
  sexo CHAR(1),
  salario REAL
);
```

## Mapeando entidade fraca

Regra:

- Crie uma tabela para a entidade fraca.
- Inclua seus atributos simples.
- Inclua a chave primária da entidade proprietária como chave estrangeira.
- A chave primária da entidade fraca é composta: `PK_proprietario + chave_parcial`.
- O relacionamento de identificação é mapeado automaticamente.

Exemplo:

```sql
CREATE TABLE DEPENDENTE (
  cpf_funcionario CHAR(11),
  nome_dependente VARCHAR(50),
  sexo CHAR(1),
  data_nascimento DATE,
  parentesco VARCHAR(20),
  PRIMARY KEY (cpf_funcionario, nome_dependente),
  FOREIGN KEY (cpf_funcionario) REFERENCES FUNCIONARIO(cpf_funcionario)
);
```

Dois funcionários diferentes podem ter dependentes com o mesmo nome, porque a PK inclui `cpf_funcionario`.

## Mapeando atributo multivalorado

Regra:

- Crie uma nova tabela.
- Inclua a chave primária da entidade original como FK.
- Inclua o valor do atributo multivalorado.
- Use PK composta: `PK_entidade + atributo_multivalorado`.

Exemplo:

```sql
CREATE TABLE LOCALIZACAO_DEPARTAMENTO (
  numero_departamento INT,
  localizacao VARCHAR(50),
  PRIMARY KEY (numero_departamento, localizacao),
  FOREIGN KEY (numero_departamento) REFERENCES DEPARTAMENTO(numero_departamento)
);
```

Não armazene lista separada por vírgula em uma coluna; isso quebra normalização.

## Mapeando relacionamento 1:1

Abordagem comum: chave estrangeira.

- Escolha como relação referenciadora a entidade com participação total, se existir.
- Adicione nela a chave primária da outra entidade como FK.
- Como é 1:1, a FK deve ser `UNIQUE`.
- Atributos do relacionamento ficam na relação referenciadora.
- Se as duas entidades têm participação total, pode haver fusão das tabelas.

Exemplo:

```sql
ALTER TABLE DEPARTAMENTO
  ADD cpf_gerente CHAR(11) UNIQUE NOT NULL,
  ADD data_inicio_gerencia DATE,
  ADD FOREIGN KEY (cpf_gerente) REFERENCES FUNCIONARIO(cpf_funcionario);
```

## Mapeando relacionamento 1:N

Regra:

- Coloque a FK no lado N, isto é, na tabela da entidade que se liga a uma única ocorrência do lado 1.
- Atributos do relacionamento também ficam nessa tabela.
- Se a participação for total, use `NOT NULL`.

Exemplo:

```sql
ALTER TABLE FUNCIONARIO
  ADD numero_departamento INT NOT NULL,
  ADD FOREIGN KEY (numero_departamento) REFERENCES DEPARTAMENTO(numero_departamento);
```

Interpretação: muitos funcionários trabalham para um departamento; cada funcionário aponta para um departamento.

## Mapeando relacionamento recursivo 1:N

A FK aponta para a própria tabela.

```sql
ALTER TABLE FUNCIONARIO
  ADD cpf_supervisor CHAR(11),
  ADD FOREIGN KEY (cpf_supervisor) REFERENCES FUNCIONARIO(cpf_funcionario);
```

Se nem todo funcionário tem supervisor, a FK pode ser nula.

## Mapeando relacionamento M:N

Regra:

- Crie uma tabela para o relacionamento.
- Inclua as PKs das entidades participantes como FKs.
- A PK da nova tabela costuma ser composta pelas FKs.
- Atributos do relacionamento entram nessa nova tabela.

Exemplo:

```sql
CREATE TABLE TRABALHA_EM (
  numero_projeto INT,
  cpf_funcionario CHAR(11),
  horas SMALLINT,
  PRIMARY KEY (numero_projeto, cpf_funcionario),
  FOREIGN KEY (numero_projeto) REFERENCES PROJETO(numero_projeto),
  FOREIGN KEY (cpf_funcionario) REFERENCES FUNCIONARIO(cpf_funcionario)
);
```

## Decisão rápida para prova

- Entidade forte → tabela própria.
- Entidade fraca → tabela própria com PK do dono + chave parcial.
- Atributo multivalorado → tabela própria.
- Relacionamento 1:1 → FK única; total-total pode fundir.
- Relacionamento 1:N → FK no lado N.
- Relacionamento M:N → tabela associativa.
- Relacionamento n-ário → tabela associativa com n FKs.
