# Algebra Relacional - Resumo para Prova

______________________________________________________________________

**Date:** 2026-05-20
**Tags:** [BancoDeDados](../tags/BancoDeDados.md), [SQL](../tags/SQL.md), [AlgebraRelacional](../tags/AlgebraRelacional.md)
**URL:** https://www.db-book.com/ | https://dev.mysql.com/doc/refman/8.4/en/select.html

______________________________________________________________________

## Ideia central

- A álgebra relacional é uma linguagem formal do modelo relacional.
- Ela é **procedural**: descreve quais operações aplicar para chegar ao resultado.
- Toda operação recebe uma ou mais relações e devolve outra relação.
- Isso permite compor consultas: o resultado de uma operação vira entrada de outra.
- É base teórica para SQL e para otimização de consultas.

## Operações fundamentais

- **Seleção** `σ_predicado(R)`: filtra **linhas/tuplas** de `R`.
- **Projeção** `Π_atributos(R)`: escolhe **colunas/atributos** de `R`.
- **Renomeação** `ρ_novo_nome(R)`: dá nome a uma relação ou renomeia atributos.
- **União** `R ∪ S`: junta tuplas de duas relações compatíveis.
- **Diferença** `R − S`: tuplas que estão em `R`, mas não em `S`.
- **Produto cartesiano** `R × S`: combina cada tupla de `R` com cada tupla de `S`.

## Seleção: filtro horizontal

Seleciona registros que satisfazem uma condição.

```text
σ_Tipo_aluno='D'(ALUNO)
```

Equivalente em SQL:

```sql
SELECT *
FROM ALUNO
WHERE Tipo_aluno = 'D';
```

Pense: seleção muda a quantidade de linhas, mas mantém as colunas.

## Projeção: filtro vertical

Seleciona atributos e descarta os demais.

```text
Π_Nome(ALUNO)
```

Equivalente em SQL:

```sql
SELECT Nome
FROM ALUNO;
```

Pense: projeção muda a quantidade de colunas. Como relações são conjuntos, duplicatas são eliminadas.

## Composição de operações

Consulta: nomes dos alunos do curso 101.

```text
Π_Nome(σ_Curso=101(ALUNO))
```

Equivalente em SQL:

```sql
SELECT Nome
FROM ALUNO
WHERE Curso = 101;
```

A seleção escolhe as tuplas; a projeção escolhe os atributos finais.

## União e diferença

União: nomes dos alunos dos cursos 101 ou 102.

```text
Π_Nome(σ_Curso=101(ALUNO)) ∪ Π_Nome(σ_Curso=102(ALUNO))
```

```sql
SELECT Nome FROM ALUNO WHERE Curso = 101
UNION
SELECT Nome FROM ALUNO WHERE Curso = 102;
```

Diferença: alunos que não fazem cursos com número menor que 115.

```text
ALUNO − σ_Curso<115(ALUNO)
```

```sql
SELECT *
FROM ALUNO
WHERE Numero_aluno NOT IN (
  SELECT Numero_aluno FROM ALUNO WHERE Curso < 115
);
```

União e diferença exigem relações compatíveis: mesmo número de atributos e domínios compatíveis.

## Produto cartesiano e junção

Produto cartesiano combina todas as tuplas possíveis.

```text
ALUNO × CURSO
```

```sql
SELECT ALUNO.Nome, CURSO.Nome
FROM ALUNO CROSS JOIN CURSO;
```

Se `ALUNO` tem 50 linhas e `CURSO` tem 20, o produto gera 1000 combinações.

Junção natural ou junção com condição evita combinações sem sentido:

```text
ALUNO ⋈ CURSO
```

```sql
SELECT *
FROM ALUNO
JOIN CURSO ON ALUNO.Curso = CURSO.Numero_curso;
```

## Operações derivadas

- **Interseção** `R ∩ S`: tuplas comuns entre `R` e `S`.
- Pode ser expressa como `R − (R − S)`.
- **Junção natural** combina produto cartesiano + seleção + projeção.
- **Atribuição** `temp ← expressão`: guarda resultado temporário.

Exemplo de interseção:

```text
σ_Numero_aluno<320(ALUNO) ∩ σ_Curso<115(ALUNO)
```

## Álgebra relacional x cálculo relacional x SQL

- Álgebra relacional: procedural, foca em **como** obter o resultado.
- Cálculo relacional: não procedural, foca em **o que** deseja.
- SQL é majoritariamente declarativa/não procedural: você declara o resultado desejado.
- O SGBD pode reordenar operações para otimizar a consulta.
- Índices podem mudar o plano de execução, reduzindo leituras.

## Pegadinhas de prova

- “Apenas atributos desejados” geralmente é **projeção**.
- “Linhas que satisfazem condição” geralmente é **seleção**.
- “Combinar duas relações quaisquer” pode ser **produto cartesiano**.
- `R − S` é a parte de `R` que não está em `S`.
- `R ∪ S`, `R ∩ S` e `R − S` exigem compatibilidade de união.
- Produto cartesiano não exige relações compatíveis, mas pode explodir o número de linhas.
- Junção sem condição pode virar produto cartesiano acidental.
