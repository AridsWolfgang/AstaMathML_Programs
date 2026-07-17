"""
=====================================================================
 MODULE 6: GRAPH ALGORITHMS  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - GPS Navigation: Shortest path (Dijkstra)
  - Social Networks: Friend-of-friend (BFS)
  - Web Crawling: Page discovery (BFS/DFS)
  - Network Routing: OSPF protocol (Dijkstra)
  - AI: Game tree search (DFS)

In Python, we use lists as adjacency lists and implement
our own queue/stack classes for educational purposes.

COMPLEXITIES:
  BFS/DFS:    O(V + E) time, O(V) space
  Dijkstra:   O((V + E) log V) with priority queue
=====================================================================
"""

import heapq


# =================================================================
#  GRAPH REPRESENTATION
# =================================================================
#
#  Test graph (unweighted):
#       0 --- 1
#      / \     \
#     2 - 3 --- 4
#      \ /
#       5
# =================================================================
def build_sample_graph():
    """Build an unweighted graph as adjacency list."""
    g = {
        0: [1, 2],
        1: [0, 4],
        2: [0, 3, 5],
        3: [2, 4, 5],
        4: [1, 3],
        5: [2, 3],
    }
    return g


def print_graph(g):
    for v in sorted(g):
        print(f"    {v}: {g[v]}")


# =================================================================
#  QUEUE (for BFS)
# =================================================================
class Queue:
    """Simple queue using a list (FIFO)."""
    def __init__(self):
        self._data = []

    def enqueue(self, val):
        self._data.append(val)

    def dequeue(self):
        if not self._data:
            return None
        return self._data.pop(0)

    def is_empty(self):
        return len(self._data) == 0


# =================================================================
#  BFS
# =================================================================
#
#  Uses a queue. Visits nodes level by level.
#  Finds the SHORTEST path in unweighted graphs.
#
#  REAL-WORLD: GPS (fewest streets), social (fewest hops).
# =================================================================
def bfs(g, start):
    visited = set()
    q = Queue()
    order = []

    visited.add(start)
    q.enqueue(start)

    print(f"  BFS traversal from vertex {start}:\n")

    while not q.is_empty():
        v = q.dequeue()
        order.append(v)

        print(f"  Visit {v} -> neighbors: {g[v]}")

        for neighbor in g[v]:
            if neighbor not in visited:
                visited.add(neighbor)
                q.enqueue(neighbor)
                print(f"       Enqueue {neighbor}")

    print(f"\n  BFS order: {order}")
    print("  Lesson: BFS finds the SHORTEST path in unweighted graphs.\n")


# =================================================================
#  STACK (for DFS)
# =================================================================
class Stack:
    """Simple stack using a list (LIFO)."""
    def __init__(self):
        self._data = []

    def push(self, val):
        self._data.append(val)

    def pop(self):
        if not self._data:
            return None
        return self._data.pop()

    def is_empty(self):
        return len(self._data) == 0


# =================================================================
#  DFS
# =================================================================
#
#  Uses a stack. Goes deep first, then backtracks.
#
#  REAL-WORLD: Maze solving, cycle detection, topological sort.
# =================================================================
def dfs(g, start):
    visited = set()
    s = Stack()
    order = []

    s.push(start)

    print(f"  DFS traversal from vertex {start}:\n")

    while not s.is_empty():
        v = s.pop()

        if v in visited:
            continue

        visited.add(v)
        order.append(v)

        print(f"  Visit {v} -> neighbors: {g[v]}")

        # Push in reverse order for natural exploration
        for neighbor in reversed(g[v]):
            if neighbor not in visited:
                s.push(neighbor)
                print(f"       Push {neighbor} onto stack")

    print(f"\n  DFS order: {order}")
    print("  Lesson: DFS uses less memory on wide graphs.\n")


# =================================================================
#  DIJKSTRA'S SHORTEST PATH
# =================================================================
#
#  Like BFS, but for weighted graphs. Uses a priority queue.
#
#  Weighted test graph:
#          2
#     0 ----- 1
#     |       |
#    4|       |3
#     |       |
#     2 --1-- 3 --1-- 4
#      \      |
#      2\     |2
#         \   |
#           5
#
#  REAL-WORLD: Google Maps, network routing (OSPF).
# =================================================================
def build_weighted_graph():
    """Build a weighted graph as adjacency list of (neighbor, weight)."""
    g = {
        0: [(1, 2), (2, 4)],
        1: [(0, 2), (3, 3)],
        2: [(0, 4), (3, 1), (5, 2)],
        3: [(1, 3), (2, 1), (4, 1), (5, 2)],
        4: [(3, 1)],
        5: [(2, 2), (3, 2)],
    }
    return g


def print_weighted_graph(g):
    for v in sorted(g):
        edges = ", ".join(f"({n}, w={w})" for n, w in g[v])
        print(f"    {v}: {edges}")


def dijkstra(g, start):
    INF = float('inf')
    dist = {v: INF for v in g}
    prev = {v: None for v in g}
    dist[start] = 0

    # Priority queue: (distance, vertex)
    pq = [(0, start)]

    print(f"  Dijkstra from vertex {start}:\n")

    while pq:
        d, v = heapq.heappop(pq)

        if d > dist[v]:
            continue  # Skip outdated entries

        print(f"  Visit {v} (distance={d})")
        print(f"    Neighbors: ", end="")

        for neighbor, weight in g[v]:
            new_dist = d + weight
            print(f"({neighbor}, w={weight}) ", end="")

            if new_dist < dist[neighbor]:
                dist[neighbor] = new_dist
                prev[neighbor] = v
                heapq.heappush(pq, (new_dist, neighbor))
                print(f"<< updated dist[{neighbor}]={new_dist}", end="")

            print()
            print(f"             ", end="")

        print()

    # Print results
    print(f"  Final distances from {start}:")
    for v in sorted(g):
        if dist[v] == INF:
            print(f"    {start} -> {v}: unreachable")
        else:
            # Reconstruct path
            path = []
            curr = v
            while curr is not None:
                path.append(curr)
                curr = prev[curr]
            path.reverse()
            print(f"    {start} -> {v}: {dist[v]}  (path: {' -> '.join(map(str, path))})")

    print("\n  Lesson: Dijkstra = BFS for weighted graphs.")
    print("     Uses a priority queue to always expand the closest node.\n")


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_graph_module():
    print("\n  --- Graph Algorithms ---")
    print("  1) BFS")
    print("  2) DFS")
    print("  3) Dijkstra's Shortest Path")
    print("  4) All")
    choice = input("  Enter choice: ").strip()

    print()

    if choice in ("1", "4"):
        print("  >> BFS")
        print("  Graph (adjacency list):")
        g = build_sample_graph()
        print_graph(g)
        print()
        bfs(g, 0)

    if choice in ("2", "4"):
        print("  >> DFS")
        print("  Graph (adjacency list):")
        g = build_sample_graph()
        print_graph(g)
        print()
        dfs(g, 0)

    if choice in ("3", "4"):
        print("  >> DIJKSTRA'S SHORTEST PATH")
        print("  Weighted graph:")
        g = build_weighted_graph()
        print_weighted_graph(g)
        print()
        dijkstra(g, 0)

    input("  Press Enter to return to menu...")
