#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 8

int ehPrimo(int x) {
  if (x < 2)
    return 0;
  if (x % 2 == 0)
    return x == 2;
  for (int d = 3; d * d <= x; d += 2) {
    if (x % d == 0)
      return 0;
  }
  return 1;
}

/* ---------------------------------------------------------------------
 * Versões com diferentes clauses para a variável contador
 * ------------------------------------------------------------------- */

int contarPrivate(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) private(contador)
  for (int i = 2; i <= n; i++) {
    if (ehPrimo(i))
      contador++;
  }
  /* contador aqui é a variável original: como private() nao inicializa
     nem copia o valor de volta, ela permanece com o valor de antes do
     laço (0), mesmo que cada thread tenha contado internamente. */
  return contador;
}

int contarFirstprivate(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) firstprivate(contador)
  for (int i = 2; i <= n; i++) {
    if (ehPrimo(i))
      contador++;
  }
  /* firstprivate garante que cada cópia comece em 0 (valor da variável
     original antes do laço), mas assim como private(), nao ha copia de
     volta ao final: a variável original continua 0. */
  return contador;
}

int contarLastprivate(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) lastprivate(contador)
  for (int i = 2; i <= n; i++) {
    if (ehPrimo(i))
      contador++;
  }
  /* lastprivate copia de volta apenas o valor da cópia que executou a
     ultima iteração (sequencialmente falando) do laço. Como essa cópia
     nunca foi inicializada com o valor de fora (lastprivate, sozinha,
     não faz isso), ela começa com lixo de pilha e o incremento é feito
     em cima desse lixo: o resultado é comportamento indefinido. O valor
     impresso pode até parecer "razoável" às vezes (se o lixo inicial
     for pequeno) ou completamente absurdo em outras (dependendo de
     flags de compilação, otimização, etc.) -- em nenhum dos dois casos
     é confiável ou coincide com a contagem real de primos. */
  return contador;
}

int contarFirstLastprivate(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) firstprivate(contador)   \
    lastprivate(contador)
  for (int i = 2; i <= n; i++) {
    if (ehPrimo(i))
      contador++;
  }
  /* combina os dois comportamentos: cada cópia começa em 0 (firstprivate)
     e ao final apenas a cópia da última iteração é copiada de volta
     (lastprivate). O resultado é igualmente incorreto: apenas a
     contagem parcial do último chunk. */
  return contador;
}

int contarDefaultNone(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) default(none) shared(n)  \
    reduction(+ : contador)
  for (int i = 2; i <= n; i++) {
    if (ehPrimo(i))
      contador++;
  }
  /* default(none) obriga a declarar explicitamente o data-sharing de
     toda variável usada dentro da região paralela. Isso nao corrige,
     por si só, o problema de private/firstprivate/lastprivate: aqui o
     resultado só fica correto porque combinamos default(none) com
     reduction(+:contador), a clause certa para esse tipo de acumulo. */
  return contador;
}

int contarReduction(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : contador)
  for (int i = 2; i <= n; i++) {
    if (ehPrimo(i))
      contador++;
  }
  /* reduction cria uma cópia privada de contador por thread (inicializada
     em 0), cada uma acumula sua parte do laço sem nenhuma disputa de
     memória e, ao final, o OpenMP soma todas as cópias na variável
     original. É a única versão acima que produz o resultado correto. */
  return contador;
}

/* ---------------------------------------------------------------------
 * Versões com diferentes escalonadores, todas usando reduction (única
 * variante correta acima) como base.
 * ------------------------------------------------------------------- */

int contarScheduleStatic(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : contador)  \
    schedule(static)
  for (int i = 2; i <= n; i++)
    if (ehPrimo(i))
      contador++;
  return contador;
}

int contarScheduleDynamic(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : contador)  \
    schedule(dynamic)
  for (int i = 2; i <= n; i++)
    if (ehPrimo(i))
      contador++;
  return contador;
}

int contarScheduleGuided(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : contador)  \
    schedule(guided)
  for (int i = 2; i <= n; i++)
    if (ehPrimo(i))
      contador++;
  return contador;
}

int contarScheduleAuto(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : contador)  \
    schedule(auto)
  for (int i = 2; i <= n; i++)
    if (ehPrimo(i))
      contador++;
  return contador;
}

int contarScheduleRuntime(int n) {
  int contador = 0;
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+ : contador)  \
    schedule(runtime)
  for (int i = 2; i <= n; i++)
    if (ehPrimo(i))
      contador++;
  return contador;
}

