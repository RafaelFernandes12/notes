# Struct Padding and Alignment (C)

______________________________________________________________________

**Date:** 2026-03-03
**Tags:**

- [C](../tags/C.md)
- [Structs](../tags/Structs.md)
- [Memory](../tags/Memory.md)
- [Data_Alignment](../tags/Data_Alignment.md)
- [Security](../tags/Security.md)

**URL:**

- https://en.cppreference.com/w/c/language/object
- https://en.cppreference.com/w/c/language/struct
- https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html (packed)

______________________________________________________________________

## What are “padding bytes”?

When you write a `struct` in C, the compiler may insert **extra invisible bytes** between fields (or at the end) called **padding**.

Why? Many CPUs prefer (or require) that certain types start at specific addresses.

- On many platforms, an `int` likes to start at an address that is a multiple of **4**.
- The compiler adds padding so each field starts at a “nice” aligned address.

So:

- “sum of field sizes” can be smaller than `sizeof(struct ...)`.

---

## How to *see* padding (the idea)

Two tools:

- `sizeof(x)` → total bytes used
- `offsetof(type, field)` → where each field starts (in bytes)

Example helper:

```c
#include <stddef.h> // offsetof
#include <stdio.h>

struct Student {
  int id;
  char province[3];
  int age;
};

int main(void) {
  printf("sizeof(Student) = %zu\n", sizeof(struct Student));
  printf("offsetof(id) = %zu\n", offsetof(struct Student, id));
  printf("offsetof(province) = %zu\n", offsetof(struct Student, province));
  printf("offsetof(age) = %zu\n", offsetof(struct Student, age));
}
```

---

## Example 1: why your `Student` often becomes 12 bytes (not 11)

```c
struct Student {
  int id;          // 4 bytes
  char province[3]; // 3 bytes
  int age;         // 4 bytes
};
```

Typical layout (byte offsets):

- `id` at offset 0..3 (4 bytes)
- `province` at offset 4..6 (3 bytes)
- offset 7 is **padding** (1 byte) so that...
- `age` can start at offset 8..11 (4 bytes)

Total: 4 + 3 + 1(padding) + 4 = **12** bytes.

---

## Example 2: field order can change the amount of padding

### “Bad” order (more padding)

```c
struct A {
  char a;  // 1
  int  b;  // 4
  char c;  // 1
};
```

Typical layout:

- `a` at 0
- padding at 1..3 (3 bytes) so `b` can start at 4
- `b` at 4..7
- `c` at 8
- padding at 9..11 (3 bytes) so the whole struct size is a multiple of 4

Total: 1 + 3 + 4 + 1 + 3 = **12** bytes.

### “Better” order (less padding)

```c
struct B {
  int  b;  // 4
  char a;  // 1
  char c;  // 1
};
```

Typical layout:

- `b` at 0..3
- `a` at 4
- `c` at 5
- padding at 6..7 (2 bytes) so struct size is multiple of 4

Total: 4 + 1 + 1 + 2 = **8** bytes.

Rule of thumb: **put bigger-aligned fields first** (e.g., `double`, pointers, `int`) and smaller ones later.

---

## Example 3: removing padding with `packed` (use carefully)

Some compilers let you request “no padding”:

```c
struct __attribute__((packed)) A_packed {
  char a;
  int  b;
  char c;
};
```

This can make `sizeof(struct A_packed)` closer to the sum of fields (often **6** here).

But packed structs can be dangerous/slow:

- `int b` may be **unaligned** in memory
- some CPUs crash on unaligned access; others do extra work (slower)
- it can reduce portability

Packed structs are common for **network/file formats** (exact byte layout matters), but they need extra care when reading/writing fields.

---

## Why this matters (bugs + security)

- If you assume `sizeof(struct)` equals the sum of fields, you may allocate too little, copy too much, or read wrong bytes.
- Padding bytes may contain **uninitialized data** if you copy the whole struct byte-for-byte (e.g., writing to disk or sending over the network).
