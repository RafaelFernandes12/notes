# C Strings (Null Terminator) and `strncpy`

______________________________________________________________________

**Date:** 2026-03-03
**Tags:**

- [C](../tags/C.md)
- [Strings](../tags/Strings.md)
- [C_Standard_Library](../tags/C_Standard_Library.md)
- [Security](../tags/Security.md)

**URL:**

- https://en.cppreference.com/w/c/string/byte/strncpy
- https://man7.org/linux/man-pages/man3/strncpy.3.html

______________________________________________________________________

## The idea: what is a “C string”?

In C, a “string” is not a special type. It’s just an array of bytes (`char[]`) that ends with a **stop marker** byte called the **null terminator**: `\0`.

So the text:

- `"ON"`

is stored in memory as 3 bytes:

| byte 0 | byte 1 | byte 2 |
|---|---|---|
| `'O'` | `'N'` | `'\0'` |

That last `\0` is how functions like `printf("%s", ...)`, `strlen`, `strcpy`, etc. know when to stop reading.

If `\0` is missing, C will keep reading *past the end* of your array until it *happens* to find a `\0` somewhere else in memory (this can print garbage or crash).

---

## What `strncpy` means

`strncpy(dest, src, n)` means:

> Copy **up to `n` bytes** from `src` into `dest`.

The important behavior depends on the length of `src`:

### Case A: `src` is shorter than `n`

`strncpy` copies the characters from `src` **and** the `\0`, then fills (“pads”) the rest of the `n` bytes with more `\0`.

### Case B: `src` is `n` bytes or longer

`strncpy` copies **exactly `n` bytes** and **does not guarantee** `dest` ends with `\0`.

This is the part that surprises many beginners.

---

## Example 1 (your struct): copying a 2-letter province safely

From your note:

```c
struct Student {
  int id;
  char province[3]; // 2 letters + '\0'
  int age;
};

strncpy(student1.province, "ON\0", sizeof(student1.province));
```

`province` has 3 bytes, so `n = 3`. The source `"ON\0"` contains 3 bytes: `O`, `N`, `\0`.

Result in `province`:

- `'O'`, `'N'`, `'\0'`

So it becomes a valid C string.

Note: writing `"ON"` would also work here, because C string literals already include the terminator.

```c
strncpy(student1.province, "ON", sizeof(student1.province));
```

---

## Example 2 (common bug): copied text but forgot the stop marker

```c
char province[3];
strncpy(province, "ONTARIO", sizeof(province)); // n = 3

printf("%s\n", province); // DANGER
```

What gets copied is the first 3 bytes: `O`, `N`, `T`.

There is **no room** left for `\0`, so `province` is **not** a valid C string.

Then `printf("%s", province)` will keep reading bytes *after* `province` until it finds a `\0` somewhere else. That’s why this is dangerous.

---

## Example 3 (practical safety pattern): force a `\0`

If you want to use `strncpy` with a buffer and then treat the result as a string, a common pattern is:

```c
strncpy(dest, src, sizeof(dest));
dest[sizeof(dest) - 1] = '\0';
```

This guarantees the last byte is the stop marker even if `src` was too long.

---

## When would I use `strncpy`?

Historically, people used `strncpy` to copy into fixed-size arrays. But because it can *not* null-terminate and because it pads with `\0` (which can be wasted work), many codebases prefer alternatives depending on the goal:

- **I need a safely terminated string:** `snprintf(dest, sizeof dest, "%s", src)`
- **I need exactly N bytes (not “a string”):** `memcpy(dest, src, n)` (only if you know `src` has at least `n` bytes)
- **I’m on BSD/macOS and available:** `strlcpy` (not standard C, not on all platforms)

The key is: decide if you’re copying a *string* (must end with `\0`) or just *bytes*.

---

## Quick mental checklist

- Do I have space for the `\0` terminator? (`buffer_size >= text_length + 1`)
- After copying, am I **guaranteeing** there is a `\0` before I treat it as a string?
- Am I printing with `%s` or calling `strlen`? If yes, `\0` must be present.