/* ---------------------------------------------------------------------
 * Infraestrutura de medição
 * ------------------------------------------------------------------- */

typedef int (*funcaoContagem)(int);

typedef struct {
  const char *nome;
  funcaoContagem fn;
} Versao;

void medir(const char *nome, funcaoContagem fn, int n) {
  double inicio = omp_get_wtime();
  int resultado = fn(n);
  double fim = omp_get_wtime();
  printf("%-24s n=%-10d primos=%-9d tempo=%.4fs\n", nome, n, resultado,
         fim - inicio);
}

int main() {
  int valoresDeN[] = {100000, 500000, 1000000, 2000000, 4000000};
  int quantidade = sizeof(valoresDeN) / sizeof(valoresDeN[0]);

  Versao clausulas[] = {
      {"private", contarPrivate},
      {"firstprivate", contarFirstprivate},
      {"lastprivate", contarLastprivate},
      {"first+lastprivate", contarFirstLastprivate},
      {"default(none)", contarDefaultNone},
      {"reduction", contarReduction},
  };
  int numClausulas = sizeof(clausulas) / sizeof(clausulas[0]);

  printf("=== Comparacao das clauses da variavel contador ===\n");
  for (int i = 0; i < quantidade; i++) {
    for (int j = 0; j < numClausulas; j++) {
      medir(clausulas[j].nome, clausulas[j].fn, valoresDeN[i]);
    }
    printf("\n");
  }

  Versao escalonadores[] = {
      {"schedule(static)", contarScheduleStatic},
      {"schedule(dynamic)", contarScheduleDynamic},
      {"schedule(guided)", contarScheduleGuided},
      {"schedule(auto)", contarScheduleAuto},
      {"schedule(runtime)", contarScheduleRuntime},
  };
  int numEscalonadores = sizeof(escalonadores) / sizeof(escalonadores[0]);

  printf("=== Comparacao dos escalonadores (base: reduction) ===\n");
  printf("(schedule(runtime) usa OMP_SCHEDULE=%s)\n",
         getenv("OMP_SCHEDULE") ? getenv("OMP_SCHEDULE") : "(nao definido)");
  for (int i = 0; i < quantidade; i++) {
    for (int j = 0; j < numEscalonadores; j++) {
      medir(escalonadores[j].nome, escalonadores[j].fn, valoresDeN[i]);
    }
    printf("\n");
  }

  return 0;
}

