#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NOME 100
#define NUM_ARQUIVOS 5
#define NUM_ALGARISMOS 10 // algarismos de 0 a 9

typedef struct No {

  int arquivo[MAX_NOME];
  int tamanho_arquivo;

  struct No *proximo;

} No;

// Cria um novo nó e preenche o arquivo com números aleatórios
No *criar_no() {
  No *novo = (No *)malloc(sizeof(No));

  if (novo == NULL) {
    printf("Erro na alocacao de memoria.\n");
    exit(1);
  }

  // Define aleatoriamente o tamanho do arquivo
  novo->tamanho_arquivo = 1 + rand() % MAX_NOME;

  // Preenche o arquivo com algarismos de 0 a 9
  for (int i = 0; i < novo->tamanho_arquivo; i++) {
    novo->arquivo[i] = rand() % 10;
  }

  novo->proximo = NULL;

  return novo;
}

// Insere um novo arquivo no final da lista
void inserir(No **lista) {
  No *novo = criar_no();

  if (*lista == NULL) {
    *lista = novo;
    return;
  }

  No *atual = *lista;

  while (atual->proximo != NULL) {
    atual = atual->proximo;
  }

  atual->proximo = novo;
}

// Imprime os arquivos da lista
void imprimir_lista(No *lista) {
  No *atual = lista;

  int numero_arquivo = 1;

  while (atual != NULL) {

    printf("Arquivo %d (tamanho = %d):\n", numero_arquivo,
           atual->tamanho_arquivo);

    for (int i = 0; i < atual->tamanho_arquivo; i++) {
      printf("%d ", atual->arquivo[i]);
    }

    printf("\n\n");

    atual = atual->proximo;
    numero_arquivo++;
  }
}

// Conta as ocorrências de cada algarismo em um único arquivo (executado por
// uma task) e depois soma o resultado no histograma global.
void processar_arquivo(No *no, int *histograma, omp_lock_t *lock) {
  // Histograma local à task, sem necessidade de sincronização enquanto conta.
  int local[NUM_ALGARISMOS] = {0};

  for (int i = 0; i < no->tamanho_arquivo; i++) {
    local[no->arquivo[i]]++;
  }

  // Região crítica: várias tasks podem chamar processar_arquivo ao mesmo
  // tempo, então o acesso ao histograma compartilhado é protegido pelo lock.
  omp_set_lock(lock);
  for (int d = 0; d < NUM_ALGARISMOS; d++) {
    histograma[d] += local[d];
  }
  omp_unset_lock(lock);
}

// Libera a memória da lista
void liberar_lista(No *lista) {
  No *atual = lista;

  while (atual != NULL) {

    No *temp = atual;
    atual = atual->proximo;

    free(temp);
  }
}

// Percorre a lista encadeada e dispara uma task para processar cada arquivo
// em paralelo, já que o tamanho da lista e de cada arquivo é irregular.
void contarNumeros(No *lista, int *histograma) {
  omp_lock_t lock;
  omp_init_lock(&lock);

#pragma omp parallel
  {
    // Apenas uma thread percorre a lista e cria as tasks, evitando que
    // várias threads andem pelo mesmo ponteiro "atual" ao mesmo tempo.
#pragma omp single
    {
      No *atual = lista;

      while (atual != NULL) {
        // firstprivate(atual) faz cada task guardar sua própria cópia do
        // ponteiro no momento da criação, já que "atual" muda a cada
        // iteração do laço antes da task ser de fato executada.
#pragma omp task firstprivate(atual)
        processar_arquivo(atual, histograma, &lock);

        atual = atual->proximo;
      }
      // Barreira implícita do single: a thread só sai daqui quando todas
      // as tasks tiverem sido criadas (não necessariamente concluídas).
    }
    // Barreira implícita do parallel: garante que todas as tasks terminem
    // antes das threads saírem da região paralela.
  }

  omp_destroy_lock(&lock);
}
int main() {
  No *lista = NULL;
  // Histograma global, compartilhado entre as tasks e atualizado com lock.
  int histograma[NUM_ALGARISMOS] = {0};

  srand(time(NULL));

  // Cria NUM_ARQUIVOS arquivos
  for (int i = 0; i < NUM_ARQUIVOS; i++) {
    inserir(&lista);
  }

  imprimir_lista(lista);

  // Processa os arquivos em paralelo e contabiliza os algarismos.
  contarNumeros(lista, histograma);

  printf("Histograma global de algarismos:\n");
  for (int d = 0; d < NUM_ALGARISMOS; d++) {
    printf("  algarismo %d: %d ocorrencias\n", d, histograma[d]);
  }

  liberar_lista(lista);

  return 0;
}

