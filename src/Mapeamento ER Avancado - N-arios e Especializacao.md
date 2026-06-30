# Mapeamento ER Avancado - N-arios e Especializacao

______________________________________________________________________

**Date:** 2026-05-20
**Tags:** [BancoDeDados](../tags/BancoDeDados.md), [ModelagemDeDados](../tags/ModelagemDeDados.md), [SQL](../tags/SQL.md), [ModeloER](../tags/ModeloER.md), [ModeloEER](../tags/ModeloEER.md), [MapeamentoMERMR](../tags/MapeamentoMERMR.md)
**URL:** https://www.db-book.com/ | https://dev.mysql.com/doc/refman/8.4/en/create-table.html

______________________________________________________________________

## Recapitulação das estratégias de relacionamento

- **Tabela própria**: cria uma tabela para o relacionamento.
- **Adição de coluna**: adiciona uma FK em uma das entidades.
- **Fusão de tabelas**: une entidades em uma tabela só.

Uso típico:

- 1:1 total-total → fusão pode ser indicada.
- 1:1 ou 1:N → adição de FK costuma ser suficiente.
- M:N e n-ário → tabela própria é a regra mais comum.

## Mapeando relacionamento n-ário

Regra geral:

- Crie uma tabela para o relacionamento.
- Inclua as chaves primárias de todas as entidades participantes como FKs.
- Inclua os atributos do relacionamento.
- A PK da nova tabela normalmente é a combinação das FKs.

Exemplo ternário:

```text
FORNECEDOR -- FORNECE -- PROJETO
                    |
                  PECA
```

```sql
CREATE TABLE FORNECE (
  nome_fornecedor VARCHAR(50),
  nome_projeto VARCHAR(50),
  numero_peca INT,
  quantidade_peca INT,
  PRIMARY KEY (nome_fornecedor, nome_projeto, numero_peca),
  FOREIGN KEY (nome_fornecedor) REFERENCES FORNECEDOR(nome_fornecedor),
  FOREIGN KEY (nome_projeto) REFERENCES PROJETO(nome_projeto),
  FOREIGN KEY (numero_peca) REFERENCES PECA(numero_peca)
);
```

## Quando um lado do n-ário tem cardinalidade 1

Se um lado tem cardinalidade máxima `1`, as outras entidades determinam esse lado.

Exemplo: se `FORNECEDOR` é o lado `1`, cada par `(PROJETO, PECA)` determina no máximo um fornecedor.

```sql
CREATE TABLE FORNECE (
  nome_fornecedor VARCHAR(50) NOT NULL,
  nome_projeto VARCHAR(50),
  numero_peca INT,
  quantidade_peca INT,
  PRIMARY KEY (nome_projeto, numero_peca),
  FOREIGN KEY (nome_fornecedor) REFERENCES FORNECEDOR(nome_fornecedor),
  FOREIGN KEY (nome_projeto) REFERENCES PROJETO(nome_projeto),
  FOREIGN KEY (numero_peca) REFERENCES PECA(numero_peca)
);
```

Importante: a FK do lado `1` continua existindo; ela apenas pode ficar fora da PK composta.

## Questão clássica: grau 4

Um relacionamento quaternário com 4 entidades normalmente gera:

- 4 tabelas para as entidades.
- 1 tabela para o relacionamento.
- Total: 5 relações.

## Mapeando especialização/generalização: abordagem 1

Crie uma tabela para a superclasse e uma tabela para cada subclasse.

- Superclasse recebe atributos comuns.
- Subclasse recebe atributos específicos.
- PK da superclasse entra na subclasse como PK e FK.
- Funciona em qualquer caso.

Exemplo:

```sql
CREATE TABLE FUNCIONARIO (
  cpf_funcionario CHAR(11) PRIMARY KEY,
  primeiro_nome VARCHAR(20),
  salario REAL
);

CREATE TABLE SECRETARIO (
  cpf_funcionario CHAR(11) PRIMARY KEY,
  velocidade_digitacao REAL,
  FOREIGN KEY (cpf_funcionario) REFERENCES FUNCIONARIO(cpf_funcionario)
);
```

## Abordagem 2: apenas subclasses

Crie uma tabela para cada subclasse contendo atributos comuns e específicos.

- Não cria tabela da superclasse.
- Funciona apenas para **disjunção total**.
- É ruim se houver relacionamentos ligados à superclasse ou muitas subclasses.

Exemplo:

```sql
CREATE TABLE SECRETARIO (
  cpf_funcionario CHAR(11) PRIMARY KEY,
  primeiro_nome VARCHAR(20),
  salario REAL,
  velocidade_digitacao REAL
);
```

## Abordagem 3: uma tabela com atributo tipo

Crie só a tabela da superclasse com todos os atributos.

- Adicione um atributo `tipo`.
- Serve para especialização por **disjunção**.
- Pode gerar muitos valores nulos.
- Em disjunção parcial, `tipo` pode ser nulo se a entidade não pertencer a nenhuma subclasse.

Exemplo:

```sql
CREATE TABLE FUNCIONARIO (
  cpf_funcionario CHAR(11) PRIMARY KEY,
  primeiro_nome VARCHAR(20),
  tipo VARCHAR(20),
  velocidade_digitacao REAL,
  grau_tec VARCHAR(50),
  tipo_eng VARCHAR(50)
);
```

## Abordagem 4: uma tabela com flags booleanas

Crie só a tabela da superclasse com todos os atributos.

- Adicione uma flag por subclasse.
- Representa sobreposição, pois mais de uma flag pode ser verdadeira.
- Funciona em qualquer caso, mas é mais indicada para sobreposição.
- Também pode gerar muitos valores nulos.

Exemplo:

```sql
CREATE TABLE FUNCIONARIO (
  cpf_funcionario CHAR(11) PRIMARY KEY,
  primeiro_nome VARCHAR(20),
  flag_secretario BOOLEAN,
  flag_tecnico BOOLEAN,
  flag_engenheiro BOOLEAN,
  velocidade_digitacao REAL,
  grau_tec VARCHAR(50),
  tipo_eng VARCHAR(50)
);
```

## Escolha rápida por restrição

- Qualquer caso: abordagem 1.
- Disjunção total: qualquer abordagem pode funcionar.
- Disjunção parcial: não use abordagem 2.
- Sobreposição parcial ou total: prefira abordagem 1 ou 4.
- Se a prova perguntar “mais geral/segura”, escolha superclasse + subclasses.

## Pegadinhas de prova

- Atributo de relacionamento deve ir para a tabela do relacionamento ou para a tabela referenciadora.
- Em 1:1 com FK, lembre do `UNIQUE`.
- Em M:N, não basta uma FK simples; precisa de tabela associativa.
- Em n-ário, uma tabela extra representa o relacionamento.
- Grau 4 com quatro entidades geralmente vira 5 relações.
- Especialização total obriga participação em subclasse; parcial não obriga.
- Disjunção não permite múltiplas subclasses; sobreposição permite.
