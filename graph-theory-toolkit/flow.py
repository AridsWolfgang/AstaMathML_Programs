from collections import deque


class FlowGraph:
    """Flow network for Ford-Fulkerson / Edmonds-Karp.

    Stores both forward and reverse edges for residual graph.
    """

    def __init__(self):
        self.adj = {}
        self.capacity = {}

    def add_vertex(self, v):
        if v not in self.adj:
            self.adj[v] = []

    def add_edge(self, u, v, cap):
        self.add_vertex(u)
        self.add_vertex(v)
        self.adj[u].append(v)
        self.adj[v].append(u)
        self.capacity[(u, v)] = self.capacity.get((u, v), 0) + cap
        if (v, u) not in self.capacity:
            self.capacity[(v, u)] = 0

    def vertices(self):
        return list(self.adj.keys())


def bfs_residual(flow_graph, source, sink, parent):
    """BFS to find augmenting path in residual graph (Edmonds-Karp)."""
    visited = {source}
    queue = deque([source])
    while queue:
        u = queue.popleft()
        for v in flow_graph.adj[u]:
            if v not in visited and flow_graph.capacity.get((u, v), 0) > 0:
                visited.add(v)
                parent[v] = u
                if v == sink:
                    return True
                queue.append(v)
    return False


def ford_fulkerson(flow_graph, source, sink):
    """Ford-Fulkerson max flow algorithm with BFS (Edmonds-Karp).

    Returns (max_flow_value, flow_dict) where flow_dict maps (u, v) -> flow.
    Time: O(V * E^2) for Edmonds-Karp.
    """
    max_flow = 0
    parent = {}

    while bfs_residual(flow_graph, source, sink, parent):
        path_flow = float("inf")
        v = sink
        while v != source:
            u = parent[v]
            path_flow = min(path_flow, flow_graph.capacity[(u, v)])
            v = u

        v = sink
        while v != source:
            u = parent[v]
            flow_graph.capacity[(u, v)] -= path_flow
            flow_graph.capacity[(v, u)] += path_flow
            v = u

        max_flow += path_flow
        parent.clear()

    flow_dict = {}
    for (u, v), orig_cap in flow_graph.capacity.items():
        current_cap = flow_graph.capacity[(u, v)]
        flow = orig_cap - current_cap
        if flow > 0:
            flow_dict[(u, v)] = flow

    return max_flow, flow_dict


def build_flow_graph(adj_dict):
    """Build a FlowGraph from a dictionary adjacency list."""
    fg = FlowGraph()
    for u in adj_dict:
        fg.add_vertex(u)
        for v, cap in adj_dict[u]:
            fg.add_edge(u, v, int(cap))
    return fg
