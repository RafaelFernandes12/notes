# Notacoes Assintoticas (O, Omega, Theta, o e omega)

______________________________________________________________________

**Date:** 2026-03-16
**Tags:** [Algoritmos](../tags/Algoritmos.md), [Math](../tags/Math.md), [ComplexidadeAssintotica](../tags/ComplexidadeAssintotica.md)
**URL:**

- [Slides: Complexidade assintótica — Jorge E. S. Souza](file:///home/rafael/Downloads/3-Complexidade_assinttica-1.pdf)
- [Big-O notation (Wikipedia)](https://en.wikipedia.org/wiki/Big_O_notation)
- [Asymptotic analysis (Wikipedia)](https://en.wikipedia.org/wiki/Asymptotic_analysis)

______________________________________________________________________

## O que é (conceito)

**Complexidade assintótica** é a análise de como o custo de um algoritmo cresce quando o tamanho da entrada `n` cresce muito.

Em vez de focar no tempo exato em milissegundos (que depende de máquina, linguagem e compilador), focamos na **ordem de crescimento**.

Ideia central dos slides: para entradas grandes, importa mais o "formato" do crescimento (`n`, `n²`, `log n`, etc.) do que constantes e detalhes pequenos.

## Para que serve

- Comparar algoritmos diferentes para o mesmo problema.
- Escolher soluções que escalam melhor com dados grandes.
- Entender quando uma solução vai "explodir" de custo.

## Notações assintóticas (resumo)

### 1) Big-O — limite superior ("teto" assintótico)

`f(n) = O(g(n))` se existem constantes `c > 0` e `n0` tais que:

`0 <= f(n) <= c * g(n)`, para todo `n >= n0`.

Leitura prática: **f cresce no máximo como g** (ignorando constantes e detalhes de baixo impacto).

Quando usar:

- quando você quer um **limite superior de custo**;
- muito comum para descrever **pior caso** de tempo.

Exemplos:

- `3n + 20 ∈ O(n)` (é linear);
- `n log n ∈ O(n²)` (verdadeiro, embora menos preciso que `Θ(n log n)`);
- busca linear no pior caso: `O(n)`.

---

### 2) Big-Omega — limite inferior ("piso" assintótico)

`f(n) = Ω(g(n))` se existem `c > 0` e `n0` tais que:

`0 <= c * g(n) <= f(n)`, para todo `n >= n0`.

Leitura prática: **f cresce no mínimo como g**.

Quando usar:

- para mostrar que o custo não pode ser menor que certa ordem;
- muito usado em provas de limite inferior e em melhor caso.

Exemplos:

- `3n + 20 ∈ Ω(n)`;
- `n² ∈ Ω(n log n)`;
- busca linear no melhor caso: `Ω(1)` (achou no primeiro elemento).

---

### 3) Big-Theta — limite firme (ordem "exata" assintótica)

`f(n) = Θ(g(n))` se `f(n) = O(g(n))` **e** `f(n) = Ω(g(n))`.

Equivale a existir `c1, c2 > 0` e `n0` tais que:

`0 <= c1*g(n) <= f(n) <= c2*g(n)`, para todo `n >= n0`.

Leitura prática: **f cresce na mesma ordem de g**.

Quando usar:

- quando você quer ser mais preciso que Big-O;
- quando conhece limite superior e inferior da mesma classe.

Exemplos:

- `5n² + 7n + 1 ∈ Θ(n²)`;
- merge sort: `Θ(n log n)`;
- laço duplo completo: `Θ(n²)`.

---

### 4) little-o — limite superior estrito (cresce estritamente menos)

`f(n) = o(g(n))` quando `f` cresce estritamente mais devagar que `g`.

Caracterização por limite:

`lim (f(n)/g(n)) = 0` quando `n -> infinito`.

Leitura prática: `f` fica "cada vez menor" em relação a `g`.

Exemplos:

- `n ∈ o(n²)` (`n/n² = 1/n -> 0`);
- `log n ∈ o(n)`;
- `n ∉ o(n)` (razão = 1).

---

### 5) little-omega — limite inferior estrito (cresce estritamente mais)

`f(n) = ω(g(n))` quando `f` cresce estritamente mais rápido que `g`.

Caracterização por limite:

`lim (f(n)/g(n)) = infinito` quando `n -> infinito`.

Leitura prática: `f` domina `g` de forma estrita.

Exemplos:

- `n² ∈ ω(n)`;
- `n log n ∈ ω(n)`;
- `n ∉ ω(n)`.

---

### Comparação rápida (intuição de ordem)

- `f ∈ O(g)`  ~  "`f` é <= `g` assintoticamente"
- `f ∈ Ω(g)`  ~  "`f` é >= `g` assintoticamente"
- `f ∈ Θ(g)`  ~  "`f` é = `g` em ordem de grandeza"
- `f ∈ o(g)`  ~  "`f` é < `g` assintoticamente"
- `f ∈ ω(g)`  ~  "`f` é > `g` assintoticamente"

## Como usar (passo a passo)

1. Defina a função de custo `T(n)` que você quer analisar.
2. Escolha uma função referência `g(n)` (por exemplo `n`, `n log n`, `n²`).
3. Tente provar as desigualdades com constantes (`c`, `c1`, `c2`) e `n0`.
4. Declare claramente se é limite superior (`O`), inferior (`Ω`) ou firme (`Θ`).

## Exemplos matemáticos (do tipo mostrado no PDF)

### Exemplo 1 — `n + 34 ∈ O(n)`

Queremos `n + 34 <= c*n` para `n >= n0`.

Escolhendo `c = 2`, basta ter `n + 34 <= 2n` -> `34 <= n`.

Então `n0 = 34` funciona.

Logo, `n + 34 ∈ O(n)`.

### Exemplo 2 — `n² ∉ O(n)`

Se fosse verdade, existiriam `c` e `n0` com `n² <= c*n` para todo `n >= n0`.

Dividindo por `n > 0`: `n <= c`.

Isso é impossível para todo `n` grande (porque `n` cresce sem limite e `c` é constante).

Logo, `n²` **não** está em `O(n)`.

### Exemplo 3 — `(n²/2 - 3n) ∈ Θ(n²)`

Para `Θ(n²)`, precisamos de:

`c1*n² <= n²/2 - 3n <= c2*n²`, para `n >= n0`.

Nos slides, uma escolha válida é `c1 = 1/14`, `c2 = 1/2`, `n0 = 7`.

Então a função fica limitada por cima e por baixo por múltiplos de `n²`.

Conclusão: `(n²/2 - 3n) ∈ Θ(n²)`.

## Exemplos de código (foco prático)

### Exemplo 1 — Busca linear (Python)

```python
def busca_linear(arr, alvo):
    for i, valor in enumerate(arr):
        if valor == alvo:
            return i
    return -1
```

- Melhor caso: `Ω(1)` (achou no primeiro elemento).
- Pior caso: `O(n)` (varre tudo).
- Caso típico de crescimento linear.

### Exemplo 2 — Busca binária (Python)

```python
def busca_binaria(arr_ordenado, alvo):
    ini, fim = 0, len(arr_ordenado) - 1
    while ini <= fim:
        meio = (ini + fim) // 2
        if arr_ordenado[meio] == alvo:
            return meio
        if arr_ordenado[meio] < alvo:
            ini = meio + 1
        else:
            fim = meio - 1
    return -1
```

- A cada passo, o espaço de busca cai pela metade.
- Complexidade de tempo: `O(log n)`.
- Exige array ordenado.

### Exemplo 3 — Fibonacci recursivo vs iterativo

```python
def fib_rec(n):
    if n <= 1:
        return n
    return fib_rec(n - 1) + fib_rec(n - 2)


def fib_it(n):
    if n <= 1:
        return n
    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b
```

- `fib_rec`: tempo exponencial (`O(2^n)`, aproximação comum) por recomputar subproblemas.
- `fib_it`: tempo linear (`O(n)`) e espaço `O(1)`.
- Mesmo problema, ordens de crescimento muito diferentes.

## Regras operacionais úteis (lembradas nos slides)

- `c * O(f(n)) = O(f(n))`, para `c` constante positiva.
- `O(f(n)) + O(g(n)) = O(max(f(n), g(n)))`.
- `O(f(n)) * O(g(n)) = O(f(n) * g(n))`.
- `f(n) = Θ(g(n))` sse `f(n) = O(g(n))` e `f(n) = Ω(g(n))`.

## Dicas rápidas para prova e interpretação

- Use `O` quando quiser garantir teto (pior caso / limite superior).
- Use `Θ` quando souber a ordem exata de crescimento.
- Se `lim f(n)/g(n) = 0`, normalmente pense em `o(g(n))`.
- Se `lim f(n)/g(n) = c > 0`, normalmente pense em `Θ(g(n))`.
- Se `lim f(n)/g(n) = infinito`, normalmente pense em `ω(g(n))`.

## Aprofundamento das notações (mais formal e mais intuitivo)

### Convenções importantes antes de provar qualquer coisa

Nas definições, quase sempre assumimos que as funções são **assintoticamente não negativas** (eventualmente `>= 0`), para evitar ambiguidades.

Também é essencial entender o papel de `n0`:

- A desigualdade **não precisa valer para todo n pequeno**.
- Ela precisa valer a partir de algum ponto (`n >= n0`).

Esse detalhe explica por que expressões como `n + 34` podem ser tratadas como lineares: o `+34` para de importar quando `n` é grande.

---

### Big-O (O) com quantificadores

Definição formal:

`f(n) ∈ O(g(n))  <=>  (∃ c > 0)(∃ n0)(∀ n >= n0): 0 <= f(n) <= c*g(n)`

Interpretação:

- `g(n)` é um **teto assintótico** para `f(n)` (até constante multiplicativa).
- Em algoritmos, costuma representar **garantia de pior caso**.

#### Exemplo detalhado 1 — `3n + 20 ∈ O(n)`

Queremos mostrar `3n + 20 <= c*n`.

Uma escolha simples:

- se `n >= 20`, então `20 <= n`;
- logo `3n + 20 <= 3n + n = 4n`.

Portanto, `c = 4` e `n0 = 20` funcionam, e a afirmação é verdadeira.

> Observação útil: as constantes não são únicas. Poderíamos escolher `c = 10`, por exemplo, e ajustar `n0`.

#### Exemplo detalhado 2 (negação) — `n² ∉ O(n)`

Suponha por absurdo que `n² ∈ O(n)`.

Então existiriam `c > 0` e `n0` tais que `n² <= c*n` para `n >= n0`.

Para `n > 0`, dividimos por `n`:

`n <= c`.

Mas `n` cresce sem limite e `c` é constante. Contradição.

Logo, `n² ∉ O(n)`.

---

### Big-Omega (Ω) com quantificadores

Definição formal:

`f(n) ∈ Ω(g(n))  <=>  (∃ c > 0)(∃ n0)(∀ n >= n0): 0 <= c*g(n) <= f(n)`

Interpretação:

- `g(n)` é um **piso assintótico** para `f(n)`.
- Em algoritmos, aparece como limite inferior (ex.: ninguém faz melhor que isso em determinado modelo).

#### Exemplo detalhado 1 — `3n + 20 ∈ Ω(n)`

Queremos `c*n <= 3n + 20`.

Tomando `c = 1`, temos `n <= 3n + 20` para todo `n >= 0`.

Logo, vale com `n0 = 0`.

#### Exemplo detalhado 2 (negação) — `n ∉ Ω(n²)`

Se fosse verdade, existiriam `c > 0` e `n0` tais que:

`c*n² <= n`, para `n >= n0`.

Dividindo por `n > 0`: `c*n <= 1`, ou `n <= 1/c`.

De novo impossível para todo `n` grande.

Logo, `n ∉ Ω(n²)`.

---

### Big-Theta (Θ) com quantificadores

Definição formal:

`f(n) ∈ Θ(g(n))  <=>  (∃ c1, c2 > 0)(∃ n0)(∀ n >= n0): 0 <= c1*g(n) <= f(n) <= c2*g(n)`

Interpretação:

- `f` fica "espremida" entre dois múltiplos de `g`.
- É a forma mais informativa para falar de ordem de crescimento exata.

#### Exemplo detalhado 1 — `7n log n + 13n ∈ Θ(n log n)`

Para `n >= 2`, temos `log n >= 1`, então `13n <= 13n log n`.

Logo:

`7n log n + 13n <= 7n log n + 13n log n = 20n log n`.

Também é óbvio que:

`7n log n <= 7n log n + 13n`.

Então, para `n >= 2`:

`7n log n <= 7n log n + 13n <= 20n log n`.

Basta escolher `c1 = 7`, `c2 = 20`, `n0 = 2`.

Conclusão: está em `Θ(n log n)`.

#### Exemplo detalhado 2 — do slide: `(n²/2 - 3n) ∈ Θ(n²)`

O PDF mostra uma escolha possível:

- `c1 = 1/14`
- `c2 = 1/2`
- `n0 = 7`

Assim, para todo `n >= 7`, temos:

`(1/14)n² <= (1/2)n² - 3n <= (1/2)n²`.

Logo, pertence a `Θ(n²)`.

---

### little-o (o): crescimento estritamente menor

Uma definição comum é:

`f(n) ∈ o(g(n))  <=>  (∀ c > 0)(∃ n0)(∀ n >= n0): 0 <= f(n) < c*g(n)`

Caracterização por limite (muito usada):

`f(n) ∈ o(g(n))  <=>  lim_{n->∞} f(n)/g(n) = 0`

#### Exemplos

- `n ∈ o(n²)` porque `n/n² = 1/n -> 0`.
- `log n ∈ o(n)` porque `log n / n -> 0`.
- `n ∉ o(n)` porque `n/n = 1` (não tende a 0).

---

### little-omega (ω): crescimento estritamente maior

Definição equivalente via limite:

`f(n) ∈ ω(g(n))  <=>  lim_{n->∞} f(n)/g(n) = ∞`

Forma com quantificadores:

`f(n) ∈ ω(g(n))  <=>  (∀ c > 0)(∃ n0)(∀ n >= n0): 0 <= c*g(n) < f(n)`

#### Exemplos

- `n² ∈ ω(n)` porque `n²/n = n -> ∞`.
- `n log n ∈ ω(n)` porque `(n log n)/n = log n -> ∞`.
- `n ∉ ω(n)` porque `n/n = 1`.

---

### Relações rápidas entre as notações

- `Θ(g) = O(g) ∩ Ω(g)`
- `o(g) ⊂ O(g)`
- `ω(g) ⊂ Ω(g)`
- Se `f ∈ o(g)`, então `f` não está em `Ω(g)`.
- Se `f ∈ ω(g)`, então `f` não está em `O(g)`.

## Propriedades úteis para prova (visão de algebra de ordens)

- Reflexividade:
  - `f ∈ O(f)`, `f ∈ Ω(f)`, `f ∈ Θ(f)`.
- Simetria:
  - `f ∈ Θ(g) <=> g ∈ Θ(f)`.
- Transposição:
  - `f ∈ O(g) <=> g ∈ Ω(f)`.
  - `f ∈ o(g) <=> g ∈ ω(f)`.
- Transitividade:
  - se `f ∈ O(g)` e `g ∈ O(h)`, então `f ∈ O(h)`.
  - análogo para `Ω`, `Θ`, `o`, `ω`.

## Método prático de análise de algoritmos (checklist)

1. Defina o tamanho da entrada (`n`).
2. Escolha operação básica (comparação, acesso, soma, etc.).
3. Conte quantas vezes essa operação acontece.
4. Escreva uma função de custo aproximada (`T(n)`).
5. Simplifique para termo dominante (ordem assintótica).
6. Declare o caso analisado: melhor, médio, pior.

## Exemplos extras de código com análise detalhada

### Exemplo A — Dois laços independentes (não aninhados)

```python
def imprimir_duas_vezes(arr):
    for x in arr:        # n iterações
        _ = x

    for x in arr:        # n iterações
        _ = x
```

Contagem: `n + n = 2n` operações básicas.

- `T(n) = 2n`
- `T(n) ∈ Θ(n)`

> Erro comum: achar que é `n²` só porque existem dois `for`.
> Só vira `n²` quando um laço está **dentro** do outro.

### Exemplo B — Laços aninhados completos

```python
def pares_completos(n):
    total = 0
    for i in range(n):
        for j in range(n):
            total += 1
    return total
```

Contagem: para cada `i` (n valores), `j` roda `n` vezes.

`T(n) = n * n = n²`.

Logo: `Θ(n²)`.

### Exemplo C — Laço triangular

```python
def pares_triangulares(n):
    total = 0
    for i in range(n):
        for j in range(i):
            total += 1
    return total
```

Contagem total:

`0 + 1 + 2 + ... + (n-1) = n(n-1)/2`.

Então:

- `T(n) = (n² - n)/2`
- `T(n) ∈ Θ(n²)`

Mesmo sem ser quadrado perfeito (`n*n`), continua quadrático.

### Exemplo D — Busca binária com limite explícito de iterações

```python
def busca_binaria(arr_ordenado, alvo):
    ini, fim = 0, len(arr_ordenado) - 1
    while ini <= fim:
        meio = (ini + fim) // 2
        if arr_ordenado[meio] == alvo:
            return meio
        if arr_ordenado[meio] < alvo:
            ini = meio + 1
        else:
            fim = meio - 1
    return -1
```

Se após `k` iterações restam no máximo `n / 2^k` elementos, paramos quando isso cai para `1`:

`n / 2^k <= 1  =>  2^k >= n  =>  k >= log2(n)`.

Logo, número de iterações é `Θ(log n)`.

### Exemplo E — Merge Sort (dividir e conquistar)

```python
def merge_sort(arr):
    if len(arr) <= 1:
        return arr

    meio = len(arr) // 2
    esquerda = merge_sort(arr[:meio])
    direita = merge_sort(arr[meio:])
    return merge(esquerda, direita)
```

Recorrência clássica:

`T(n) = 2T(n/2) + Θ(n)`

- `2T(n/2)`: dois subproblemas de metade do tamanho.
- `Θ(n)`: custo para mesclar.

Resultado conhecido: `T(n) = Θ(n log n)`.

### Exemplo F — Fibonacci: recursão ingênua, memoização e iterativo

```python
from functools import lru_cache

def fib_rec(n):
    if n <= 1:
        return n
    return fib_rec(n - 1) + fib_rec(n - 2)


@lru_cache(None)
def fib_memo(n):
    if n <= 1:
        return n
    return fib_memo(n - 1) + fib_memo(n - 2)


def fib_it(n):
    if n <= 1:
        return n
    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b
```

Comparação:

- `fib_rec`: exponencial (`~O(2^n)`), porque repete subproblemas.
- `fib_memo`: `Θ(n)` tempo e `Θ(n)` memória de cache.
- `fib_it`: `Θ(n)` tempo e `Θ(1)` memória auxiliar.

Mesmo problema, ordens e custos práticos muito diferentes.

## Erros comuns ao usar notações assintóticas

1. **Confundir Big-O com "custo exato"**
   - `O(n)` não quer dizer "é exatamente n".
2. **Esquecer o "para n suficientemente grande"**
   - pequenos `n` podem não obedecer a desigualdade escolhida.
3. **Misturar caso médio e pior caso sem avisar**
   - sempre informe qual caso está sendo analisado.
4. **Ignorar pré-condições do algoritmo**
   - busca binária é `O(log n)` só com entrada ordenada.

## Mini tabela mental (crescimento)

`O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2^n) < O(n!)`

Quando `n` cresce, diferenças entre essas classes ficam enormes.
