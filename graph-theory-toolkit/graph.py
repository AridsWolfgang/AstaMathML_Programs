class AdjList:
    """Adjacency list graph representation.

    Edge format: (neighbor, weight).
    For unweighted graphs, weight = 1.
    """

    def __init__(self, adj_dict=None):
        self.adj = {}
        self.directed = False
        if adj_dict:
            for v in adj_dict:
                self.adj[v] = list(adj_dict[v])

    def add_vertex(self, v):
        if v not in self.adj:
            self.adj[v] = []

    def add_edge(self, u, v, weight=1):
        self.add_vertex(u)
        self.add_vertex(v)
        self.adj[u].append((v, weight))

    def add_undirected_edge(self, u, v, weight=1):
        self.add_edge(u, v, weight)
        self.add_edge(v, u, weight)

    def remove_vertex(self, v):
        if v in self.adj:
            del self.adj[v]
        for u in self.adj:
            self.adj[u] = [(w, wt) for w, wt in self.adj[u] if w != v]

    def remove_edge(self, u, v):
        if u in self.adj:
            self.adj[u] = [(w, wt) for w, wt in self.adj[u] if w != v]

    def get_neighbors(self, v):
        return self.adj.get(v, [])

    def vertices(self):
        return list(self.adj.keys())

    def edge_count(self):
        return sum(len(nbrs) for nbrs in self.adj.values())

    def __str__(self):
        lines = ["Adjacency List:"]
        for v in sorted(self.adj.keys(), key=str):
            edges_str = ", ".join(
                f"{n}({w})" for n, w in self.adj[v]
            )
            lines.append(f"  {v}: [{edges_str}]")
        return "\n".join(lines)


class AdjMatrix:
    """Adjacency matrix graph representation."""

    def __init__(self, vertices=None):
        self.vertices = list(vertices) if vertices else []
        self.index = {v: i for i, v in enumerate(self.vertices)}
        n = len(self.vertices)
        self.matrix = [[float("inf")] * n for _ in range(n)]
        for i in range(n):
            self.matrix[i][i] = 0

    def _ensure(self, v):
        if v not in self.index:
            self.vertices.append(v)
            self.index[v] = len(self.vertices) - 1
            n = len(self.vertices)
            for row in self.matrix:
                row.append(float("inf"))
            self.matrix.append([float("inf")] * n)
            self.matrix[-1][-1] = 0

    def add_edge(self, u, v, weight=1):
        self._ensure(u)
        self._ensure(v)
        i, j = self.index[u], self.index[v]
        self.matrix[i][j] = weight

    def add_undirected_edge(self, u, v, weight=1):
        self.add_edge(u, v, weight)
        self.add_edge(v, u, weight)

    def get_weight(self, u, v):
        if u in self.index and v in self.index:
            return self.matrix[self.index[u]][self.index[v]]
        return float("inf")

    def get_neighbors(self, v):
        if v not in self.index:
            return []
        i = self.index[v]
        nbrs = []
        for j, w in enumerate(self.matrix[i]):
            if j != i and w != float("inf"):
                nbrs.append((self.vertices[j], w))
        return nbrs

    def vertices_list(self):
        return list(self.vertices)

    def __str__(self):
        if not self.vertices:
            return "Empty matrix"
        header = "     " + "  ".join(f"{v:>4}" for v in self.vertices)
        lines = [header]
        for v in self.vertices:
            i = self.index[v]
            row_vals = []
            for w in self.matrix[i]:
                if w == float("inf"):
                    row_vals.append("  ∞")
                else:
                    row_vals.append(f"{w:4.0f}" if w == int(w) else f"{w:4.1f}")
            lines.append(f"{v:>4} " + "".join(row_vals))
        return "\n".join(lines)
