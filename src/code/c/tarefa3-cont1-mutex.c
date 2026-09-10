#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#define NUM_PROCESSOS 4
#define INCREMENTOS 1000000
int main() {
  int *contador = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *contador = 0;

  pthread_mutex_t *mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(mutex, &attr);

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
        pthread_mutex_lock(mutex);
        (*contador)++;
        pthread_mutex_unlock(mutex);
      }
      printf("Filho %d: contador = %d\n", getpid(), *contador);
      exit(EXIT_SUCCESS);
    }
  }
  // Processo pai espera todos os filhos
  for (int i = 0; i < NUM_PROCESSOS; i++) {
    wait(NULL);
  }
  printf("\nContador no processo pai: %d\n", *contador);

  pthread_mutex_destroy(mutex);
  munmap(mutex, sizeof(pthread_mutex_t));
  munmap(contador, sizeof(int));
  return 0;
}
