# Graph Theory Toolkit

An interactive Python toolkit for graph theory algorithms — adjacency lists/matrices, graph traversals, shortest paths, minimum spanning trees, and maximum flow — built entirely from scratch using only the Python standard library.

---

## Table of Contents

1. [Graph Representations](#1-graph-representations)
2. [Graph Traversals](#2-graph-traversals)
   - [Depth-First Search (DFS)](#depth-first-search-dfs)
   - [Breadth-First Search (BFS)](#breadth-first-search-bfs)
   - [Topological Sort](#topological-sort)
3. [Shortest Path Algorithms](#3-shortest-path-algorithms)
   - [Dijkstra's Algorithm](#dijkstras-algorithm)
   - [Bellman-Ford Algorithm](#bellman-ford-algorithm)
   - [Floyd-Warshall Algorithm](#floyd-warshall-algorithm)
4. [Minimum Spanning Tree](#4-minimum-spanning-tree)
   - [Kruskal's Algorithm](#kruskals-algorithm)
   - [Prim's Algorithm](#prims-algorithm)
5. [Maximum Flow](#5-maximum-flow)
   - [Ford-Fulkerson / Edmonds-Karp](#ford-fulkerson--edmonds-karp)
6. [File Reference](#6-file-reference)

---

## 1. Graph Representations

### Adjacency List

Each vertex stores a list of its neighbors (and optionally edge weights). Space-efficient for sparse graphs.

**Example:**
```
A: [(B, 4), (C, 2)]
B: [(A, 4), (C, 1), (D, 5)]
```

- Space: O(V + E)
- Edge lookup: O(deg(v))
- Adding a vertex: O(1)
- Adding an edge: O(1)

### Adjacency Matrix

A V×V matrix where entry (i, j) = weight of edge from i to j (∞ if no edge). Best for dense graphs.

**Example:**
```
     A   B   C
A    0   4   2
B    4   0   1
C    2   1   0
```

- Space: O(V²)
- Edge lookup: O(1)
- Adding a vertex: O(V) (need to resize)
- Adding an edge: O(1)

---

## 2. Graph Traversals

### Depth-First Search (DFS)

Explores as far as possible along each branch before backtracking.

**Algorithm (iterative):**
1. Push start vertex onto a stack
2. While stack is not empty:
   - Pop vertex v
   - If v not visited, mark visited and push all neighbors

**Time:** O(V + E) — visits each vertex and edge once

**Applications:** Connected components, cycle detection, topological sort, maze solving

### Breadth-First Search (BFS)

Explores all vertices at the current depth before moving deeper.

**Algorithm:**
1. Enqueue start vertex
2. While queue is not empty:
   - Dequeue vertex v
   - For each unvisited neighbor, mark visited and enqueue

**Time:** O(V + E)

**Applications:** Shortest path in unweighted graphs, web crawling, social networks

### Topological Sort

Orders vertices of a Directed Acyclic Graph (DAG) so that for every edge u → v, u appears before v.

**Kahn's Algorithm (BFS-based):**
1. Compute in-degree for each vertex
2. Queue all vertices with in-degree 0
3. While queue not empty:
   - Dequeue v, add to result
   - Decrease in-degree of neighbors; queue any that reach 0
4. If result size ≠ V, graph has a cycle

**DFS-based:**
1. Run DFS; when a vertex's neighbors are exhausted, push to stack
2. Reverse the stack

**Time:** O(V + E)

**Applications:** Task scheduling, dependency resolution, build systems

---

## 3. Shortest Path Algorithms

### Dijkstra's Algorithm

Finds shortest paths from a single source to all other vertices in a graph with **non-negative** edge weights.

**Algorithm:**
1. Set distance[source] = 0, all others = ∞
2. Use a priority queue keyed by distance
3. While queue not empty:
   - Extract vertex v with smallest distance
   - For each neighbor u, if distance[v] + weight(v,u) < distance[u], update distance[u]

Greedy strategy: once a vertex is extracted, its shortest distance is finalized.

**Time:** O((V + E) log V) with binary heap

**Limitation:** Fails with negative edge weights

### Bellman-Ford Algorithm

Finds shortest paths from a single source, handling **negative edge weights** and detecting negative cycles.

**Algorithm:**
1. Set distance[source] = 0, all others = ∞
2. Repeat V - 1 times:
   - Relax every edge: if distance[u] + w(u,v) < distance[v], update
3. Check for negative cycles: if any edge can still be relaxed, a negative cycle exists

**Time:** O(V · E)

**Applications:** Graphs with negative weights, currency arbitrage detection

### Floyd-Warshall Algorithm

Finds shortest paths between **all pairs** of vertices.

**Algorithm:**
1. Initialize dist[i][j] = weight(i,j) (or ∞ if no edge, 0 if i=j)
2. For each k from 1 to V:
   - For each i from 1 to V:
     - For each j from 1 to V:
       - dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

**Time:** O(V³)

**Space:** O(V²)

**Applications:** All-pairs shortest paths, transitive closure, routing tables

---

## 4. Minimum Spanning Tree (MST)

A spanning tree that connects all vertices with the minimum total edge weight.

### Kruskal's Algorithm

Greedy algorithm that adds edges in increasing order of weight, skipping edges that create cycles.

**Algorithm:**
1. Sort all edges by weight
2. Initialize Union-Find (Disjoint Set Union) data structure
3. For each edge (u, v) in sorted order:
   - If find(u) ≠ find(v), add edge to MST and union(u, v)

**Union-Find (Disjoint Set Union):**
- **find(x):** Returns the representative (root) of x's set
- **union(x, y):** Merges the sets containing x and y
- With path compression and union by rank: O(α(V)) per operation

**Time:** O(E log E) (dominated by sorting)

### Prim's Algorithm

Grows the MST from a starting vertex, always adding the cheapest edge connecting the tree to a vertex outside it.

**Algorithm:**
1. Start with an arbitrary vertex, mark as visited
2. Add all its edges to a priority queue
3. While queue not empty:
   - Extract the cheapest edge (u, v)
   - If v not visited, add to MST, mark visited, add v's edges to queue

**Time:** O((V + E) log V) with binary heap

**Comparison:**
| Kruskal | Prim |
|---------|------|
| Edge-based | Vertex-based |
| Good for sparse graphs | Good for dense graphs |
| O(E log E) | O((V+E) log V) |
| Uses Union-Find | Uses Priority Queue |

---

## 5. Maximum Flow

### Ford-Fulkerson / Edmonds-Karp

Finds the maximum flow from a source to a sink in a flow network.

**Key concepts:**
- **Flow network:** Directed graph with edge capacities
- **Residual graph:** Shows remaining capacity and back-flow capacity
- **Augmenting path:** Path from source to sink with positive residual capacity

**Algorithm (Edmonds-Karp = Ford-Fulkerson with BFS):**
1. Initialize flow = 0 on all edges
2. While there exists an augmenting path in the residual graph:
   - Find path using BFS (Edmonds-Karp ensures shortest augmenting path)
   - Find bottleneck capacity on the path
   - Update residual capacities (forward: subtract, backward: add)
   - Add bottleneck to max flow

**Time:** O(V · E²) for Edmonds-Karp

**Applications:** Network routing, bipartite matching (via reduction), image segmentation

**Max-Flow Min-Cut Theorem:**
The maximum flow value equals the capacity of the minimum cut (a set of edges whose removal disconnects source from sink).

---

## 6. File Reference

| File | Contents |
|------|----------|
| `main.py` | Interactive menu system |
| `graph.py` | AdjList and AdjMatrix classes |
| `traversal.py` | DFS (recursive/iterative), BFS, topological sort (Kahn/DFS) |
| `shortest_path.py` | Dijkstra, Bellman-Ford, Floyd-Warshall |
| `mst.py` | Kruskal (Union-Find), Prim |
| `flow.py` | FlowGraph, Ford-Fulkerson (Edmonds-Karp) |
| `data/datasets.py` | Sample graphs for testing |
| `README.md` | This lecture document |

---

*All code uses only the Python standard library.*
