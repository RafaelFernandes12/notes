# metodo de busca I

______________________________________________________________________

**Date:** 2026-09-10
**Tags:** [Python.md](tags/Python.md), [Grafo.md](tags/Grafo.md)
**URL:** [01-metodos-de-busca-i.html](https://oandrefonseca.github.io/fundamentos-de-ia/capitulos/01-metodos-de-busca-i.html)


______________________________________________________________________

## BFS

O BFS (breadt first search ou buscar em largura) é um algoritmo de busca em grafo, ele funciona percorrendo os nós de um grafo a partir de um nó inicial, visitando primeiro todos os vizinhos do nó inicial, e então visitando os vizinhos dos vizinhos até achar o objetivo. Ele usa uma estrutura do tipo FIFO (primeiro a entrar, primeiro a sair), isso garante que os nós mais recem-gerados (mais profundos) vão para o final da fila, e os nós mais antigos e rasos sejam expandidos primeiro.

- Ele é útil para encontrar o caminho mais curto entre dois nós.


```py

import matplotlib.pyplot as plt
import networkx as nx

environment = {
    "Natal": {
        "Parnamirim": 1,
        "Extremoz": 1,
        "São Gonçalo do Amarante": 1,
        "Macaíba": 1,
    },
    "Parnamirim": {"Natal": 1, "São José de Mipibu": 1, "Macaíba": 1},
    "Extremoz": {"Natal": 1},
    "São Gonçalo do Amarante": {"Natal": 1, "Macaíba": 1, "Ceará-Mirim": 1},
    "Macaíba": {
        "Natal": 1,
        "Parnamirim": 1,
        "São Gonçalo do Amarante": 1,
        "Ielmo Marinho": 1,
        "Vera Cruz": 1,
    },
    "Ceará-Mirim": {"São Gonçalo do Amarante": 1},
    "Ielmo Marinho": {"Macaíba": 1},
    "Vera Cruz": {"Macaíba": 1, "Monte Alegre": 1},
    "Monte Alegre": {"Vera Cruz": 1, "São José de Mipibu": 1},
    "São José de Mipibu": {"Parnamirim": 1, "Goianinha": 1, "Monte Alegre": 1},
    "Goianinha": {"São José de Mipibu": 1, "Tibau do Sul": 1},
    "Tibau do Sul": {"Goianinha": 1, "Pipa": 1},
    "Pipa": {"Tibau do Sul": 1},
}

graph = environment
estado_inicial = "Natal"
objetivo = "Pipa"

def breadth_first_search(graph, start, goal, return_expansion_order=False):
    """Busca em largura: expande o caminho mais raso primeiro (fila FIFO)."""
    if start == goal:
        return ([start], [start]) if return_expansion_order else [start]

    frontier = [[start]]
    visited = {start}
    expansion_order = []

    while frontier:
        path = frontier.pop(0)
        current = path[-1]
        expansion_order.append(current)

        for neighbor in graph.get(current, {}):
            if neighbor in visited:
                continue
            new_path = path + [neighbor]
            if neighbor == goal:
                expansion_order.append(neighbor)
                return (
                    (new_path, expansion_order) if return_expansion_order else new_path
                )
            visited.add(neighbor)
            frontier.append(new_path)

    return (None, expansion_order) if return_expansion_order else None


resultado_bfs, expansoes_bfs = breadth_first_search(
    graph, estado_inicial, objetivo, return_expansion_order=True
)
print("Caminho encontrado (BFS):", resultado_bfs)
print("Ordem de expansão BFS:", expansoes_bfs)
# Caminho encontrado (BFS): ['Natal', 'Parnamirim', 'São José de Mipibu', 'Goianinha', 'Tibau do Sul', 'Pipa']
# Ordem de expansão BFS: ['Natal', 'Parnamirim', 'Extremoz', 'São Gonçalo do Amarante', 'Macaíba', 'São José de Mipibu', 'Ceará-Mirim', 'Ielmo Marinho', 'Vera Cruz', 'Goianinha', 'Monte Alegre', 'Tibau do Sul', 'Pipa']
```

## DFS
O deep first search (dfs) é parecido com o [Breadth-First Search](src/bfs%20python.md), porém, enquanto o bfs procura por camadas, passando por um vizinho por ves, o dfs mergulha o mais fundo possível num nó antes de voltar, ele usa uma estrutura do tipo pilha (ultimo a entrar, primeiro a sair).

- O dfs nao garante o caminho mais curto
- Usa pouca mémoria
- Pode ficar preso em espaços de estados infinitos

```py
import matplotlib.pyplot as plt
import networkx as nx

environment = {
    "Natal": {
        "Parnamirim": 1,
        "Extremoz": 1,
        "São Gonçalo do Amarante": 1,
        "Macaíba": 1,
    },
    "Parnamirim": {"Natal": 1, "São José de Mipibu": 1, "Macaíba": 1},
    "Extremoz": {"Natal": 1},
    "São Gonçalo do Amarante": {"Natal": 1, "Macaíba": 1, "Ceará-Mirim": 1},
    "Macaíba": {
        "Natal": 1,
        "Parnamirim": 1,
        "São Gonçalo do Amarante": 1,
        "Ielmo Marinho": 1,
        "Vera Cruz": 1,
    },
    "Ceará-Mirim": {"São Gonçalo do Amarante": 1},
    "Ielmo Marinho": {"Macaíba": 1},
    "Vera Cruz": {"Macaíba": 1, "Monte Alegre": 1},
    "Monte Alegre": {"Vera Cruz": 1, "São José de Mipibu": 1},
    "São José de Mipibu": {"Parnamirim": 1, "Goianinha": 1, "Monte Alegre": 1},
    "Goianinha": {"São José de Mipibu": 1, "Tibau do Sul": 1},
    "Tibau do Sul": {"Goianinha": 1, "Pipa": 1},
    "Pipa": {"Tibau do Sul": 1},
}

graph = environment
estado_inicial = "Natal"
objetivo = "Pipa"

def depth_first_search(graph, start, goal, return_expansion_order=False):
    """Busca em profundidade: expande o caminho mais recente primeiro (pilha)."""
    frontier = [[start]]
    visited = set()
    expansion_order = []

    while frontier:
        path = frontier.pop()
        current = path[-1]
        expansion_order.append(current)

        if current == goal:
            return (path, expansion_order) if return_expansion_order else path

        if current in visited:
            continue
        visited.add(current)

        for neighbor in graph.get(current, {}):
            if neighbor not in visited:
                frontier.append(path + [neighbor])

    return (None, expansion_order) if return_expansion_order else None


resultado_dfs, expansoes_dfs = depth_first_search(
    graph, estado_inicial, objetivo, return_expansion_order=True
)
print("Caminho encontrado (DFS):", resultado_dfs)
print("Ordem de expansão DFS:", expansoes_dfs)
# Caminho encontrado (DFS): ['Natal', 'Macaíba', 'Vera Cruz', 'Monte Alegre', 'São José de Mipibu', 'Goianinha', 'Tibau do Sul', 'Pipa']
# Ordem de expansão DFS: ['Natal', 'Macaíba', 'Vera Cruz', 'Monte Alegre', 'São José de Mipibu', 'Goianinha', 'Tibau do Sul', 'Pipa']


```

