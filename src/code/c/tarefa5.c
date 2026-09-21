#include <omp.h>
#include <stdio.h>
#include <time.h>

#define N 20000
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
#pragma omp parallel num_threads(8)
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

/*
  Nesse código nós estamos paralelizando o for loop com a diretiva for do omp,
  acima dela eu escolho o número de threads do meu pc baseado no número de cores
  que o meu processador tem, que no caso são 8.
  A diretiva for nesse caro divide o loop em 8 threads, o que faz o resultado
  ser mais rápido, mas mesmo assim para números grandes o tempo de execução
  ainda será ENORME. Isso ocorre por que a divisão de trabalho é desigual,
  enquanto que as primeiras threads vão ficar com repetições pequena, as ultimas
  vão ficar com enormes laços de repetição
*/
