import heapq


class UnionFind:
    """Union-Find (Disjoint Set Union) for Kruskal's algorithm."""

    def __init__(self, vertices):
        self.parent = {v: v for v in vertices}
        self.rank = {v: 0 for v in vertices}

    def find(self, v):
        while self.parent[v] != v:
            self.parent[v] = self.parent[self.parent[v]]
            v = self.parent[v]
        return v

    def union(self, a, b):
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        if self.rank[ra] < self.rank[rb]:
            self.parent[ra] = rb
        elif self.rank[ra] > self.rank[rb]:
            self.parent[rb] = ra
        else:
            self.parent[rb] = ra
            self.rank[ra] += 1
        return True


def kruskal(graph):
    """Kruskal's algorithm for Minimum Spanning Tree.

    1. Sort all edges by weight
    2. Add edges in order if they don't create a cycle (using Union-Find)
    Returns list of edges in MST: [(u, v, weight), ...]
    Time: O(E log E).
    """
    edges = []
    for v in graph.vertices():
        for nbr, w in graph.get_neighbors(v):
            if v < nbr:
                edges.append((w, v, nbr))
    edges.sort(key=lambda x: x[0])

    uf = UnionFind(graph.vertices())
    mst = []

    for w, u, v in edges:
        if uf.union(u, v):
            mst.append((u, v, w))

    return mst


def prim(graph, start=None):
    """Prim's algorithm for Minimum Spanning Tree.

    Grows the MST by always adding the cheapest edge connecting the tree to a vertex outside it.
    Uses a binary heap.
    Returns list of edges in MST: [(u, v, weight), ...]
    Time: O((V + E) log V).
    """
    vertices = graph.vertices()
    if not vertices:
        return []

    if start is None:
        start = vertices[0]

    visited = {start}
    edges = []

    pq = []
    for nbr, w in graph.get_neighbors(start):
        heapq.heappush(pq, (w, start, nbr))

    while pq and len(visited) < len(vertices):
        w, u, v = heapq.heappop(pq)
        if v in visited:
            continue
        visited.add(v)
        edges.append((u, v, w))
        for nbr, w2 in graph.get_neighbors(v):
            if nbr not in visited:
                heapq.heappush(pq, (w2, v, nbr))

    return edges