/* Discussão
  Programação regular e irregular: o problema é irregular porque nem a
  quantidade de arquivos nem o tamanho de cada um são conhecidos em tempo
  de compilação (tamanho_arquivo é sorteado em criar_no) e a estrutura é
  uma lista encadeada, sem acesso indexado. Por isso não dá pra dividir o
  trabalho em blocos uniformes com um "parallel for" (adequado a
  paralelismo regular); cada arquivo vira uma unidade de trabalho própria.

  Criação e execução de tasks: dentro de contarNumeros, cada iteração do
  laço que percorre a lista cria uma task com "#pragma omp task", que
  processa um único arquivo em processar_arquivo. As tasks são colocadas
  num pool e executadas pelas threads da região paralela conforme ficam
  livres, sem que a distribuição do trabalho precise ser prevista antes.

  Processamento de uma estrutura de dados representada por uma lista
  encadeada: como a lista só pode ser percorrida sequencialmente (via
  "atual->proximo"), apenas uma thread ("#pragma omp single") faz esse
  percurso e cria as tasks; o processamento de cada arquivo, em si, é
  que roda em paralelo através das tasks já criadas.

  Acesso concorrente a dados compartilhados: o vetor "histograma" é
  compartilhado por todas as tasks, que podem terminar de contar seus
  arquivos e tentar atualizá-lo ao mesmo tempo, o que caracteriza uma
  condição de corrida se não houver controle.

  Exclusão mútua: para evitar a condição de corrida, cada task primeiro
  conta os algarismos do seu arquivo num vetor "local" (sem precisar de
  sincronização, pois é privado à task) e só depois soma esse resultado
  no histograma global dentro de uma seção protegida por lock, garantindo
  que apenas uma task por vez modifique o histograma.

  Utilização de locks explícitos: em vez de "#pragma omp critical", foi
  usado um omp_lock_t (omp_init_lock, omp_set_lock, omp_unset_lock e
  omp_destroy_lock), como pedido no enunciado. O lock é criado uma única
  vez em contarNumeros e passado por ponteiro para todas as tasks, que o
  compartilham para proteger a mesma seção crítica.

  Sincronização e conclusão das tasks: não foi necessário um
  "#pragma omp taskwait" explícito porque tanto a região "single" quanto a
  região "parallel" têm barreira implícita ao final. A barreira do
  "single" garante que todas as tasks já tenham sido criadas antes da
  thread seguir; a barreira do "parallel" garante que todas as tasks
  tenham efetivamente terminado antes de omp_destroy_lock ser chamado e
  da função retornar, evitando destruir o lock com tasks ainda em execução.

  Impacto da sincronização no desempenho da aplicação: como cada task
  primeiro acumula em um histograma local e só entra na seção crítica
  para uma soma rápida (10 inteiros), o tempo gasto dentro do lock é
  pequeno comparado ao processamento do arquivo, minimizando a disputa
  pelo lock. Se a atualização do histograma global fosse feita direto
  dentro do laço de contagem (sem o vetor local), o lock seria adquirido
  e liberado uma vez por algarismo lido, aumentando muito a contenção
  entre as tasks e degradando o ganho de desempenho do paralelismo.
*/
