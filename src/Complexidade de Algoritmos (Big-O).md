# Complexidade de Algoritmos (Big-O)

______________________________________________________________________

**Date:** 2026-03-07
**Tags:** [Algoritmos](../tags/Algoritmos.md), [Math](../tags/Math.md), [Uf.md](tags/Uf.md), [Edb2.md](tags/Edb2.md)
**Subject:** [Edb2.md](tags/Edb2.md)
**Assets:** https://drive.google.com/file/d/10g0wq114SJoKAea6DZ2hYEAmZ90ibV5N/view
**URL:**


______________________________________________________________________

## O que é (ideia)

**Complexidade** descreve como o consumo de recursos de um algoritmo cresce com o tamanho da entrada `n`.

## Como usar (o “como”): ideia de contar passos e simplificar

Um jeito clássico é escolher uma **operação fundamental** (ex.: uma comparação) e contar quantas vezes ela ocorre.

Depois, para comparar algoritmos, normalmente simplificamos:

- Ignoramos constantes: `2n + 4` vira “linear”.
- Ignoramos termos de menor ordem: `an² + bn + c` vira `n²`.

Isso leva à ordem de crescimento (ex.: `O(n)`, `O(n²)`).


## Tempos comuns

### O(1)

Independente do tamanho do input, a complexidade de tempo permanecerá a mesma.
Se o input é de 10, 100 ou 100.000.000, o tempo de execução será igual.
Isso independente do tempo de execução, se o tempo de execução for 10_000 anos com o input de 1, e com o input de 1.000.000, continuará sendo O(1)
A mesma lógica se aplica a complexidade espacial.
* Exemplo: Achar o primeiro elemento do array

### logn
A cada iteração, o intervalo cai pela metade ⇒ `O(log n)`.
Para O(log n), enquanto o input aumenta exponencialmente, o tempo de execução aumenta linearmente, ou seja.

log2 (10) -> 3.32
log2 (20) -> 4.32
log2 (40) -> 5.32
* Exemplo: binary search

### O(N)

Ele escala exatamente na mesma medida que o input aumenta, 10 input, 10 de execução, 1.000.000 input, 1.000.000 de execução.
* Exemplo: busca simples

### O (N Logn)

Explicar matematicamente isso é muito complexo, reze pra que ninguem pergunte
Basicamente, no caso do merge sort, ele percorre o array em O(N), e ele divide o array em O(log n)

Exemplo: Sorting(quicksort, mergesort), divide and conquer

### O (N^2)

Quando percorre o array duas vezes.
for dentro de for, é O(n^2).


## Tabela mental (ordem típica)

Do “cresce pouco” ao “explode rápido”:

`O(1)` < `O(log n)` < `O(n)` < `O(n log n)` < `O(n²)` < `O(2^n)` < `O(n!)`
