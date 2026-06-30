# Banco de Dados - Revisao Geral da Prova

______________________________________________________________________

**Date:** 2026-05-20
**Tags:** [BancoDeDados](../tags/BancoDeDados.md), [SQL](../tags/SQL.md), [ModelagemDeDados](../tags/ModelagemDeDados.md), [AlgebraRelacional](../tags/AlgebraRelacional.md), [ModeloER](../tags/ModeloER.md), [ModeloEER](../tags/ModeloEER.md), [MapeamentoMERMR](../tags/MapeamentoMERMR.md)
**URL:** https://www.db-book.com/ | https://dev.mysql.com/doc/ | https://www.postgresql.org/docs/current/queries.html

______________________________________________________________________

## Conteúdo da avaliação

- Álgebra relacional.
- Modelo ER.
- Modelo EER: especialização e generalização.
- Mapeamento MER/MEER para modelo relacional.

## Álgebra relacional: o que memorizar

- Seleção `σ`: filtra linhas.
- Projeção `Π`: filtra colunas.
- Renomeação `ρ`: renomeia relação ou atributos.
- União `∪`: junta tuplas de relações compatíveis.
- Diferença `−`: tuplas de uma relação que não estão na outra.
- Produto cartesiano `×`: combina todas as tuplas de duas relações.
- Interseção `∩`: tuplas comuns; pode ser derivada.
- Junção `⋈`: produto cartesiano + condição + projeção.

## Exemplos de álgebra

```text
σ_Peso>100(PRODUTO)
```

Filtra produtos com peso maior que 100.

```text
Π_Nome,Peso(σ_Peso>100(PRODUTO))
```

Filtra linhas por peso e depois mostra só nome e peso.

```text
R − S
```

Mostra o que está em `R` e não está em `S`.

## Questões rápidas de álgebra

- Consulta resultou em todas as linhas, mas só atributos desejados: **projeção**.
- Obter nome e peso para produtos com peso > 100: `Π_Nome,Peso(σ_Peso>100(Produto))`.
- Combinar informações de duas relações quaisquer: **produto cartesiano**.
- União/diferença precisam de mesmo número de atributos e domínios compatíveis.

## Fases do projeto de banco de dados

- Levantamento/análise de requisitos: entender dados e funções.
- Projeto conceitual: gerar esquema ER.
- Projeto lógico: transformar ER em modelo relacional.
- Projeto físico: detalhes de implementação no SGBD.

## ER: notação e conceitos

- Retângulo: entidade.
- Retângulo duplo: entidade fraca.
- Losango: relacionamento.
- Losango duplo: relacionamento identificador.
- Elipse: atributo.
- Elipse sublinhada: atributo-chave.
- Elipse dupla: atributo multivalorado.
- Elipse tracejada: atributo derivado.
- Atributo composto: elipse ligada a subelipses.

## ER: entidades e atributos

- Entidade forte: tem chave própria.
- Entidade fraca: depende de entidade proprietária e chave parcial.
- Atributo-chave no ER é candidato; a PK é escolhida no modelo relacional.
- Atributo derivado pode ser calculado, como idade por data de nascimento.
- Atributo multivalorado deve virar tabela separada no mapeamento.

## ER: relacionamentos

- Grau binário: 2 entidades.
- Grau ternário: 3 entidades.
- Grau quaternário: 4 entidades.
- Recursivo: mesma entidade participa mais de uma vez.
- Cardinalidade máxima: 1:1, 1:N, M:N.
- Participação mínima: total ou parcial.

## Mapeamento ER → relacional

- Entidade forte → tabela.
- Entidade fraca → tabela com FK do dono; PK = FK do dono + chave parcial.
- Atributo composto → componentes simples.
- Atributo derivado → normalmente não armazenar.
- Atributo multivalorado → tabela com FK da entidade + valor.
- Relacionamento 1:1 → FK única; se total-total, pode fundir.
- Relacionamento 1:N → FK no lado N.
- Relacionamento M:N → tabela associativa.
- Relacionamento n-ário → tabela associativa com n FKs.

