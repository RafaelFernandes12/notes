#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#define NUM_PROCESSOS 4
#define INCREMENTOS 1000000

#define NUM_THREADS 4
#define INCREMENTOS 1000000

int contador = 0;
pthread_mutex_t thread = PTHREAD_MUTEX_INITIALIZER;

void *incrementar(void *arg) {
  for (int i = 0; i < INCREMENTOS; i++) {
    pthread_mutex_lock(&thread);
    contador++;
    pthread_mutex_unlock(&thread);
  }
  return NULL;
}

int contThread() {
  pthread_t threads[NUM_THREADS];
  printf("Contador inicial: %d\n", contador);
  for (int i = 0; i < NUM_THREADS; i++) {
    int erro = pthread_create(&threads[i], NULL, incrementar, NULL);
    if (erro != 0) {
      fprintf(stderr, "Erro ao criar thread\n");
      exit(EXIT_FAILURE);
    }
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("\nContador final: %d\n", contador);
  return 0;
}

int processos() {
  _Atomic int *contador =
      mmap(NULL, sizeof(_Atomic int), PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (contador == MAP_FAILED) {
    perror("Erro ao alocar memoria compartilhada");
    exit(EXIT_FAILURE);
  }
  *contador = 0;

  printf("Contador inicial: %d\n", *contador);
  fflush(stdout);

  for (int i = 0; i < NUM_PROCESSOS; i++) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("Erro ao criar processo");
      exit(EXIT_FAILURE);
    }
    if (pid == 0) {
      for (int j = 0; j < INCREMENTOS; j++) {
        atomic_fetch_add_explicit(contador, 1, memory_order_relaxed);
      }
      printf("Filho %d: contador = %d\n", getpid(), *contador);
      exit(EXIT_SUCCESS);
    }
  }

  for (int i = 0; i < NUM_PROCESSOS; i++) {
    wait(NULL);
  }
  printf("\nContador no processo pai: %d\n", *contador);

  munmap(contador, sizeof(_Atomic int));
  return 0;
}

int main() {
  processos();
  contThread();
  return 0;
}

/*
 Na função processos, a primeira coisa que eu faço é criar variável *contador
 que será compartilhada entre os processos usando a função nmap.

*/
