#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void injetar(double **A, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      A[i][j] = rand() % 10;
}

void multiplicar_linhas(double **A, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      A[i][j] *= 2;
}

void multiplicar_colunas(double **A, int n) {
  for (int j = 0; j < n; j++)
    for (int i = 0; i < n; i++)
      A[i][j] *= 2;
}

int main() {
  int tamanhos[] = {100, 200, 1000, 4000, 5000, 10000};

  for (int t = 0; t < 6; t++) {
    int n = tamanhos[t];

    double **A = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
      A[i] = malloc(n * sizeof(double));

    injetar(A, n);

    clock_t inicio1 = clock();
    multiplicar_linhas(A, n);
    clock_t fim1 = clock();

    clock_t inicio2 = clock();
    multiplicar_colunas(A, n);
    clock_t fim2 = clock();

    double t1 = (double)(fim1 - inicio1) / CLOCKS_PER_SEC;
    double t2 = (double)(fim2 - inicio2) / CLOCKS_PER_SEC;
    double aumento = (t2 - t1) / t1 * 100;
    printf("n=%d | linhas: %.4fs | colunas: %.4fs | aumento: %.2f%%\n", n, t1,
           t2, aumento);

    for (int i = 0; i < n; i++)
      free(A[i]);
    free(A);
  }

  return 0;
}
