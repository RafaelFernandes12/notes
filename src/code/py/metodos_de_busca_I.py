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

G = nx.DiGraph()
for origem, destinos in environment.items():
    for destino in destinos:
        G.add_edge(origem, destino)

pos = nx.spring_layout(G, seed=7)

plt.figure(figsize=(10, 6))
nx.draw(
    G,
    pos,
    with_labels=True,
    node_color="#d9e8fb",
    node_size=1800,
    # arrows=True,
    font_size=11,
    edgecolors="black",
)
plt.title("Estrutura do grafo do ambiente")
plt.savefig("grafo.png")


def path_cost(graph, path):
    """Soma o custo das arestas percorridas em um caminho."""
    return sum(graph[a][b] for a, b in zip(path, path[1:]))


graph = environment
estado_inicial = "Natal"
objetivo = "Pipa"

# print("Estado inicial:", estado_inicial)
# print("Objetivo:", objetivo)
# print("Ações possíveis em Natal:", list(graph[estado_inicial].keys()))


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


# resultado_bfs, expansoes_bfs = breadth_first_search(
#     graph, estado_inicial, objetivo, return_expansion_order=True
# )
# print("Caminho encontrado (BFS):", resultado_bfs)
# print("Ordem de expansão BFS:", expansoes_bfs)


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


# resultado_dfs, expansoes_dfs = depth_first_search(
#     graph, estado_inicial, objetivo, return_expansion_order=True
# )
#
# print("Caminho encontrado (DFS):", resultado_dfs)
# print("Ordem de expansão DFS:", expansoes_dfs)


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


# resultado_ufs, expansoes_ufs = uniform_cost_search(
#     graph, estado_inicial, objetivo, return_expansion_order=True
# )
# print("Caminho encontrado (UFS):", resultado_ufs)
# print("Ordem de expansão UFS:", expansoes_ufs)


def depth_limited_search(graph, start, goal, limit=2):
    """Busca em profundidade sem expandir caminhos além da profundidade `limit`."""
    # Comece com o caminho inicial na fronteira.
    frontier = [[start]]

    while frontier:
        path = frontier.pop()
        current = path[-1]

        # Verifique se o objetivo foi encontrado.
        if current == goal:
            return path

        # Expanda o caminho apenas se ele ainda estiver dentro do limite.
        if len(path) - 1 < limit:
            for neighbor in graph.get(current, {}):
                frontier.append(path + [neighbor])

    return None


resultado_dls = depth_limited_search(graph, estado_inicial, objetivo)
print("Caminho encontrado (DLS):", resultado_dls)


def iterative_deepening_search(graph, start, goal, max_limit=10):
    """Busca em profundidade iterativa: chama depth_limited_search com limites crescentes."""
    for limit in range(max_limit + 1):
        result = depth_limited_search(graph, start, goal, limit)
        if result is not None:
            return result

    return None


resultado_ids = iterative_deepening_search(graph, estado_inicial, objetivo)
print("Caminho encontrado (IDS):", resultado_ids)