/*
  Discussao dos resultados
  ------------------------------------------------------------------------
  1) Clauses da variavel contador (resultado correto = quantidade real de
     primos entre 2 e n):

     - private: a variavel contador impressa fica sempre em 0. private()
       cria uma copia local por thread sem inicializa-la com o valor de
       fora e, ao final da regiao paralela, essa copia e descartada: a
       variavel original nunca e tocada.

     - firstprivate: tambem fica em 0. A diferenca para private() e so
       a inicializacao interna da copia (comeca com o valor de contador
       antes do laco, que e 0); como continua sem copiar de volta, o
       efeito observado por fora e identico ao de private().

     - lastprivate: o valor impresso e imprevisivel entre execucoes e
       flags de compilacao. Isso acontece porque lastprivate, sozinha,
       nao inicializa a copia privada com o valor de fora: a copia comeca
       com lixo de pilha e o incremento e feito sobre esse lixo. Ao
       final, so o valor da copia da ultima iteracao logica do laco e
       devolvido -- em um teste com -O2 esse lixo inicial gerou um numero
       absurdo (~2*10^8, quando o total real era da ordem de dezenas ou
       centenas de milhares); em outro teste, sem otimizacao, o lixo
       inicial calhou de ser pequeno e o resultado pareceu (por
       coincidencia) uma contagem parcial plausivel. Em ambos os casos e
       comportamento indefinido, nao um resultado confiavel.

     - first+lastprivate: aqui o firstprivate corrige a inicializacao (a
       copia comeca em 0, nao em lixo de pilha), entao o resultado deixa
       de ser um numero absurdo e passa a ser a contagem de primos
       encontrados apenas no ultimo pedaco (chunk) do laco -- bem menor
       que o total real, mas pelo menos coerente. Ainda assim continua
       incorreto, porque lastprivate so devolve a copia da ultima
       iteracao logica, descartando o que as demais threads contaram.

     - default(none): so fica correto porque foi combinado com
       reduction(+:contador); default(none) por si so apenas obriga a
       declarar explicitamente o escopo de cada variavel (aqui, shared(n)
       e reduction(contador)), evitando erros silenciosos de
       compartilhamento, mas nao resolve sozinho o problema de somar um
       contador entre threads.

     - reduction: e a unica clausula, entre as citadas, pensada para esse
       padrao de "somar um valor calculado em paralelo". Cada thread
       acumula sua propria copia sem nenhuma disputa de memoria e o
       runtime soma todas as copias no final, entao o resultado bate com
       a versao sequencial e, alem disso, tende a ser a mais rapida das
       seis, pois nao ha sincronizacao (lock/atomic) dentro do laco.

     Conclusao: reduction e a versao escolhida como melhor, tanto por ser
     a unica correta quanto por ser competitiva em desempenho.

  2) Escalonadores (todos usando reduction, com num_threads(8)):

     - schedule(static): divide o intervalo [2,n] em blocos contiguos de
       tamanho fixo, um por thread, decidido em tempo de compilacao/inicio
       do laco. Tem o menor overhead de escalonamento, mas sofre de
       desbalanceamento de carga: numeros maiores custam mais para testar
       primalidade (o limite de busca de divisores cresce com sqrt(i)), e
       como os blocos sao contiguos, a thread que recebe o bloco final
       (numeros maiores) tende a demorar mais que a que recebe o bloco
       inicial.

     - schedule(dynamic): cada thread pega, por padrao, um unico indice
       por vez assim que termina o anterior, o que da o melhor
       balanceamento de carga possivel (importante aqui, ja que o custo
       de testar primalidade cresce com sqrt(i)), mas ao preco de uma
       secao critica do runtime a cada pegada de indice. Nos testes, esse
       overhead de granularidade tao fina foi o fator dominante: dynamic
       foi consistentemente uma das opcoes mais lentas (ex.: n=500000,
       0.037s contra 0.008s do static; n=4000000, 0.168s contra 0.152s do
       static), mostrando que o custo de sincronizacao superou o ganho de
       balanceamento nessa carga de trabalho.

     - schedule(guided): tambem distribui trabalho dinamicamente, mas com
       blocos que comecam grandes e diminuem geometricamente, reduzindo
       bastante o numero de vezes que uma thread precisa voltar ao
       runtime para pegar mais trabalho (bem menos overhead de
       sincronizacao que o dynamic). Foi o escalonador mais rapido para os
       valores maiores de n testados (ex.: n=4000000, 0.123s, o melhor
       tempo entre todos), o que sugere que ele consegue o equilibrio
       certo entre balanceamento de carga e overhead para esse problema.

     - schedule(auto): delega ao compilador/runtime a escolha do
       escalonamento. Na pratica, com gcc/libgomp, o comportamento
       observado ficou proximo do static (tempos parecidos em quase todos
       os n testados), sugerindo que a implementacao esta caindo de volta
       para static por padrao.

     - schedule(runtime): usa a variavel de ambiente OMP_SCHEDULE para
       decidir a politica em tempo de execucao. Como o teste foi feito
       sem definir essa variavel, a politica usada foi a padrao da
       implementacao (static, segundo a especificacao OpenMP quando
       OMP_SCHEDULE nao esta definida), e os tempos medidos de fato
       oscilaram em torno dos do schedule(static)/schedule(auto), com
       alguma variacao adicional atribuivel ao custo extra de consultar a
       configuracao de ambiente antes do laco.

     Conclusao geral: como o custo de testar primalidade cresce com o
     valor testado (a busca de divisores vai ate sqrt(i)), a carga de
     trabalho e desbalanceada entre numeros pequenos e grandes. Isso
     beneficia escalonadores dinamicos, mas o tamanho do bloco importa
     mais do que apenas "ser dinamico": o guided, que amortiza o overhead
     de sincronizacao com blocos que encolhem aos poucos, foi o que
     melhor equilibrou balanceamento de carga e overhead nos testes,
     enquanto o dynamic (chunk=1 por padrao) pagou um preco de
     sincronizacao alto demais para compensar o ganho de balanceamento.
     static, auto e runtime (sem OMP_SCHEDULE) tiveram desempenho
     parecido entre si, por caírem todos, na pratica, no mesmo
     particionamento estatico e contiguo do laco.
*/
