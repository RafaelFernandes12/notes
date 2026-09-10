#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM_PROCESSOS 4
#define INCREMENTOS 1000000

int main() {
  // fork() copia o espaco de enderecamento, entao uma variavel global daria
  // a cada filho o SEU proprio contador. MAP_SHARED faz o inverso: todos os
  // processos veem a mesma pagina de memoria.
  _Atomic int *contador = mmap(NULL, sizeof(_Atomic int), PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (contador == MAP_FAILED) {
    perror("Erro ao alocar memoria compartilhada");
    exit(EXIT_FAILURE);
  }
  *contador = 0;

  printf("Contador inicial: %d\n", *contador);
  fflush(stdout); // esvazia o buffer antes do fork, senao cada filho
                  // herda a copia do buffer e reimprime a linha

  for (int i = 0; i < NUM_PROCESSOS; i++) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("Erro ao criar processo");
      exit(EXIT_FAILURE);
    }
    if (pid == 0) {
      // Código executado pelo processo filho
      for (int j = 0; j < INCREMENTOS; j++) {
        // relaxed: basta que nenhum incremento se perca, nao precisamos
        // ordenar mais nada em volta do contador
        atomic_fetch_add_explicit(contador, 1, memory_order_relaxed);
      }
      printf("Filho %d: contador = %d\n", getpid(), *contador);
      exit(EXIT_SUCCESS);
    }
  }

  // Processo pai espera todos os filhos; wait() ja e a barreira de
  // sincronizacao que garante ver os incrementos de todos eles
  for (int i = 0; i < NUM_PROCESSOS; i++) {
    wait(NULL);
  }
  printf("\nContador no processo pai: %d\n", *contador);

  munmap(contador, sizeof(_Atomic int));
  return 0;
}
