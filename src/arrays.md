# arrays

______________________________________________________________________

**Date:** 2026-08-27
**Tags:**[Algoritmos.md](tags/Algoritmos.md)
**URL:**

______________________________________________________________________

## O que é um array

### Array de verdade (o array em C)

Array é um espaço de memória contínuo que pode ter vários elementos, eu posso interpretar esse espaço de diferentes formas.
Como um conjunto de elementos de 2 bits, 4 bits, 8 bits, caso eu armazene 16 bits.
\[1111 0000 1111 0000\]
\[11 11 00 00 11 11 00 00\]
\[11110000 11110000\]

Exemplo, em rust, para criar um array.
```rust
let my_array: [i32: 4] = [1,2,3,4]
// i é o tipo, 32 são os bits, e 4 é o tamanho,
// O 1° elemento começara em 32*0, o 2° em 32*1, o 7° em 32*6
// Acessar e alterar um elemento desse array é O(1)
```

