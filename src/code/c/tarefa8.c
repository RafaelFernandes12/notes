#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double f(double x) { return sin(x); }

// Tarefa 1: gera o vetor. As tarefas 2 e 3 dependem dela, entao roda antes.
void gerar_vetor(double *v, double a, double h, int n) {
#pragma omp parallel num_threads(2)
  {
#pragma omp for
    for (int i = 0; i < n; i++) {
      double x = a + i * h;
      v[i] = f(x);
    }

#pragma omp single
    printf("[tarefa 1 | thread %d de %d] vetor de %d pontos gerado\n",
           omp_get_thread_num(), omp_get_num_threads(), n);
  }
}

// Tarefa 2: integral pelo metodo do trapezio.
double integral_trapezio(double *v, int n, double h) {
  double soma = 0.0;
  double resultado = 0.0;

#pragma omp parallel num_threads(2)
  {
#pragma omp for reduction(+ : soma)
    for (int i = 1; i < n - 1; i++) {
      soma += v[i];
    }

#pragma omp single
    {
      resultado = h * ((v[0] + v[n - 1]) / 2.0 + soma);
      printf("[tarefa 2 | thread %d de %d] integral de sin(x) em [0, pi] = "
             "%.6f (exato: 2.0)\n",
             omp_get_thread_num(), omp_get_num_threads(), resultado);
    }
  }

  return resultado;
}

// Tarefa 3: derivada por diferencas finitas.
void derivada_diferencas_finitas(double *v, double *dv, int n, double h,
                                 double a) {
#pragma omp parallel num_threads(2)
  {
#pragma omp single nowait
    {
      dv[0] = (v[1] - v[0]) / h;
      dv[n - 1] = (v[n - 1] - v[n - 2]) / h;
    }

#pragma omp for
    for (int i = 1; i < n - 1; i++) {
      dv[i] = (v[i + 1] - v[i - 1]) / (2.0 * h);
    }

#pragma omp single
    {
      printf("[tarefa 3 | thread %d de %d] derivada numerica x cos(x):\n",
             omp_get_thread_num(), omp_get_num_threads());
      for (int i = 0; i < n; i += n / 5) {
        double x = a + i * h;
        printf("    x = %.4f | dv[%d] = %.6f | cos(x) = %.6f\n", x, i, dv[i],
               cos(x));
      }
    }
  }
}

int main(void) {
  double a = 0.0, b = M_PI;
  int n = 1000;
  double h = (b - a) / (n - 1);

  double *v = malloc(n * sizeof(double));
  double *dv = malloc(n * sizeof(double));
  if (!v || !dv) {
    fprintf(stderr, "falha ao alocar memoria\n");
    return 1;
  }

  // permite que as regioes paralelas dentro de cada section abram threads
  omp_set_max_active_levels(2);

  gerar_vetor(v, a, h, n);

  // tarefas 2 e 3 sao independentes entre si: rodam em sections paralelas
#pragma omp parallel sections num_threads(2)
  {
#pragma omp section
    integral_trapezio(v, n, h);

#pragma omp section
    derivada_diferencas_finitas(v, dv, n, h, a);
  }

  free(v);
  free(dv);
  return 0;
}

/*
  O paralalelismo de tarefas foi usado com a diretiva sections e
  omp_set_max_active_levels do omp, a diretiva sections me permite colocar 1
  thread para executar um bloco de código e o omp_set_max_active_levels permite
  que cada section tenha 2 threads dentro delas.
  Depois disso, dentro da função integral_trapezio, eu declaro um parallel
  num_threads, que é o número de threads que executará o próximo código, o for
  reduction, o reduction aqui é importante, pois ela cria uma copia privada para
  cada thread da variável soma e ao fim soma todas elas, isso evita uma race
  condition entre as threads, onde elas poderiam somar um valor antigo da
  variável.
  Por fim é usado a diretiva single para printar o resultado, a diretiva single
  funciona da seguinte forma, a primeira thread que chegar nela, executará
  aquele pedaço de código, e as outras iram esperar o fim dessa execução, sem
  essa diretiva, as duas threads imprimiriam o resultado, o que deixaria a saída
  mais poluida

  Ao mesmo tempo que a função integral_trapezio é executada, é executada a
  função derivada_diferencas_finitas.
  Nela nós definimos 2 threads também para executar aquele pedaço de código com
  a diretiva parallel num_threads(2).
  Aqui é usado também a diretiva single nowait, o single serve para a primeira
  thread que chegar ali, executar aquele código, e o nowait serve para não
  impedir as outras threads de avançarem, já que a diretiva single possui uma
  barreira implícita, ou seja, as outras threads esperar a thread single
  concluir sua execução para avançar. Com a clausula nowait, as outras threads
  passam direto. Nesse caso não é problema pois a segunda thread não depende dos
  valores dv[0] e dv[n-1], caso a segunda thread precisasse deles, isso causaria
  uma race condition.
  Após isso há a diretiva for, no qual irá dividir o loop em 2, uma metade para
  a segunda thread, e a outra metade será para a primeira thread, a do single,
  quando ela terminar a execução do single, ela irá também executar o for. Ao
  fim vem o single novamente para imprimir o resultado, a primeira thread que
  chegar ali após a finalização do for, irá printar o resultado.
*/
