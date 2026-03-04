# Sistemas Operacionais (Introducao)

______________________________________________________________________

**Date:** 2026-03-04
**Tags:**

- [Uf](../tags/Uf.md)
- [SistemasOperacionais](../tags/SistemasOperacionais.md)
- [Kernel](../tags/Kernel.md)
- [Processos](../tags/Processos.md)
- [Memoria](../tags/Memoria.md)
- [Arquivos](../tags/Arquivos.md)
- [EntradaESaida](../tags/EntradaESaida.md)
- [ChamadasDeSistema](../tags/ChamadasDeSistema.md)

**Assets:**

- [Aula_01\_-_SO_-\_2026.1.pdf](../assets/Aula_01_-_SO_-_2026.1.pdf)
- \[Sistemas operacionais modernos\](../assets/TANENBAUM_BOOK.indb - Sistemas Operacionais Modernos Andrew S. Tanenbaum, Herbert Bos.pdf)

**URL:**

- https://www.youtube.com/watch?v=yN1UHVAKnnw&list=PLlfjpAXy4GaE3FeHp6KgPFjt6ShUJ3_1n&index=1&t=533s

______________________________________________________________________

## Summary

- **SO (sistema operacional)** e a ponte entre **aplicacoes** e **hardware**.
- Ele opera em **modo nucleo** e oferece **abstracoes** (processo, arquivo, espaco de enderecamento) para esconder detalhes do hardware.
- Outra funcao central: **gerenciar recursos** (CPU, memoria, dispositivos de E/S) para permitir **multitarefa**.

______________________________________________________________________

## Definicao (o que e um SO?)

Um sistema operacional e um conjunto de programas que:

1. **intermedia** o acesso ao hardware (aplicacoes nao acessam diretamente o hardware)
1. **gerencia recursos** do computador de forma ordenada

No modelo mostrado no PDF:

```
Aplicacao -> Sistema Operacional -> Hardware
```

______________________________________________________________________

## Funcoes essenciais (com exemplos)

### 1) Prover uma interface simples para acessar hardware

O SO cria **abstracoes** que deixam o acesso ao hardware mais simples:

- **Arquivo**: salvar sem saber a localizacao exata no disco
- **Processo**: programa em execucao
- **Espaco de enderecamento**: memoria “virtual” vista pelo processo

**Exemplo 1 (arquivo + chamada de sistema):**

Quando voce roda `mkdir pasta`, o terminal faz uma **chamada de sistema** para o SO criar o diretorio (o app nao escreve no disco direto).

**Exemplo 2 (processo):**

Abrir duas abas do navegador pode gerar **dois processos** separados, cada um com seu espaco de memoria.

______________________________________________________________________

### 2) Gerenciar recursos de hardware (CPU, memoria, E/S)

Se varios programas competem pelos mesmos recursos, o SO organiza o uso:

- **Multiplexacao no tempo**: alternar uso da CPU/impressora entre processos
- **Multiplexacao no espaco**: dividir memoria/armazenamento entre processos

**Exemplo 3 (concorrencia de recursos):**

Dois apps imprimindo ao mesmo tempo → o SO coloca um trabalho na fila e depois o outro (tempo).

______________________________________________________________________

## Modos de operacao

- **Modo nucleo (kernel mode):** acesso total ao hardware
- **Modo usuario (user mode):** acesso restrito

O SO executa em modo nucleo, enquanto os aplicativos ficam em modo usuario.

______________________________________________________________________

## Como seria um mundo sem SO?

- Cada programa precisaria conhecer detalhes especificos do hardware
- Sem gerenciamento, **nao existiria multitarefa**
- Cada app precisaria reimplementar tarefas basicas (arquivo, rede, rede de internet, impressao)
- Maquinas seriam **hiperespecializadas** (um computador para cada tarefa, uma máquina para editar vídeos, uma máquina para excel, uma máquina para calculadora, etc)

______________________________________________________________________

## Historico (resumo por geracoes)

- **1a geracao (1945–1955)**: sem SO; programacao com chaves e plugs; as mesmas pessoas projetavam, construíam, operavam e mantinham as máquinas
- **2a geracao (1956–1965)**: transistores; Sistema em lote para reduzir o tempo despediçado; precursor do SO; surgiram as linguagens Fortran e Assembly; Surgiu o antecessor do SO;
- **3a geracao (1966–1980)**: Encolhimento dos computadores (Antes um computador era do tamanho de uma casa, agora do tamanho de uma mesa); IBM desenvolve máquinas com softwares compativeis (OS/360); circuitos integrados; OS/360; escalonamento de CPU
- **4a geracao (1981–1990)**: Computadores pessoais, preços acessíveis; SOs mais amigaveis; multiprocessadores para computadores de alto desempenho
- **5a geracao (1991–hoje)**: maior abstracao; mobile; processamento distribuido; cloud

