# Complexidade de Algoritmos (Big-O)

______________________________________________________________________

**Date:** 2026-03-07
**Tags:** [Algoritmos](../tags/Algoritmos.md), [Math](../tags/Math.md), [Uf.md](tags/Uf.md), [Edb2.md](tags/Edb2.md)
**Subject:** [Edb2.md](tags/Edb2.md)
**Assets:** https://drive.google.com/file/d/10g0wq114SJoKAea6DZ2hYEAmZ90ibV5N/view
**URL:**

- https://en.wikipedia.org/wiki/Big_O_notation
- https://cp-algorithms.com/complexity/complexity.html
- https://mitpress.mit.edu/9780262046305/introduction-to-algorithms/ (CLRS)

______________________________________________________________________

## O que é (ideia)

**Complexidade** descreve como o consumo de recursos de um algoritmo cresce com o tamanho da entrada `n`.

Na prática, a gente quase sempre discute **complexidade de tempo** (quantos “passos”/operações o algoritmo executa) e usa uma **cota assintótica** (ex.: `O(n)`, `O(n²)`, `O(log n)`), focando no que domina quando `n` fica grande.

Dois pontos importantes (bem no espírito do PDF):

1. **Muitos algoritmos resolvem o mesmo problema**, mas podem ser inviáveis para entradas grandes (crescimento explode).
1. A análise pode ser feita:
   - **Experimental**: medir tempo real rodando o código (depende de máquina/linguagem/otimizações).
   - **Analítica**: contar passos e obter uma função `t(n)` que cresce com `n`.

## Como usar (o “como”): ideia de contar passos e simplificar

Um jeito clássico é escolher uma **operação fundamental** (ex.: uma comparação) e contar quantas vezes ela ocorre.

Depois, para comparar algoritmos, normalmente simplificamos:

- Ignoramos constantes: `2n + 4` vira “linear”.
- Ignoramos termos de menor ordem: `an² + bn + c` vira `n²`.

Isso leva à ordem de crescimento (ex.: `O(n)`, `O(n²)`).

## Casos: melhor, pior e médio

O mesmo algoritmo pode ter comportamentos diferentes dependendo da entrada.

- **Melhor caso**: menor número de passos para entradas de tamanho `n`.
- **Pior caso**: maior número de passos (muito usado porque dá garantia).
- **Caso médio**: valor esperado, depende de hipóteses de probabilidade sobre as entradas.

### Exemplo 1 — Busca sequencial (linear search)

Problema: achar `x` em um array `A`.

```js
function buscaSequencial(A, x) {
  for (let i = 0; i < A.length; i++) {
    if (A[i] === x) return i;
  }
  return -1;
}
```

Análise (pela ideia do PDF: contar comparações `A[i] === x`):

- **Melhor caso**: `x` está na primeira posição ⇒ 1 comparação ⇒ `O(1)`.
- **Pior caso**: `x` está na última posição (ou não está) ⇒ ~`n` comparações ⇒ `O(n)`.
- **Caso médio** (hipótese comum: `x` está no array e posições equiprováveis):
  - comparações esperadas ≈ `(n + 1) / 2` ⇒ ainda é `Θ(n)` (cresce linearmente).

## Padrões rápidos para identificar complexidade

Esses “atalhos” evitam contar linha a linha em muitos casos:

### 1) Sem laços/recursão ⇒ constante

```js
function isPar(x) {
  return x % 2 === 0; // O(1)
}
```

### 2) Um laço que vai até `n` ⇒ linear

```js
function soma(A) {
  let s = 0;
  for (let i = 0; i < A.length; i++) s += A[i];
  return s; // O(n)
}
```

### 3) Dois laços “cheios” até `n` ⇒ quadrática

Ex.: gerar todos os pares `(i, j)`.

```js
function todosOsPares(n) {
  const pairs = [];
  for (let i = 0; i < n; i++) {
    for (let j = 0; j < n; j++) {
      pairs.push([i, j]);
    }
  }
  return pairs; // O(n^2)
}
```

### 4) Laço interno depende do externo (triangular) ⇒ ainda quadrática

Ex.: `j < i` dá aproximadamente `n(n-1)/2` iterações.

```js
function paresTriangulares(n) {
  let count = 0;
  for (let i = 0; i < n; i++) {
    for (let j = 0; j < i; j++) {
      count++;
    }
  }
  return count; // O(n^2)
}
```

### 5) “Dividir por 2 a cada passo” ⇒ logarítmica

Exemplo típico: **busca binária** (array ordenado).

```js
function buscaBinaria(A, x) {
  let lo = 0, hi = A.length - 1;
  while (lo <= hi) {
    const mid = lo + Math.floor((hi - lo) / 2);
    if (A[mid] === x) return mid;
    if (A[mid] < x) lo = mid + 1;
    else hi = mid - 1;
  }
  return -1;
}
```

- A cada iteração, o intervalo cai pela metade ⇒ `O(log n)`.

## Tabela mental (ordem típica)

Do “cresce pouco” ao “explode rápido”:

`O(1)` < `O(log n)` < `O(n)` < `O(n log n)` < `O(n²)` < `O(2^n)` < `O(n!)`

## Dicas práticas

- Quando comparar algoritmos, pergunte: **qual é o termo dominante?**
  - `2n + 4` e `100n` são ambos “lineares” ⇒ `O(n)`.
  - `n² + 10n + 50` é “quadrático” ⇒ `O(n²)`.
- Sempre declare **qual caso** você está analisando (melhor/pior/médio) e quais hipóteses (ex.: array ordenado, distribuição de entradas).

## Referência usada

- Slides: “Complexidade de Algoritmos”, Jorge E. S. Souza (PDF fornecido).
