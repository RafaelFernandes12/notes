#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long n = 1000000000;

void populateArray(long vetor[n]) {
  for (long i = 0; i < n; i++) {
    vetor[i] = 1;
  }
}

int forSimples(long vetor[n]) {
  long soma = 0;
  for (long i = 0; i < n; i++) {
    soma += vetor[i];
  }
  return soma;
}
int forComplexo(long vetor[n]) {

  long soma = 0;
  long s0 = 0;
  long s1 = 0;
  long s2 = 0;
  long s3 = 0;

  for (long i = 0; i + 4 <= n; i += 4) {
    s0 += vetor[i];
    s1 += vetor[i + 1];
    s2 += vetor[i + 2];
    s3 += vetor[i + 3];
  }
  soma = s0 + s1 + s2 + s3;
  return soma;
}

int main() {
  long *vetor = malloc(n * sizeof(long));
  populateArray(vetor);

  clock_t inicio1 = clock();
  int soma1 = forSimples(vetor);
  clock_t fim1 = clock();
  double t1 = (double)(fim1 - inicio1) / CLOCKS_PER_SEC;

  clock_t inicio2 = clock();
  int soma2 = forComplexo(vetor);
  clock_t fim2 = clock();
  double t2 = (double)(fim2 - inicio2) / CLOCKS_PER_SEC;
  printf("for simples:  %f %i\n", t1, soma1);
  printf("for complexo:  %f %i\n", t2, soma2);

  return 0;
}