______________________________________________________________________

## Conceitos basicos (definicoes curtas)

- **Chamada de sistema:** interface que apps usam para pedir servicos ao SO, o mkdir requisita a criação de uma pasta ao núcleo.
- **Processo:** programa em execucao, cada aba do chrome é um processo.
- **Espaco de enderecamento:** memoria virtual vista pelo processo
- **Dispositivos de E/S:** teclado (entrada), monitor (saida)
- **Arquivo:** forma persistente de armazenar dados sem conhecer o local fisico

______________________________________________________________________

- ## Abstracoes criadas pelo sistema operacional (com exemplos)

Abstracoes sao “modelos simplificados” que escondem detalhes do hardware.

- **Processo:** um programa em execucao.
  - Ex.: abrir duas abas do navegador → dois processos.
- **Espaco de enderecamento (memoria virtual):** cada processo “acha” que tem memoria so dele.
  - Ex.: o app usa o endereco `0x1000`, mas o SO mapeia para outro lugar na RAM.
- **Arquivo:** dados persistentes sem lidar com setores do disco.
  - Ex.: salvar `relatorio.txt` sem saber onde fica no HD/SSD.
- **Dispositivos de E/S como arquivos (Unix):** teclado, tela e rede tratados como arquivos\
  especiais.
  - Ex.: escrever em `/dev/tty` envia texto para o terminal.
- **Sockets:** canal logico de comunicacao na rede.
  - Ex.: um servidor “escuta” numa porta e recebe dados sem lidar com o hardware.

## Questoes de fixacao (texto original)

**Questao 1**
CESPE/CEBRASPE - PREFEITURA DE CACHOEIRO DE ITAPEMIRIM – 2024
Operações diretas na memória do sistema são restritas para garantir a
este segurança e integridade, por isso o acesso do sistema operacional à
memória é incompleto.

- [ ] Verdadeiro
- [X] Falso

**Questao 2**
IGEDUC - PREFEITURA DE PRIMAVERA – 2024
Os sistemas operacionais modernos não são responsáveis pelo
gerenciamento de recursos de hardware, pois essa tarefa é
exclusivamente atribuída aos aplicativos que rodam sobre eles.

- [ ] Verdadeiro
- [X] Falso

**Questao 3**
IGEDUC - PREFEITURA DE BELO JARDIM – 2024
Os sistemas operacionais modernos são projetados com arquiteturas
complexas que incluem sistemas de gerenciamento de memória,
sistemas de arquivos e subsistemas de entrada e saída, fundamentais
para o funcionamento eficiente dos computadores.

- [x] Verdadeiro
- [ ] Falso

**Questao 4**
FGV - PREFEITURA DE VITÓRIA – 2024
Um sistema operacional é um conjunto de programas de software que
gerenciam os recursos de hardware e fornecem serviços comuns para os
aplicativos de software. Ele atua como uma interface entre o hardware do
computador e os programas do usuário. Não são exemplos das funcionalidades
implementadas por um sistema operacional:

- [ ] Gerência de processos e gerência de memória.
- [ ] Gerência de dispositivos e gerência de arquivos.
- [X] Gerência de conteúdo e gerência de edição de texto.
- [ ] Gerência de proteção e gerência de memória.
- [ ] Gerência de arquivos e gerência de proteção.

**Questao 5**
FURB - CÂMARA DE GASPAR – 2024
Considere as afirmativas relacionadas aos sistemas operacionais apresentadas a
seguir. Registre V, para verdadeiras, e F, para falsas:

- [X] Windows, Linux e Android são exemplos de sistemas operacionais.
- [X] sistema operacional oferece um meio para que o programa seja carregado
  na memória principal e seja executado.
- [ ] sistema operacional se refere ao hardware que gerencia e administra
  recursos de sistema, bem como componentes de hardware.
- [X] sistema operacional se refere ao software que gerencia e administra
  recursos de sistema, bem como componentes de hardware.

______________________________________________________________________

## Leituras recomendadas (PDF)

- Tanenbaum, *Sistemas Operacionais Modernos*: secoes 1.0, 1.1, 1.5, 1.6
- Se precisar de base: secao 1.3 (Arquitetura de Computdores)
