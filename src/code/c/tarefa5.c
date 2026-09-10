#include <omp.h>
#include <stdio.h>
#include <time.h>

#define N 200000
void printThread() {

  int countNumber = 0;

  clock_t inicio1 = omp_get_wtime();
  for (int i = 2; i <= N; i++) {
    int count = 0;
    for (int p = 1; p <= i; p++) {
      if (i % p == 0)
        count++;
    }
    if (count == 2)
      countNumber++;
  }

  clock_t fim1 = omp_get_wtime();

  double t1 = (double)(fim1 - inicio1) / CLOCKS_PER_SEC;
  printf("Total primos: %d, tempo: %f \n", countNumber, t1);
}
int main() {
  printThread();
  int countNumber = 0;

  clock_t inicio1 = omp_get_wtime();
#pragma omp parallel num_threads(8) reduction(+ : countNumber)
  {
#pragma omp for
    for (int i = 2; i <= N; i++) {
      int count = 0;
      for (int p = 1; p <= i; p++) {
        if (i % p == 0)
          count++;
      }
      if (count == 2)
        countNumber++;
    }
  }

  clock_t fim1 = omp_get_wtime();

  double t1 = (double)(fim1 - inicio1) / CLOCKS_PER_SEC;
  printf("Total primos: %d, tempo: %f \n", countNumber, t1);

  return 0;
}
