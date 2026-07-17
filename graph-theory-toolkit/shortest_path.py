import heapq
import math


def dijkstra(graph, start):
    """Dijkstra's algorithm for shortest paths from start.

    Uses a binary heap (priority queue) for efficiency.
    Returns (distances, predecessors).
    Time: O((V + E) log V) with binary heap.
    """
    distances = {v: float("inf") for v in graph.vertices()}
    predecessors = {v: None for v in graph.vertices()}
    distances[start] = 0

    pq = [(0, start)]
    visited = set()

    while pq:
        dist, v = heapq.heappop(pq)
        if v in visited:
            continue
        visited.add(v)

        for nbr, weight in graph.get_neighbors(v):
            new_dist = dist + weight
            if new_dist < distances[nbr]:
                distances[nbr] = new_dist
                predecessors[nbr] = v
                heapq.heappush(pq, (new_dist, nbr))

    return distances, predecessors


def bellman_ford(graph, start):
    """Bellman-Ford algorithm for shortest paths (handles negative weights).

    Detects negative cycles.
    Returns (distances, predecessors) or raises ValueError if negative cycle found.
    Time: O(V * E).
    """
    vertices = graph.vertices()
    distances = {v: float("inf") for v in vertices}
    predecessors = {v: None for v in vertices}
    distances[start] = 0

    for _ in range(len(vertices) - 1):
        updated = False
        for v in vertices:
            if distances[v] == float("inf"):
                continue
            for nbr, weight in graph.get_neighbors(v):
                if distances[v] + weight < distances[nbr]:
                    distances[nbr] = distances[v] + weight
                    predecessors[nbr] = v
                    updated = True
        if not updated:
            break

    for v in vertices:
        if distances[v] == float("inf"):
            continue
        for nbr, weight in graph.get_neighbors(v):
            if distances[v] + weight < distances[nbr]:
                raise ValueError("Graph contains a negative-weight cycle")

    return distances, predecessors


def floyd_warshall(graph):
    """Floyd-Warshall algorithm for all-pairs shortest paths.

    Returns dict of dict: dist[u][v] = shortest distance.
    Time: O(V^3).
    """
    vertices = graph.vertices()
    n = len(vertices)
    idx = {v: i for i, v in enumerate(vertices)}
    dist = [[float("inf")] * n for _ in range(n)]

    for i in range(n):
        dist[i][i] = 0

    for v in vertices:
        i = idx[v]
        for nbr, w in graph.get_neighbors(v):
            j = idx[nbr]
            dist[i][j] = w

    for k in range(n):
        for i in range(n):
            for j in range(n):
                if dist[i][k] + dist[k][j] < dist[i][j]:
                    dist[i][j] = dist[i][k] + dist[k][j]

    result = {}
    for i, v in enumerate(vertices):
        result[v] = {}
        for j, u in enumerate(vertices):
            result[v][u] = dist[i][j]

    return result


def reconstruct_path(predecessors, start, end):
    """Reconstruct path from predecessors dict."""
    path = []
    v = end
    while v is not None:
        path.append(v)
        v = predecessors.get(v)
    path.reverse()
    if path[0] != start:
        return None
    return path
