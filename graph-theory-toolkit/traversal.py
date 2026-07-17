from collections import deque


def dfs_recursive(graph, start, visited=None):
    """Depth-First Search (recursive). Returns list of visited vertices."""
    if visited is None:
        visited = []
    visited.append(start)
    for nbr, _ in graph.get_neighbors(start):
        if nbr not in visited:
            dfs_recursive(graph, nbr, visited)
    return visited


def dfs_iterative(graph, start):
    """Depth-First Search (iterative with stack)."""
    visited = []
    stack = [start]
    while stack:
        v = stack.pop()
        if v not in visited:
            visited.append(v)
            for nbr, _ in reversed(graph.get_neighbors(v)):
                stack.append(nbr)
    return visited


def bfs(graph, start):
    """Breadth-First Search. Returns list of visited vertices."""
    visited = []
    queue = deque([start])
    visited.append(start)
    while queue:
        v = queue.popleft()
        for nbr, _ in graph.get_neighbors(v):
            if nbr not in visited:
                visited.append(nbr)
                queue.append(nbr)
    return visited


def topological_sort_kahn(graph):
    """Topological sort using Kahn's algorithm (BFS-based)."""
    in_degree = {}
    for v in graph.vertices():
        in_degree[v] = 0
    for v in graph.vertices():
        for nbr, _ in graph.get_neighbors(v):
            in_degree[nbr] = in_degree.get(nbr, 0) + 1

    queue = deque([v for v, d in in_degree.items() if d == 0])
    result = []
    while queue:
        v = queue.popleft()
        result.append(v)
        for nbr, _ in graph.get_neighbors(v):
            in_degree[nbr] -= 1
            if in_degree[nbr] == 0:
                queue.append(nbr)

    if len(result) != len(graph.vertices()):
        return None  # cycle detected
    return result


def topological_sort_dfs(graph):
    """Topological sort using DFS-based approach."""
    visited = set()
    stack = []

    def _dfs(v):
        visited.add(v)
        for nbr, _ in graph.get_neighbors(v):
            if nbr not in visited:
                _dfs(nbr)
        stack.append(v)

    for v in graph.vertices():
        if v not in visited:
            _dfs(v)
    return list(reversed(stack))
