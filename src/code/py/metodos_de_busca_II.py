import sys

import pandas as pd

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

heuristics = {
    "Ceará-Mirim": 76,
    "Extremoz": 64,
    "Goianinha": 18,
    "Ielmo Marinho": 71,
    "Macaíba": 52,
    "Monte Alegre": 35,
    "Natal": 45,
    "Parnamirim": 41,
    "Pipa": 0,
    "São Gonçalo do Amarante": 57,
    "São José de Mipibu": 27,
    "Tibau do Sul": 6,
    "Vera Cruz": 47,
}


def path_cost(graph, path):
    """Soma o custo das arestas percorridas em um caminho."""
    return sum(graph[a][b] for a, b in zip(path, path[1:]))


graph = environment
estado_inicial = "Natal"
objetivo = "Pipa"

print("Estado inicial:", estado_inicial)
print("Objetivo:", objetivo)
print("Ações possíveis em Natal:", list(graph[estado_inicial].keys()))


def greedy_best_first_search(
    graph, start, goal, heuristics, return_expansion_order=False
):
    """Busca gulosa: expande sempre o caminho cujo estado atual parece mais perto do objetivo (h(n))."""
    frontier = [[start]]
    visited = set()
    expansion_order = []

    def heuristica_do_caminho(path):
        """Retorna a heurística e o nome da última cidade do caminho, para ordenar a fronteira."""
        ultima_cidade = path[-1]
        return (heuristics[ultima_cidade], ultima_cidade)

    while frontier:
        frontier.sort(key=heuristica_do_caminho)
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


resultado_gbfs, expansoes_gbfs = greedy_best_first_search(
    graph, estado_inicial, objetivo, heuristics, return_expansion_order=True
)

print("Caminho encontrado (GBFS):", resultado_gbfs)
print("Ordem de expansão GBFS:", expansoes_gbfs)

environment_with_distance = {
    "Ceará-Mirim": {"Ielmo Marinho": 26, "São Gonçalo do Amarante": 21},
    "Extremoz": {"Natal": 25, "São Gonçalo do Amarante": 17},
    "Goianinha": {"São José de Mipibu": 23, "Tibau do Sul": 19},
    "Ielmo Marinho": {"Ceará-Mirim": 26, "Macaíba": 28},
    "Macaíba": {
        "Ielmo Marinho": 28,
        "Natal": 21,
        "Parnamirim": 16,
        "São Gonçalo do Amarante": 8,
        "Vera Cruz": 25,
    },
    "Monte Alegre": {"São José de Mipibu": 12, "Vera Cruz": 13},
    "Natal": {
        "Extremoz": 25,
        "Macaíba": 21,
        "Parnamirim": 13,
        "São Gonçalo do Amarante": 22,
    },
    "Parnamirim": {"Macaíba": 16, "Natal": 13, "São José de Mipibu": 19},
    "Pipa": {"Tibau do Sul": 9},
    "São Gonçalo do Amarante": {
        "Ceará-Mirim": 21,
        "Extremoz": 17,
        "Macaíba": 8,
        "Natal": 22,
    },
    "São José de Mipibu": {"Goianinha": 23, "Monte Alegre": 12, "Parnamirim": 19},
    "Tibau do Sul": {"Goianinha": 19, "Pipa": 9},
    "Vera Cruz": {"Macaíba": 25, "Monte Alegre": 13},
}

graph = environment_with_distance

print("Estado inicial:", estado_inicial)
print("Objetivo:", objetivo)
print("Ações possíveis em Natal:", list(graph[estado_inicial].keys()))


def astar_search(graph, start, goal, heuristics, return_expansion_order=False):
    """Busca A*: expande sempre o caminho de menor custo total estimado (g(n) + h(n))."""
    frontier = [[start]]
    visited = set()
    expansion_order = []
    trace = []

    while frontier:
        frontier.sort(
            key=lambda path: (path_cost(graph, path) + heuristics[path[-1]], path[-1])
        )
        path = frontier.pop(0)
        current = path[-1]

        if current in visited:
            continue
        visited.add(current)
        expansion_order.append(current)

        g = path_cost(graph, path)
        trace.append(
            {
                "estado": current,
                "g(n)": g,
                "h(n)": heuristics[current],
                "f(n)": g + heuristics[current],
            }
        )

        if current == goal:
            print(f"{'Estado':<24} {'g(n)':>6} {'h(n)':>6} {'f(n)':>6}")
            print("-" * 46)
            for row in trace:
                print(
                    f"{row['estado']:<24} {row['g(n)']:>6} {row['h(n)']:>6} {row['f(n)']:>6}"
                )
            return (path, expansion_order) if return_expansion_order else path

        for neighbor in graph.get(current, {}):
            if neighbor not in visited:
                frontier.append(path + [neighbor])

    return (None, expansion_order) if return_expansion_order else None


resultado_astar, expansoes_astar = astar_search(
    graph, estado_inicial, objetivo, heuristics, return_expansion_order=True
)

print("Caminho encontrado (A*):", resultado_astar)
print("Ordem de expansão A*:", expansoes_astar)
