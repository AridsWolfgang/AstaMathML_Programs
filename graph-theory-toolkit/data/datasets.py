# Sample graphs for testing algorithms


def sample_graph_small():
    """Small undirected graph with 5 nodes."""
    g = {
        "A": [("B", 4), ("C", 2)],
        "B": [("A", 4), ("C", 1), ("D", 5)],
        "C": [("A", 2), ("B", 1), ("D", 8), ("E", 10)],
        "D": [("B", 5), ("C", 8), ("E", 2)],
        "E": [("C", 10), ("D", 2)],
    }
    return g, "Small Graph (5 nodes, weighted, undirected)"


def sample_graph_medium():
    """Medium graph with 7 nodes."""
    g = {
        "A": [("B", 3), ("C", 6)],
        "B": [("A", 3), ("C", 2), ("D", 4)],
        "C": [("A", 6), ("B", 2), ("D", 1), ("E", 5)],
        "D": [("B", 4), ("C", 1), ("E", 7), ("F", 8)],
        "E": [("C", 5), ("D", 7), ("F", 3), ("G", 9)],
        "F": [("D", 8), ("E", 3), ("G", 2)],
        "G": [("E", 9), ("F", 2)],
    }
    return g, "Medium Graph (7 nodes, weighted, undirected)"


def sample_digraph():
    """Directed graph for topological sort."""
    g = {
        "A": [("B", 1), ("C", 1)],
        "B": [("D", 1), ("E", 1)],
        "C": [("E", 1)],
        "D": [("F", 1)],
        "E": [("F", 1)],
        "F": [],
    }
    return g, "DAG (6 nodes, for topological sort)"


def sample_flow_network():
    """Flow network with source 'S' and sink 'T'."""
    g = {
        "S": [("A", 10), ("B", 5)],
        "A": [("B", 15), ("D", 9)],
        "B": [("C", 8), ("D", 4)],
        "C": [("T", 10)],
        "D": [("C", 15), ("T", 10)],
        "T": [],
    }
    return g, "Flow Network (S->T, capacities shown)"


def sample_disconnected_graph():
    """Disconnected graph (two components)."""
    g = {
        "A": [("B", 1)],
        "B": [("A", 1)],
        "C": [("D", 2)],
        "D": [("C", 2)],
        "E": [],
    }
    return g, "Disconnected Graph (3 components)"


def sample_graph_mst():
    """Graph specifically for MST testing (Kruskal/Prim)."""
    g = {
        0: [(1, 4), (2, 3)],
        1: [(0, 4), (2, 1), (3, 2)],
        2: [(0, 3), (1, 1), (3, 5)],
        3: [(1, 2), (2, 5)],
    }
    return g, "MST Test Graph (4 nodes, weighted)"


def sample_graph_negative():
    """Graph with negative edges (for Bellman-Ford)."""
    g = {
        "A": [("B", 4), ("C", 2)],
        "B": [("C", -3), ("D", 2)],
        "C": [("D", 3)],
        "D": [],
    }
    return g, "Graph with negative weights"
