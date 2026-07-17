/**
 * ===================================================================
 *  MODULE 6: GRAPH ALGORITHMS
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - GPS Navigation: Shortest path (Dijkstra's algorithm)
 *   - Social Networks: Friend recommendations (BFS for "friends of friends")
 *   - Web Crawling: BFS/DFS to discover and index web pages
 *   - Puzzle Solving: BFS for shortest sequence of moves (Rubik's cube)
 *   - Network Routing: OSPF protocol uses Dijkstra internally
 *   - Dependency Resolution: DFS for topological sorting (package managers)
 *   - AI Search: Game tree search (DFS with pruning)
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - Graph representation: adjacency list vs. adjacency matrix
 *   - BFS: level-by-level traversal (queue-based) — finds shortest paths
 *   - DFS: depth-first exploration (stack/recursion-based)
 *   - Dijkstra: BFS for weighted graphs — finds shortest paths with weights
 *   - The difference between tree traversal and graph traversal (cycles!)
 *
 * ⏱ COMPLEXITIES:
 *   BFS/DFS:          O(V + E) time, O(V) space    (V=vertices, E=edges)
 *   Dijkstra:         O((V + E) log V) with priority queue
 * ===================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>

void runGraphModule();

// ==================================================================
//  GRAPH REPRESENTATION
// ==================================================================
//
//  We'll use an ADJACENCY LIST: for each vertex, store a list of neighbors.
//
//  Visual representation of our test graph:
//
//       0 --- 1
//      /       \
//     2 --- 3 -- 4
//      \    |
//       \   |
//         5
//
//  Adjacency list:
//   0: {1, 2}
//   1: {0, 4}
//   2: {0, 3, 5}
//   3: {2, 4, 5}
//   4: {1, 3}
//   5: {2, 3}
// ==================================================================

using Graph = std::vector<std::vector<int>>;

Graph buildSampleGraph()
{
    Graph g(6);
    g[0] = {1, 2};
    g[1] = {0, 4};
    g[2] = {0, 3, 5};
    g[3] = {2, 4, 5};
    g[4] = {1, 3};
    g[5] = {2, 3};
    return g;
}

void printGraph(const Graph& g)
{
    for (size_t v = 0; v < g.size(); ++v) {
        std::cout << "    " << v << ": {";
        for (size_t i = 0; i < g[v].size(); ++i) {
            std::cout << g[v][i];
            if (i < g[v].size() - 1) std::cout << ", ";
        }
        std::cout << "}\n";
    }
}

// ==================================================================
//  BFS (BREADTH-FIRST SEARCH)
// ==================================================================
//
//  HOW IT WORKS:
//   Use a QUEUE. Visit a node, then enqueue all unvisited neighbors.
//   Process neighbors in FIFO order — level by level.
//
//  VISUAL:
//   BFS from vertex 0:
//
//   Queue: [0]
//   Visit 0 → enqueue 1, 2                    Queue: [1, 2]
//   Visit 1 → enqueue 4                       Queue: [2, 4]
//   Visit 2 → enqueue 3, 5                    Queue: [4, 3, 5]
//   Visit 4 → (all visited)                   Queue: [3, 5]
//   Visit 3 → (all visited)                   Queue: [5]
//   Visit 5 → (all visited)                   Queue: []
//
//   Order: 0, 1, 2, 4, 3, 5
//
//  REAL-WORLD ANALOGY:
//   Ripples in a pond — you explore in expanding circles.
//   The first time you reach a node is via the SHORTEST path.
// ==================================================================

void bfs(const Graph& g, int start)
{
    int n = static_cast<int>(g.size());
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    std::vector<int> order;

    visited[start] = true;
    q.push(start);

    std::cout << "  BFS traversal from vertex " << start << ":\n\n";

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);

        std::cout << "  Visit " << v << " → neighbors: {";
        for (size_t i = 0; i < g[v].size(); ++i) {
            int neighbor = g[v][i];
            std::cout << neighbor;
            if (i < g[v].size() - 1) std::cout << ", ";
        }
        std::cout << "}\n";

        for (int neighbor : g[v]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                std::cout << "       Enqueue " << neighbor << "\n";
            }
        }
    }

    std::cout << "\n  BFS order: ";
    for (int v : order) std::cout << v << " ";
    std::cout << "\n";
    std::cout << "  📝 BFS finds the SHORTEST path in unweighted graphs.\n";
    std::cout << "     Used in: GPS (fewest streets), social networks (fewest hops).\n\n";
}

// ==================================================================
//  DFS (DEPTH-FIRST SEARCH)
// ==================================================================
//
//  HOW IT WORKS:
//   Use a STACK (or recursion). Visit a node, then immediately
//   explore one neighbor deeply before backtracking.
//
//  VISUAL:
//   DFS from vertex 0:
//
//   Stack: [0]
//   Visit 0 → push 1, 2                      Stack: [1, 2]
//   Visit 2 → push 0(visited), 3, 5          Stack: [1, 3, 5]
//   Visit 5 → push 2(visited), 3(visited)    Stack: [1, 3]
//   Visit 3 → push 2(visited), 4             Stack: [1, 4]
//   Visit 4 → push 1(visited), 3(visited)    Stack: [1]
//   Visit 1 → push 0(visited), 4(visited)    Stack: []
//
//   Order: 0, 2, 5, 3, 4, 1
//
//  REAL-WORLD ANALOGY:
//   Exploring a maze — go as deep as you can, then backtrack.
//   If you hit a dead end, go back to the last junction and try another path.
// ==================================================================

void dfs(const Graph& g, int start)
{
    int n = static_cast<int>(g.size());
    std::vector<bool> visited(n, false);
    std::stack<int> s;
    std::vector<int> order;

    s.push(start);

    std::cout << "  DFS traversal from vertex " << start << ":\n\n";

    while (!s.empty()) {
        int v = s.top();
        s.pop();

        if (visited[v]) continue;  // Already visited (could be in stack multiple times)

        visited[v] = true;
        order.push_back(v);

        std::cout << "  Visit " << v << " → neighbors: {";
        for (size_t i = 0; i < g[v].size(); ++i) {
            std::cout << g[v][i];
            if (i < g[v].size() - 1) std::cout << ", ";
        }
        std::cout << "}\n";

        // Push neighbors in reverse order so they're explored in natural order
        for (int i = static_cast<int>(g[v].size()) - 1; i >= 0; --i) {
            int neighbor = g[v][i];
            if (!visited[neighbor]) {
                s.push(neighbor);
                std::cout << "       Push " << neighbor << " onto stack\n";
            }
        }
    }

    std::cout << "\n  DFS order: ";
    for (int v : order) std::cout << v << " ";
    std::cout << "\n";
    std::cout << "  📝 DFS uses less memory on wide graphs.\n";
    std::cout << "     Used in: maze solving, cycle detection, topological sort.\n\n";
}

// ==================================================================
//  DIJKSTRA'S SHORTEST PATH
// ==================================================================
//
//  PROBLEM:
//   Find the shortest path from a start vertex to all other vertices
//   in a weighted graph (non-negative weights).
//
//  HOW IT WORKS:
//   1. Set all distances to INFINITY, start distance = 0
//   2. Use a priority queue to always explore the CLOSEST unvisited vertex
//   3. For each neighbor, if going through current vertex is shorter, UPDATE
//
//  VISUAL (for our weighted graph):
//
//          2
//     0 ───── 1
//     |       |
//    4|       |3
//     |       |
//     2 ──1── 3 ──1── 4
//      \      |
//      2\     |2
//         \   |
//           5
//
//  Distances from 0:
//    0: 0    1: 2    2: 4    3: 5    4: 5    5: 6
//
//  REAL-WORLD ANALOGY:
//   Google Maps — among all possible routes, find the one with
//   the shortest total distance (edge weights = road lengths).
// ==================================================================

using WeightedGraph = std::vector<std::vector<std::pair<int, int>>>;

WeightedGraph buildWeightedGraph()
{
    WeightedGraph g(6);
    g[0] = {{1, 2}, {2, 4}};
    g[1] = {{0, 2}, {3, 3}};
    g[2] = {{0, 4}, {3, 1}, {5, 2}};
    g[3] = {{1, 3}, {2, 1}, {4, 1}, {5, 2}};
    g[4] = {{3, 1}};
    g[5] = {{2, 2}, {3, 2}};
    return g;
}

void dijkstra(const WeightedGraph& g, int start)
{
    const int INF = std::numeric_limits<int>::max();
    int n = static_cast<int>(g.size());

    // Distance from start to each vertex
    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);  // To reconstruct paths

    // Priority queue: (-distance, vertex) — negative for min-heap
    // or use std::greater. We'll use a simple pair and sort approach.
    using Pair = std::pair<int, int>;  // (distance, vertex)
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;

    dist[start] = 0;
    pq.push({0, start});

    std::cout << "  Dijkstra from vertex " << start << ":\n\n";

    while (!pq.empty()) {
        int d = pq.top().first;
        int v = pq.top().second;
        pq.pop();

        // Skip if we already found a better path
        if (d > dist[v]) continue;

        std::cout << "  Visit " << v << " (distance=" << d << ")\n";
        std::cout << "    Neighbors: ";

        for (auto& edge : g[v]) {
            int neighbor = edge.first;
            int weight = edge.second;
            int newDist = d + weight;

            std::cout << "(" << neighbor << ", w=" << weight << ") ";

            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                prev[neighbor] = v;
                pq.push({newDist, neighbor});
                std::cout << "← updated dist[" << neighbor << "]=" << newDist;
            }
            std::cout << "\n             ";
        }
        std::cout << "\n";
    }

    // Print results
    std::cout << "  Final distances from " << start << ":\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "    " << start << " → " << i << ": ";
        if (dist[i] == INF) {
            std::cout << "unreachable\n";
        } else {
            std::cout << dist[i] << "  (path: ";
            // Reconstruct path
            std::vector<int> path;
            for (int v = i; v != -1; v = prev[v])
                path.push_back(v);
            std::reverse(path.begin(), path.end());
            for (size_t p = 0; p < path.size(); ++p) {
                std::cout << path[p];
                if (p < path.size() - 1) std::cout << " → ";
            }
            std::cout << ")\n";
        }
    }

    std::cout << "\n  📝 Dijkstra = BFS for weighted graphs.\n";
    std::cout << "     Used in: Google Maps, network routing (OSPF), AI pathfinding.\n\n";
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runGraphModule()
{
    int choice;

    std::cout << "\n  --- Graph Algorithms ---\n";
    std::cout << "  1) BFS (Breadth-First Search)\n";
    std::cout << "  2) DFS (Depth-First Search)\n";
    std::cout << "  3) Dijkstra's Shortest Path\n";
    std::cout << "  4) All\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        std::cout << "  ▶ BFS\n";
        std::cout << "  Graph (adjacency list):\n";
        Graph g = buildSampleGraph();
        printGraph(g);
        std::cout << "\n";
        bfs(g, 0);
    }

    if (choice == 2 || choice == 4) {
        std::cout << "  ▶ DFS\n";
        std::cout << "  Graph (adjacency list):\n";
        Graph g = buildSampleGraph();
        printGraph(g);
        std::cout << "\n";
        dfs(g, 0);
    }

    if (choice == 3 || choice == 4) {
        std::cout << "  ▶ DIJKSTRA'S SHORTEST PATH\n";
        std::cout << "  Weighted graph:\n";
        WeightedGraph g = buildWeightedGraph();
        for (size_t v = 0; v < g.size(); ++v) {
            std::cout << "    " << v << ": ";
            for (auto& edge : g[v])
                std::cout << "(" << edge.first << ", w=" << edge.second << ") ";
            std::cout << "\n";
        }
        std::cout << "\n";
        dijkstra(g, 0);
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
