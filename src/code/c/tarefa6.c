#include <omp.h>
#include <stdio.h>

double f(double x) { return x * x; }

void sequencial(double a, double b, int n, double h) {

  double soma = 0.0;
  double inicio = omp_get_wtime();
  for (int i = 0; i < n; i++) {
    double x = a + i * h;
    soma += f(x);
  }
  double resultado = h * ((f(a) + f(b) / 2.0 + soma));

  double fim = omp_get_wtime();
  printf("resultado da integral: %f\n", resultado);
  printf("tempo do sequencial %f\n", (double)(fim - inicio));
}

void parallelFor(double a, double b, int n, double h) {

  double inicio = omp_get_wtime();

  double soma = 0.0;
#pragma omp parallel for num_threads(8)
  for (int i = 0; i < n; i++) {
    double x = a + i * h;
    soma += f(x);
  }
  double resultado = h * ((f(a) + f(b) / 2.0 + soma));

  double fim = omp_get_wtime();
  printf("resultado da integral: %f\n", resultado);
  printf("tempo do parallel for %f\n", (double)(fim - inicio));
}

void parallelForCritical(double a, double b, int n, double h) {

  double soma = 0.0;
  double inicio = omp_get_wtime();

#pragma omp parallel for num_threads(8)
  for (int i = 0; i < n; i++) {
    double x = a + i * h;

    double fx = f(x);
#pragma omp critical
    soma += fx;
  }
  double resultado = h * ((f(a) + f(b) / 2.0 + soma));

  double fim = omp_get_wtime();
  printf("resultado da integral: %f\n", resultado);
  printf("tempo do parallel for critical %f\n", (double)(fim - inicio));
}

void parallelForAtomic(double a, double b, int n, double h) {

  double soma = 0.0;
  double inicio = omp_get_wtime();

#pragma omp parallel for num_threads(8)
  for (int i = 0; i < n; i++) {
    double x = a + i * h;

    double fx = f(x);
#pragma omp atomic
    soma += fx;
  }
  double resultado = h * ((f(a) + f(b) / 2.0 + soma));

  double fim = omp_get_wtime();
  printf("resultado da integral: %f\n", resultado);
  printf("tempo do parallel for atomic %f\n", (double)(fim - inicio));
}

void parallelForReduction(double a, double b, int n, double h) {

  double soma = 0.0;
  double inicio = omp_get_wtime();

#pragma omp parallel for num_threads(8) reduction(+ : soma)
  for (int i = 0; i < n; i++) {
    double x = a + i * h;

    double fx = f(x);
    soma += fx;
  }
  double resultado = h * ((f(a) + f(b) / 2.0 + soma));

  double fim = omp_get_wtime();
  printf("resultado da integral: %f\n", resultado);
  printf("tempo do parallel for reduction %f\n", (double)(fim - inicio));
}

int main() {
  double a = 0.0, b = 1.0;
  int n = 2000000000;
  double h = (b - a) / n;

  sequencial(a, b, n, h);
  parallelFor(a, b, n, h);
  parallelForCritical(a, b, n, h);
  parallelForAtomic(a, b, n, h);
  parallelForReduction(a, b, n, h);

  return 0;
}

/*
  Nos testes acima a ordem de velocidade foi de parallelForReduction >
  sequencial > parallelFor > parallelForAtomic > ParallelForCritical

  Isso ocorre por que o reduction cria uma copia da variável soma para cada uma
  das threads e ao fim soma todas elas, uma opção bastante parada e não trava
  nenhuma cpu, 0 overhead.

  O sequencial vem logo em seguida por que essa não é uma operação que demanda
  muito poder computacional, então apenas o preço de mudar de contexto entre as
  threads já leva mais tempo do que a computação em si.

  O parallel For é um pouco mais lento do que a sequencial exatamente por conta
  do overhead de criar e destruir threads, mas em compensação, o resultado está
  errado. Já que o valor de soma não é atómico, todas as threads escrevem e
  sobrescrevem soma, então apesar de ser "rápida", o resultado sai errado.

  O atomic é mais lento que o parallelFor sem proteção porque, mesmo sem lock,
  toda escrita concorrente em soma já gera "cache line bouncing" (o protocolo
  de coerência de cache invalida a cópia da variável nos outros núcleos a cada
  escrita). O atomic paga esse mesmo custo e ainda adiciona uma instrução
  atômica de hardware por iteração (fence de memória garantindo a ordem e a
  atomicidade da soma), o que é mais caro que uma escrita comum, mesmo que
  ainda seja bem mais leve que um lock.

  O critical é o mais lento de todos porque não usa uma instrução atômica de
  hardware, e sim um mutex (lock). Cada uma das 2 bilhões de iterações precisa
  adquirir e liberar esse lock, e quando há disputa entre as 8 threads a
  thread bloqueada pode ser suspensa pelo kernel (futex), envolvendo troca de
  contexto. Esse overhead de sincronização é ordens de magnitude maior que o
  custo da própria soma, dominando completamente o tempo de execução.
*/
