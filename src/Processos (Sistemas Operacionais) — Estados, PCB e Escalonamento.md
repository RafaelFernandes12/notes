# Processos (Sistemas Operacionais) — Estados, PCB e Escalonamento

______________________________________________________________________

**Date:** 2026-03-11
**Tags:** [Uf](../tags/Uf.md), [SistemasOperacionais](../tags/SistemasOperacionais.md)
**URL:**

- https://drive.google.com/file/d/1FJNa3kgI9aYeoKQ_Nvzx7m2e1wSDE8QT/view
- https://www.youtube.com/watch?v=7WSJTTySb_U&list=PLlfjpAXy4GaE3FeHp6KgPFjt6ShUJ3_1n&index=2

______________________________________________________________________

## O que é um processo?

**Processo** = instância de um **programa em execução**.

- Programa é entidade *passiva* (arquivo executável + instruções).
- Processo é entidade *ativa* (programa carregado e executando com estado).

Analogia do PDF: **classe vs objetos** (programa vs processos).

### O que um processo “tem”

- identificador (PID)
- entrada/saída
- estado
- espaço de endereçamento
- “CPU virtual” (conceitualmente)

## Tipos e hierarquia de processos

- **Primeiro plano**: interage com usuário.
- **Segundo plano**: executa tarefas específicas sem interação direta.

**Hierarquia (UNIX)**: pai cria filho; sinais (ex.: teclado) podem ir para o grupo.

Windows (segundo o PDF): sem o mesmo conceito de hierarquia de processos.

## Pseudoparalelismo e multiprogramação

Mesmo em CPU única, o SO alterna processos rapidamente → impressão de execução paralela (**pseudoparalelismo**).

### Utilização da CPU x grau de multiprogramação

Se:

- `p` = fração de tempo que um processo fica esperando E/S
- `n` = número de processos na memória

Então:

```text
Utilização da CPU = 1 - p^n
```

Exemplos rápidos:

- `p=0.5`, `n=1` → `U=1-0.5=0.5` (50%)
- `p=0.5`, `n=4` → `U=1-0.5^4=1-0.0625=0.9375` (93.75%)

Ideia: mais processos prontos enquanto outros esperam E/S → mais CPU ocupada.

## Estados de um processo

Estados clássicos:

- **Em execução (running)**: usando a CPU.
- **Pronto (ready)**: apto a executar, esperando CPU.
- **Bloqueado (blocked/waiting)**: esperando evento externo (E/S, sinal, etc.).

ASCII (mental model):

```text
ready --(escalonador despacha)--> running
running --(preempção / fim quantum)--> ready
running --(espera E/S)--> blocked
blocked --(evento/E/S completa)--> ready
```

## PCB (Process Control Block)

Quando o SO alterna processos (context switch), ele precisa salvar/carregar contexto.

O **PCB** costuma armazenar:

- Identificador (PID)
- Estado
- Prioridade
- Contador de programa (endereço da próxima instrução)
- Ponteiros de memória (faixa do processo na memória)
- Contexto de CPU (registradores)
- Estado de E/S (requisições pendentes, dispositivos associados)

## Criação e término de processos

### Criação (eventos típicos)

- inicialização do sistema
- processo cria outro
- solicitação do usuário
- tarefa em lote

### Chamadas clássicas

- UNIX: `fork()`
- Windows: `CreateProcess()`

Exemplo mínimo (C / UNIX):

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
  pid_t pid = fork();
  if (pid == 0) {
    printf("child\n");
  } else {
    printf("parent (child pid=%d)\n", pid);
  }
  return 0;
}
```

### Término

- saída normal: `exit()` (Unix), `ExitProcess()` (Windows)
- erro fatal / erro
- morte por outro processo: `kill()` (Unix), `TerminateProcess()` (Windows)

## Escalonamento de processos

Objetivo da **multiprogramação**: sempre ter algum processo em execução para otimizar CPU.

O **escalonador** decide quem entra/usa a CPU.

### Filas

- fila de **prontos** (processos na memória esperando CPU)
- filas de **dispositivos** (processos esperando E/S; cada dispositivo pode ter sua fila)

### Tipos de escalonadores

- **Longo prazo**: decide o que entra na memória (admissão de processos)
- **Curto prazo**: escolhe o próximo processo a executar (dispatcher)
- **Médio prazo**: remove/reintroduz processos (swap) sob sobrecarga

## Questões de fixação (ideias-chave)

- Processo = programa em execução.
- Pseudoparalelismo é a “simulação” via alternância rápida.
- Recursos de processo: CPU, memória, E/S, arquivos… (barramento não é “recurso do processo”, é infraestrutura).
- Modos usuário vs privilegiado: SO roda privilegiado; instruções de E/S são restritas.
