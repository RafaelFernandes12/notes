# GDB Basics (Debugging C Programs)

______________________________________________________________________

**Date:** 2026-03-03
**Tags:**

- [GDB](../tags/GDB.md)
- [Debugging](../tags/Debugging.md)
- [C](../tags/C.md)
- [Linux](../tags/Linux.md)
- [Forense](../tags/Forense.md)

**URL:**

- https://sourceware.org/gdb/current/onlinedocs/gdb/
- https://man7.org/linux/man-pages/man1/gdb.1.html
- https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html

______________________________________________________________________

## What is GDB?

**GDB** (GNU Debugger) lets you run a program *under control*, so you can:

- stop at specific lines (breakpoints)
- step line-by-line
- inspect variables and memory
- understand crashes (segfaults)

______________________________________________________________________

## 1) Compile your C program for debugging

Use:

```bash
gcc -g -O0 -Wall -Wextra program.c -o program
```

What these flags do:

- `-g`: includes **debug symbols** (variable names, source lines) so GDB can show them.
- `-O0`: disables optimizations (makes stepping and variables more predictable).
- `-Wall -Wextra`: enables useful **warnings**.

If you forget `-g`, GDB may say things like “No debugging symbols found”.

______________________________________________________________________

## 2) Start GDB correctly (load the executable)

Prefer:

```bash
gdb ./program
```

Inside GDB, you can also load it with:

```gdb
file ./program
```

If you see:

- `No symbol table is loaded. Use the "file" command.`

it usually means you didn’t load the executable (or compiled without `-g`).

______________________________________________________________________

## 3) Breakpoints + running

### The easiest start: stop at the first line of `main`

```gdb
start
```

This runs the program and stops at the beginning of `main`, so local variables are “in context”.

### Manual way

```gdb
break main
run
```

______________________________________________________________________

## 4) Stepping (how to move through code)

- `next` (or `n`): run **next line**, stepping *over* function calls
- `step` (or `s`): step **into** function calls
- `continue` (or `c`): continue running until next breakpoint
- `finish`: run until the current function returns

______________________________________________________________________

## 5) Inspecting variables (the most common commands)

### Print values

```gdb
print x
print myStruct
print myArray[0]
```

### Print in hexadecimal

```gdb
print/x x
```

### Show locals and arguments

```gdb
info locals
info args
```

### “No symbol ... in current context” (common beginner error)

If you do:

```gdb
print student1
```

and GDB replies:

- `No symbol "student1" in current context.`

it means the program is **not stopped inside** the scope where `student1` exists.

Fix: start or run to a breakpoint inside `main`:

```gdb
start
print student1
```

______________________________________________________________________

## 6) Examining raw memory with `x` (super useful)

The `x` command means “examine memory”. Format:

```gdb
x/<count><format><size> <address>
```

Common ones:

- `x/16bx &var` → 16 bytes, hex, byte-sized
- `x/4wx &var` → 4 “words” (often 4 bytes each), hex
- `x/4bt &var` → 4 bytes, binary

Examples:

```gdb
x/4bx &myInt      # see the 4 raw bytes of an int
x/32bx myArray    # dump 32 bytes from an array
x/s myString      # print as a C string (stops at \0)
```

______________________________________________________________________

## Example A: Debugging the `Student` struct (alignment + bytes)

Given:

```c
struct Student {
  int id;
  char province[3];
  int age;
};

int main() {
  struct Student student1;
  student1.id = 100364168;
  strncpy(student1.province, "ON", sizeof(student1.province));
  student1.age = 18;
}
```

In GDB:

```gdb
start
next  # step until after assignments

print student1
print/x student1.id
print/x student1.age

x/4bx &student1.id
x/3bx student1.province
x/4bx &student1.age

x/16bx &student1  # see raw bytes (often shows padding)
```

What you’ll often notice:

- `province` is 3 bytes, but the struct may contain 1 **padding byte** before `age`.

______________________________________________________________________

## Example B: Quick crash/segfault investigation

Run the program; if it crashes:

```gdb
run
backtrace
```

Useful stack commands:

- `backtrace` (or `bt`): show call stack
- `frame 0`: go to a specific frame
- `list`: show source around current line

______________________________________________________________________

## Example C: Break at a specific file/line

```gdb
break student.c:42
run
```

Good when you want to stop right after a certain assignment.

______________________________________________________________________

## Small cheat-sheet (commands you’ll use all the time)

```gdb
file ./program
start
break main
break file.c:LINE
run

next
step
finish
continue

print expr
print/x expr
info locals
info args

x/16bx &var
x/s str

backtrace
list
quit
```
