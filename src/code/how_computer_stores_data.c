#include <stdio.h>
#include <string.h>

struct Student {
  int id;
  char province[3]; // ON, BC etc. + terminating null
  int age;
};
int main() {
  struct Student student1;
  // Assign values to structure variables
  student1.id = 100364168;
  strncpy(student1.province, "ON\0", sizeof(student1.province));
  student1.age = 18;
  printf("The size of struct member id is %ld bytes\n", sizeof(student1.id));
  printf("The size of struct member province is %ld bytes\n",
         sizeof(student1.province));
  printf("The size of struct member age is %ld bytes\n", sizeof(student1.age));
  printf("The size of struct Student is %ld bytes\n", sizeof(struct Student));
  return 0;
}
