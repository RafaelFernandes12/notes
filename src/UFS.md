# UFS

______________________________________________________________________

**Date:** 2026-09-10
**Tags:**
**URL:**

______________________________________________________________________


O UFS (Uniform Cost Search) é uma generalização do [bfs python](src/bfs%20python.md) para grafos com custos de áreas diferentes, em vez de expandir sempre o caminho mais raso, ele vai expandir para o caminho com custo o menor custo acumulado (g(n)), se todas as arestas custarem o mesmo como é o caso, ele vai produzir o mesmo resultado que o bfs


```py
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

def uniform_cost_search(graph, start, goal, return_expansion_order=False):
    """Busca de custo uniforme: expande sempre o caminho de menor custo acumulado."""
    frontier = [[start]]
    visited = set()
    expansion_order = []

    while frontier:
        frontier.sort(key=lambda path: (path_cost(graph, path), path[-1]))
        path = frontier.pop(0)
        current = path[-1]

        if current in visited:
            continue
        visited.add(current)
        expansion_order.append(current)

        if current == goal:
            return (path, expansion_order) if return_expansion_order else path

        for neighbor in graph.get(current, {}):
            if neighbor not in visited:
                frontier.append(path + [neighbor])

    return (None, expansion_order) if return_expansion_order else None


resultado_ufs, expansoes_ufs = uniform_cost_search(
    graph, estado_inicial, objetivo, return_expansion_order=True
)
print("Caminho encontrado (UFS):", resultado_ufs)
print("Ordem de expansão UFS:", expansoes_ufs)

# Caminho encontrado (UCS): ['Natal', 'Parnamirim', 'São José de Mipibu', 'Goianinha', 'Tibau do Sul', 'Pipa']
# Ordem de expansão UCS: ['Natal', 'Extremoz', 'Macaíba', 'Parnamirim', 'São Gonçalo do Amarante', 'Ceará-Mirim', 'Ielmo Marinho', 'São José de Mipibu', 'Vera Cruz', 'Goianinha', 'Monte Alegre', 'Tibau do Sul', 'Pipa']
```
