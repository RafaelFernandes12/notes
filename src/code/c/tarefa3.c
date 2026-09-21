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

/* O retorno void* e receber void* é obrigatorio para funções usadas com
 pthread_create */
void *incrementar(void *arg) {
  for (int i = 0; i < INCREMENTOS; i++) {
    /*
       O mutex serve como lock da variável contador, já que incrementar a
       variável não é uma ação atômica (ler o valor, somar 1, escrever de volta)
       Sem o mutex, ocorreria uma race condition entre as threads, isso é, uma
       thread sobrescrever o valor da outra.
       O mutex funciona do jeito que apenas uma thread por vez pode incrementar
       o valor de contador.
    */
    pthread_mutex_lock(&thread);
    contador++;
    pthread_mutex_unlock(&thread);
  }
  return NULL;
}

int contThread() {
  /* Cria o identificador das 4 threads, essa variável será responsável por
   armazenar o endereço de memoria de onde as threads ficarão */
  pthread_t threads[NUM_THREADS];
  // printa o contador inicial, contador = 0
  printf("Contador inicial: %d\n", contador);
  for (int i = 0; i < NUM_THREADS; i++) {
    /* cria as threads, o primeiro parâmetro é o endereço de onde o sistema vai
      guardar o identificador dessa thread, o segundo parâmetro são as opções
      da thread, no caso aqui serão as padrões, o terceiro parametro será a
      função que será implementada, o ultimo são os parametro da propria função
      do terceiro parâmetro, como ela não recebe nada, é NULL
     */
    int erro = pthread_create(&threads[i], NULL, incrementar, NULL);
    if (erro != 0) {
      fprintf(stderr, "Erro ao criar thread\n");
      exit(EXIT_FAILURE);
    }
  }
  // o pthread_join faz a thread principal esperar as outras threads terminarem
  // suas execuções, é semelhante ao wait dos processos
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("\nContador final: %d\n", contador);
  return 0;
}

int processos() {
  /*
   contador compartilhado entre os processos via mmap: NULL (endereço
   escolhido pelo kernel), tamanho de um int, PROT_READ|WRITE (leitura e
   escrita), MAP_SHARED (o pulo do gato: memória compartilhada entre
   processos) | MAP_ANONYMOUS (fica em memória, sem arquivo).
  */
  _Atomic int *contador =
      mmap(NULL, sizeof(_Atomic int), PROT_READ | PROT_WRITE,
           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (contador == MAP_FAILED) {
    perror("Erro ao alocar memoria compartilhada");
    exit(EXIT_FAILURE);
  }
  *contador = 0;

  printf("Contador inicial: %d\n", *contador);
  /*
   fflush descarrega o buffer do printf acima antes do fork. Sem ele, se a
   saída for redirecionada pra arquivo/pipe (modo fully-buffered: só
   descarrega ao encher 4kb ou o processo terminar), esse texto pendente é
   copiado pra cada filho e acaba duplicado na saída, um "Contador inicial: 0"
   por filho. Em terminal (modo line-buffered) o \n já descarrega na hora,
   então não faz diferença — só importa ao salvar em arquivo.
  */
  fflush(stdout);

  /*
   Loop cria NUM_PROCESSOS (4) filhos via fork(). fork() retorna 0 no filho e
   o pid do filho no pai. O filho entra no if (pid == 0), incrementa o
   contador e sai; o pai não entra, só volta ao loop e cria o próximo filho.
  */
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
      printf("Filho para o pai %d Filho %d: contador = %d\n", pid, getpid(),
             *contador);
      exit(EXIT_SUCCESS);
    }
  }

  /*
   Pai espera os 4 filhos terminarem (um wait por filho). Sem isso ele
   imprimiria o contador antes dos filhos acabarem, com valor errado.
  */
  for (int i = 0; i < NUM_PROCESSOS; i++) {
    wait(NULL);
  }
  printf("\nContador no processo pai: %d\n", *contador);

  munmap(contador, sizeof(_Atomic int));
  return 0;
}

int main() {
  /* processos(); */
  contThread();
  return 0;
}

/*
 O código acima é um exemplo de como resolver os problemas do cont1.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROCESSOS 4
#define INCREMENTOS 1000000
int contador = 0;
int main() {
printf("Contador inicial: %d\n", contador);
for (int i = 0; i < NUM_PROCESSOS; i++) {
pid_t pid = fork();
if (pid < 0) {
perror("Erro ao criar processo");
exit(EXIT_FAILURE);
}
if (pid == 0) {
// Código executado pelo processo filho
for (int j = 0; j < INCREMENTOS; j++) {
contador++;
}
printf("Filho %d: contador = %d\n", getpid(), contador);
exit(EXIT_SUCCESS);
}
}
// Processo pai espera todos os filhos
for (int i = 0; i < NUM_PROCESSOS; i++) {
wait(NULL);
}
printf("\nContador no processo pai: %d\n", contador);
return 0;
e do cont2.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 4
#define INCREMENTOS 1000000
int contador = 0;
void *incrementar(void *arg) {
for (int i = 0; i < INCREMENTOS; i++) {
contador++;
}
return NULL;
}
int main() {
pthread_t threads[NUM_THREADS];
printf("Contador inicial: %d\n", contador);
// Criar as threads
for (int i = 0; i < NUM_THREADS; i++) {
int erro = pthread_create(
&threads[i],
NULL,
incrementar,
NULL
);
if (erro != 0) {
fprintf(stderr, "Erro ao criar thread\n");
exit(EXIT_FAILURE);
}
}
// Esperar todas as threads
for (int i = 0; i < NUM_THREADS; i++) {
pthread_join(threads[i], NULL);
}
printf("\nContador final: %d\n", contador);
return 0;
}
O cont1 é um programa que paraleliza a contagem de um contador por meio de
processos, mas ele tem um problema fundamental, a variável contador não é
compartilhada entre os processos, então cada processo terá sua propria variável
contador e o print do contador ao fim será 0, pois ele não tem acesso a variável
contador dos filhos. Pra resolver isso, eu transformei a variável contador em
uma variável atomica compartilhada entre os processos.

O cont2 tem outro problema, uma race condition, isso é, a variável contador dela
não é atomica, uma thread pode sobrescrever o valor da outra, isso significa que
ao fim o valor do contador estará errado, para resolver isso eu apliquei um
mutex a essa variável dessa forma, apenas uma thread atualiza o valor de
contador, isso é uma das soluções, provavelmente não é a melhor

A principal diferença de um processo e uma thread é que um processo pode conter
diversas threads. Os processos não compartilham memoria entre si, nós precisamos
declarar uma variável compartilhada entre os processos, enquanto que todas as
threads tem acesso ao mesmo espaço de memoria mas elas não são atomicas.
Portanto na pratica para fazer isso dar certo, eu preciso compartilhar a
variável entre os processos e uma forma de sincronizar a variável com as threads
*/
