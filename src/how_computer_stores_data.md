# how_computer_stores_data

______________________________________________________________________

**Date:** 2026-03-03
**Tags:**

- [Uf.md](tags/Uf.md)
- [Forense.md](tags/Forense.md)
- [Security.md](tags/Security.md)

**URL:** [how_computer_stores_data.md](https://github.com/ramonfontes/security/blob/master/how_computer_stores_data.md)

______________________________________________________________________

# Question

## How a computer stores and processes data

**In this short demo you will:**

- Learn on how a computer stores and processes data

**Requirements:**

- C Programming Language

For this exercise, assume that you have a physical or virtual Linux system with a C compiler installed, for example Linux GNU GCC.

Consider the following C program:

```C=
#include <stdio.h>
#include <string.h>

struct Student
{ int id;
  char province[3]; //ON, BC etc. + terminating null
  int age;
};
 int main( ){
  struct Student student1;
  // Assign values to structure variables
  student1.id = 100364168;
  strncpy(student1.province, "ON\0", sizeof(student1.province));
  student1.age = 18;
  printf("The size of struct member id is %ld bytes\n", sizeof(student1.id));
  printf("The size of struct member province is %ld bytes\n", sizeof(student1.province));
  printf("The size of struct member age is %ld bytes\n", sizeof(student1.age));
  printf("The size of struct Student is %ld bytes\n", sizeof(struct Student));
  return 0;
}
```

## Part I: Data Alignment

Answer the following questions based on the above C code, by filling in all of the blanks where indicated based on the output of the program (Table 1). Note that the size of a variable or data type is evaluated using sizeof operator in C Programming.

Table1. Storage sizes of variables or data types in the above C code

| Variable or data type | Syze in bytes |
|------------------------------------|---------------|
| student1.id (**100**) | |
| student1.province (**101**) | |
| student1.age (**102**) | |
| Add lines **100**, **101**, and **102** (**103**) | |
| Struct student (**104**) | |

**Question 1**\
Which is bigger? Student (104) is bigger because, the subtotal of the sizes of all members of student (103) is 4 (id) + 3 (province) + 4 (age) = 11, but since age is an int, it must start in a multiple of 4, so the compiler will add a padding byte for province, so age can start at a multiple of 4, so the total for Student (103) will be 4 (id) + 3 (province) + 1 (province padding byte) + 4 (age) = 12.

1. The size of struct Student (**104**) or
1. Subtotal of the sizes of all the members of struct Student (**103**).

## Part II: Get the Representation of Data Using GDB

In the following exercises we will look into how computers represent data based on the above code. Specially, you will use GDB commands to look into the memory to get the representations of all the members of struct Student. For example, if we declare an int variable:

**int a = 16;**

Then we can use the x command in GDB

(gdb) **x/4bt &a**\
**0xbffff56c: 00010000 00000000 00000000 00000000**

where the first item is memory address where an integer value (in 4 bytes) is stored, the second item is the content stored in the memory.

Now, answer the following questions after debugging the above C program, by filling in all of the blanks with the output of GDB commands (Table 2):

Table 2: Data presentation of variables in the above C code

| Variables | Representation of values (in hexadecimal) (after a value is assigned to the variable) |
|-------------------|---------------|
| student1.id | 0x7fffffffd634: 10001000 01101111 11111011 00000101|
| student1.province | 0x7fffffffd638: 01001111 01001110 00000000 11110111|
| student1.age | 0x7fffffffd63c: 00010010 00000000 00000000 00000000|
| student1 | 0x7fffffffd634: 10001000 01101111 11111011 00000101|