## Exemplos de mapeamento

Entidade forte:

```sql
CREATE TABLE ALUNO (
  cpf_aluno CHAR(11) PRIMARY KEY,
  primeiro_nome VARCHAR(20),
  sobrenome VARCHAR(40),
  data_nascimento DATE
);
```

Atributo multivalorado:

```sql
CREATE TABLE TELEFONE_ALUNO (
  cpf_aluno CHAR(11),
  telefone CHAR(9),
  PRIMARY KEY (cpf_aluno, telefone),
  FOREIGN KEY (cpf_aluno) REFERENCES ALUNO(cpf_aluno)
);
```

Relacionamento 1:1 com atributo:

```sql
ALTER TABLE ENTIDADE2
  ADD cod1 CHAR(5) UNIQUE,
  ADD tipo VARCHAR(20),
  ADD FOREIGN KEY (cod1) REFERENCES ENTIDADE1(cod1);
```

Relacionamento M:N:

```sql
CREATE TABLE TRABALHA_EM (
  cpf_funcionario CHAR(11),
  numero_projeto INT,
  horas SMALLINT,
  PRIMARY KEY (cpf_funcionario, numero_projeto)
);
```

## Relacionamento n-ário

- Para grau 4 com quatro entidades: 4 tabelas de entidades + 1 tabela de relacionamento = 5 relações.
- Se um lado do n-ário tem cardinalidade `1`, as outras chaves podem determinar esse lado.
- Exemplo: se `(Projeto, Peça)` determina `Fornecedor`, a PK da relação pode ser `(Projeto, Peça)`.

## Entidade fraca com mais de um identificador

- A chave da entidade fraca combina chaves dos proprietários e chave parcial.
- No exemplo de `TRECHO_SOBREVOADO`, os destaques indicam algo como:

```text
PK(TRECHO_SOBREVOADO) = (Numero_trecho, Numero_voo, Data)
```

Leia sempre: chave do dono + chave parcial/atributo que diferencia a ocorrência.

## EER: especialização e generalização

- Especialização: superclasse → subclasses.
- Generalização: subclasses → superclasse.
- Disjunção `d`: entidade pertence a no máximo uma subclasse.
- Sobreposição `o`: entidade pode pertencer a várias subclasses.
- Total: toda entidade da superclasse participa de alguma subclasse.
- Parcial: entidade pode não participar de nenhuma subclasse.

## Mapeamento de especialização/generalização

- Abordagem 1: superclasse + subclasses; funciona em qualquer caso.
- Abordagem 2: só subclasses com atributos comuns e específicos; só funciona em disjunção total.
- Abordagem 3: só superclasse com atributo `tipo`; funciona para disjunção.
- Abordagem 4: só superclasse com flags booleanas; boa para sobreposição.
- Abordagens 3 e 4 podem gerar muitos valores nulos.

## Respostas/padrões que caem

- Projeto lógico relacional: ER → modelo relacional.
- Cardinalidade não é quantidade de atributos.
- Em 1:N, a FK normalmente fica no lado N, não no lado 1.
- Em 1:1 com FK, use `UNIQUE`.
- Atributo de relacionamento deve ser mapeado também.
- Entidade fraca usa PK que também contém FK do proprietário.
- Multivalorado não deve ficar como lista dentro da entidade.
- Especialização: V, V, V, F, F na questão da aula.

## Checklist antes de responder

1. Linhas ou colunas? Linhas = seleção; colunas = projeção.
2. M:N ou n-ário? Crie tabela associativa.
3. 1:N? FK no lado N. 1:1? FK única ou fusão se total-total.
4. Multivalorado? Tabela separada. Fraca? PK com chave do dono. Especialização? Veja disjunção/sobreposição e total/parcial.
