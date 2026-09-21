# Questão 19 — Eliminação de Gauss-Jordan

Sistema homogêneo (2 equações, 4 incógnitas):

$$
\begin{cases}
3x_1 + x_2 + x_3 + x_4 = 0 \\
5x_1 - x_2 + x_3 - x_4 = 0
\end{cases}
$$

## Resolução

Matriz aumentada:

```
[ 3   1   1   1 | 0]
[ 5  -1   1  -1 | 0]
```

**Passo 1:** L1 ← L1 − L2 (para conseguir um pivô sem frações)

```
[-2   2   0   2 | 0]
[ 5  -1   1  -1 | 0]
```

**Passo 2:** L1 ← L1 / (−2)

```
[ 1  -1   0  -1 | 0]
[ 5  -1   1  -1 | 0]
```

**Passo 3:** L2 ← L2 − 5·L1

```
[ 1  -1   0  -1 | 0]
[ 0   4   1   4 | 0]
```

Como só há 2 pivôs (x₁ e x₂) para 4 incógnitas, o sistema é **possível e indeterminado**, com **2 variáveis livres** (x₃ e x₄).

Da linha 2:
$$4x_2 + x_3 + 4x_4 = 0 \implies x_2 = \frac{-x_3 - 4x_4}{4}$$

Da linha 1:
$$x_1 - x_2 - x_4 = 0 \implies x_1 = x_2 + x_4$$

Fazendo **x₁ = s** e **x₄ = t** (s, t ∈ ℝ, livres):

- Da soma das duas equações originais: 8x₁ + 2x₃ = 0 → **x₃ = −4s**
- Substituindo na 1ª equação: 3s + x₂ − 4s + t = 0 → **x₂ = s − t**

## Solução geral

$$(x_1, x_2, x_3, x_4) = (s,\; s-t,\; -4s,\; t), \quad s,t \in \mathbb{R}$$

## Verificação

- Eq. 1: 3s + (s−t) + (−4s) + t = 0 ✓
- Eq. 2: 5s − (s−t) + (−4s) − t = 0 ✓

Sistema homogêneo com infinitas soluções (inclusive a trivial, quando s = t = 0).
