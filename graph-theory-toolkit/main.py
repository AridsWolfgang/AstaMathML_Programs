import os
import sys

from graph import AdjList, AdjMatrix
from traversal import dfs_recursive, dfs_iterative, bfs, topological_sort_kahn, topological_sort_dfs
from shortest_path import dijkstra, bellman_ford, floyd_warshall, reconstruct_path
from mst import kruskal, prim
from flow import ford_fulkerson, build_flow_graph
from data.datasets import (
    sample_graph_small,
    sample_graph_medium,
    sample_digraph,
    sample_flow_network,
    sample_disconnected_graph,
    sample_graph_mst,
    sample_graph_negative,
)


def clear():
    os.system("cls" if os.name == "nt" else "clear")


def pause():
    input("\nPress Enter to continue...")


class App:
    def __init__(self):
        self.graph = AdjList()
        self.graph_name = "Empty graph"

    def load_sample(self, idx):
        samples = [
            sample_graph_small,
            sample_graph_medium,
            sample_digraph,
            sample_flow_network,
            sample_disconnected_graph,
            sample_graph_mst,
            sample_graph_negative,
        ]
        if 0 <= idx < len(samples):
            adj, name = samples[idx]()
            self.graph = AdjList(adj)
            self.graph_name = name
            print(f"\nLoaded: {name}")

    def menu_create(self):
        clear()
        print("=" * 60)
        print("             CREATE / MANAGE GRAPHS")
        print("=" * 60)
        print("1. Load sample graph")
        print("2. Add vertex")
        print("3. Add edge (directed)")
        print("4. Add edge (undirected)")
        print("5. Remove vertex")
        print("6. Remove edge")
        print("7. Clear graph")
        print("8. Convert to adjacency matrix view")
        print("9. Back")
        choice = input("\nChoice: ").strip()

        if choice == "1":
            print("\nSample graphs:")
            samples = [
                "Small graph (5 nodes, weighted)",
                "Medium graph (7 nodes, weighted)",
                "DAG (for topological sort)",
                "Flow network",
                "Disconnected graph",
                "MST test graph",
                "Negative weight graph",
            ]
            for i, name in enumerate(samples):
                print(f"  {i + 1}. {name}")
            s = input("Select sample: ").strip()
            if s.isdigit():
                self.load_sample(int(s) - 1)
        elif choice == "2":
            v = input("Vertex name: ").strip()
            self.graph.add_vertex(v)
            print(f"Added vertex '{v}'")
        elif choice == "3":
            u = input("From: ").strip()
            v = input("To: ").strip()
            w = float(input("Weight (default 1): ") or 1)
            self.graph.add_edge(u, v, w)
            print(f"Added directed edge {u} -> {v} (weight={w})")
        elif choice == "4":
            u = input("Vertex 1: ").strip()
            v = input("Vertex 2: ").strip()
            w = float(input("Weight (default 1): ") or 1)
            self.graph.add_undirected_edge(u, v, w)
            print(f"Added undirected edge {u} - {v} (weight={w})")
        elif choice == "5":
            v = input("Vertex to remove: ").strip()
            self.graph.remove_vertex(v)
            print(f"Removed vertex '{v}'")
        elif choice == "6":
            u = input("From: ").strip()
            v = input("To: ").strip()
            self.graph.remove_edge(u, v)
            print(f"Removed edge {u} -> {v}")
        elif choice == "7":
            self.graph = AdjList()
            self.graph_name = "Empty graph"
            print("Graph cleared")
        elif choice == "8":
            self.print_matrix_view()
        pause()

    def print_matrix_view(self):
        if not self.graph.vertices():
            print("Graph is empty")
            return
        mat = AdjMatrix(self.graph.vertices())
        for v in self.graph.vertices():
            for nbr, w in self.graph.get_neighbors(v):
                mat.add_edge(v, nbr, w)
        print("\n" + str(mat))

    def menu_traversal(self):
        clear()
        print("=" * 60)
        print("               GRAPH TRAVERSALS")
        print("=" * 60)
        print(str(self.graph))
        if not self.graph.vertices():
            print("\nGraph is empty. Load or create a graph first.")
            pause()
            return
        start = input("\nStart vertex: ").strip()
        if start not in self.graph.vertices():
            print(f"Vertex '{start}' not found")
            pause()
            return

        print("\n--- DFS (Recursive) ---")
        print(" -> ".join(dfs_recursive(self.graph, start)))
        print("\n--- DFS (Iterative) ---")
        print(" -> ".join(dfs_iterative(self.graph, start)))
        print("\n--- BFS ---")
        print(" -> ".join(bfs(self.graph, start)))

        print("\n--- Topological Sort ---")
        ts_kahn = topological_sort_kahn(self.graph)
        ts_dfs = topological_sort_dfs(self.graph)
        if ts_kahn is None:
            print("Kahn: Graph has a cycle (no topological order)")
        else:
            print(f"Kahn: {' -> '.join(ts_kahn)}")
        print(f"DFS:  {' -> '.join(ts_dfs)}")

        pause()

    def menu_shortest_path(self):
        clear()
        print("=" * 60)
        print("               SHORTEST PATH DEMO")
        print("=" * 60)
        print(str(self.graph))
        if not self.graph.vertices():
            print("\nGraph is empty.")
            pause()
            return
        start = input("Start vertex: ").strip()
        if start not in self.graph.vertices():
            print(f"Vertex '{start}' not found")
            pause()
            return
        end = input("End vertex (Enter for all): ").strip()

        print("\n--- Dijkstra ---")
        try:
            dist, pred = dijkstra(self.graph, start)
            if end and end in dist:
                path = reconstruct_path(pred, start, end)
                print(f"Distance to {end}: {dist[end]}")
                print(f"Path: {' -> '.join(path) if path else 'none'}")
            else:
                for v, d in sorted(dist.items(), key=lambda x: x[1]):
                    path = reconstruct_path(pred, start, v)
                    p = " -> ".join(path) if path else "none"
                    print(f"  {start} -> {v}: {d}  [{p}]")
        except Exception as e:
            print(f"Dijkstra error: {e}")

        print("\n--- Bellman-Ford ---")
        try:
            dist, pred = bellman_ford(self.graph, start)
            if end and end in dist:
                path = reconstruct_path(pred, start, end)
                print(f"Distance to {end}: {dist[end]}")
                print(f"Path: {' -> '.join(path) if path else 'none'}")
            else:
                for v, d in sorted(dist.items(), key=lambda x: x[1]):
                    path = reconstruct_path(pred, start, v)
                    p = " -> ".join(path) if path else "none"
                    print(f"  {start} -> {v}: {d}  [{p}]")
        except ValueError as e:
            print(f"Bellman-Ford: {e}")

        print("\n--- Floyd-Warshall (all pairs) ---")
        try:
            fw = floyd_warshall(self.graph)
            if end and end in fw.get(start, {}):
                print(f"{start} -> {end}: {fw[start][end]}")
            else:
                for v in sorted(fw.keys(), key=str):
                    for u in sorted(fw[v].keys(), key=str):
                        if v != u and fw[v][u] != float("inf"):
                            print(f"  {v} -> {u}: {fw[v][u]}")
        except Exception as e:
            print(f"Floyd-Warshall error: {e}")

        pause()

    def menu_mst(self):
        clear()
        print("=" * 60)
        print("               MINIMUM SPANNING TREE DEMO")
        print("=" * 60)
        print(str(self.graph))
        if not self.graph.vertices():
            print("\nGraph is empty.")
            pause()
            return

        print("\n--- Kruskal's MST ---")
        mst_k = kruskal(self.graph)
        total_k = sum(w for _, _, w in mst_k)
        for u, v, w in mst_k:
            print(f"  {u} - {v}: {w}")
        print(f"Total weight: {total_k}")

        print("\n--- Prim's MST ---")
        mst_p = prim(self.graph)
        total_p = sum(w for _, _, w in mst_p)
        for u, v, w in mst_p:
            print(f"  {u} - {v}: {w}")
        print(f"Total weight: {total_p}")

        pause()

    def menu_flow(self):
        clear()
        print("=" * 60)
        print("               MAX FLOW DEMO")
        print("=" * 60)
        print(str(self.graph))
        if not self.graph.vertices():
            print("\nGraph is empty.")
            pause()
            return

        source = input("Source vertex: ").strip()
        sink = input("Sink vertex: ").strip()

        try:
            adj = {}
            for v in self.graph.vertices():
                adj[v] = [(n, int(w)) for n, w in self.graph.get_neighbors(v) if w == int(w) or w == float(w)]
            adj = {v: [(n, max(1, int(w))) for n, w in self.graph.get_neighbors(v)] for v in self.graph.vertices()}
            adj_dict = {}
            for v in self.graph.vertices():
                adj_dict[v] = []
                for n, w in self.graph.get_neighbors(v):
                    cap = int(w) if w == int(w) else max(1, int(w))
                    adj_dict[v].append((n, cap))

            fg = build_flow_graph(adj_dict)
            max_flow, flow_dict = ford_fulkerson(fg, source, sink)
            print(f"\nMax flow from {source} to {sink}: {max_flow}")
            print("\nFlow on edges:")
            for (u, v), flow in sorted(flow_dict.items()):
                cap = dict(fg.capacity).get((u, v), 0) + flow
                print(f"  {u} -> {v}: {flow}/{cap}")
        except Exception as e:
            print(f"Error: {e}")

        pause()

    def run(self):
        while True:
            clear()
            print("=" * 60)
            print("              GRAPH THEORY TOOLKIT")
            print("=" * 60)
            print(f"Current: {self.graph_name}")
            vert_count = len(self.graph.vertices())
            edge_count = self.graph.edge_count()
            print(f"Vertices: {vert_count} | Edges: {edge_count}")
            print()
            print("1. Create / Manage Graphs")
            print("2. Run Traversals (DFS, BFS, Topological Sort)")
            print("3. Shortest Path Demo (Dijkstra, Bellman-Ford, Floyd-Warshall)")
            print("4. MST Demo (Kruskal, Prim)")
            print("5. Max Flow Demo (Ford-Fulkerson)")
            print("6. Print Graph")
            print("7. Exit")
            choice = input("\nSelect option: ").strip()

            if choice == "1":
                self.menu_create()
            elif choice == "2":
                self.menu_traversal()
            elif choice == "3":
                self.menu_shortest_path()
            elif choice == "4":
                self.menu_mst()
            elif choice == "5":
                self.menu_flow()
            elif choice == "6":
                clear()
                print(str(self.graph))
                pause()
            elif choice == "7":
                print("\nGoodbye!")
                break


if __name__ == "__main__":
    app = App()
    app.run()
