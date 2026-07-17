/*
 * ===================================================================
 *  MODULE 6: GRAPH ALGORITHMS  (C Edition)
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - GPS Navigation: Shortest path (Dijkstra)
 *    - Social Networks: Friend-of-friend (BFS)
 *    - Web Crawling: Page discovery (BFS/DFS)
 *    - Network Routing: OSPF protocol (Dijkstra)
 *    - AI: Game tree search (DFS)
 *    - Package Managers: Dependency resolution (topological sort via DFS)
 *
 *  In C, we implement EVERY data structure by hand:
 *    - Graph as adjacency list (array of linked lists or arrays)
 *    - Queue for BFS (circular buffer)
 *    - Stack for DFS (dynamic array)
 *    - Priority queue for Dijkstra (binary heap)
 *
 *  COMPLEXITIES:
 *    BFS/DFS:    O(V + E) time, O(V) space
 *    Dijkstra:   O((V + E) log V) with priority queue
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>   /* for INT_MAX */

void run_graph_module(void);
extern void clear_input(void);

/* ==================================================================
 *  GRAPH REPRESENTATION (Adjacency List)
 * ==================================================================
 *
 *  We store the graph as an array of edges per vertex.
 *  Each edge is a struct with (neighbor, weight).
 *
 *  Test graph (unweighted):
 *       0 --- 1
 *      / \     \
 *     2 - 3 --- 4
 *      \ /
 *       5
 *
 *  Adjacency list:
 *    0: {1, 2}
 *    1: {0, 4}
 *    2: {0, 3, 5}
 *    3: {2, 4, 5}
 *    4: {1, 3}
 *    5: {2, 3}
 * ================================================================== */

struct Edge {
    int vertex;
    int weight;  /* For weighted graph (Dijkstra) */
};

struct Graph {
    int num_vertices;
    struct Edge **adj_lists;  /* Array of edge arrays */
    int *sizes;               /* Current size of each adjacency list */
    int *capacities;          /* Capacity of each adjacency list */
};

/* Create a graph with n vertices */
static struct Graph* graph_create(int n)
{
    struct Graph *g = (struct Graph*)malloc(sizeof(struct Graph));
    if (!g) return NULL;

    g->num_vertices = n;
    g->adj_lists = (struct Edge**)calloc((size_t)n, sizeof(struct Edge*));
    g->sizes = (int*)calloc((size_t)n, sizeof(int));
    g->capacities = (int*)calloc((size_t)n, sizeof(int));

    if (!g->adj_lists || !g->sizes || !g->capacities) {
        free(g->adj_lists); free(g->sizes); free(g->capacities); free(g);
        return NULL;
    }

    /* Initialize each adjacency list */
    for (int i = 0; i < n; ++i) {
        g->capacities[i] = 4;  /* Start small, grow as needed */
        g->adj_lists[i] = (struct Edge*)malloc(
            (size_t)g->capacities[i] * sizeof(struct Edge));
        g->sizes[i] = 0;
    }

    return g;
}

/* Add a directed edge from u to v with given weight */
static void graph_add_edge(struct Graph *g, int u, int v, int weight)
{
    if (g->sizes[u] >= g->capacities[u]) {
        /* Grow the array */
        g->capacities[u] *= 2;
        g->adj_lists[u] = (struct Edge*)realloc(
            g->adj_lists[u],
            (size_t)g->capacities[u] * sizeof(struct Edge));
    }

    g->adj_lists[u][g->sizes[u]].vertex = v;
    g->adj_lists[u][g->sizes[u]].weight = weight;
    g->sizes[u]++;
}

/* Add undirected edge */
static void graph_add_undirected(struct Graph *g, int u, int v)
{
    graph_add_edge(g, u, v, 0);
    graph_add_edge(g, v, u, 0);
}

/* Free all memory */
static void graph_destroy(struct Graph *g)
{
    if (!g) return;
    for (int i = 0; i < g->num_vertices; ++i)
        free(g->adj_lists[i]);
    free(g->adj_lists);
    free(g->sizes);
    free(g->capacities);
    free(g);
}

static void graph_print(struct Graph *g)
{
    for (int v = 0; v < g->num_vertices; ++v) {
        printf("    %d: {", v);
        for (int i = 0; i < g->sizes[v]; ++i) {
            printf("%d", g->adj_lists[v][i].vertex);
            if (g->adj_lists[v][i].weight != 0)
                printf("(w=%d)", g->adj_lists[v][i].weight);
            if (i < g->sizes[v] - 1) printf(", ");
        }
        printf("}\n");
    }
}

/* ==================================================================
 *  QUEUE (for BFS)
 * ================================================================== */
struct Queue {
    int *data;
    int front;
    int rear;
    int size;
    int capacity;
};

static struct Queue* queue_create(int cap)
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->data = (int*)malloc((size_t)cap * sizeof(int));
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity = cap;
    return q;
}

static void queue_destroy(struct Queue *q)
{
    free(q->data);
    free(q);
}

static void queue_enqueue(struct Queue *q, int val)
{
    if (q->size >= q->capacity) return;
    q->data[q->rear] = val;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

static int queue_dequeue(struct Queue *q)
{
    if (q->size <= 0) return -1;
    int val = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return val;
}

static int queue_is_empty(struct Queue *q)
{
    return q->size == 0;
}

/* ==================================================================
 *  BFS
 * ==================================================================
 *
 *  Uses a queue. Visits nodes level by level (expanding circles).
 *  Finds the SHORTEST path in unweighted graphs.
 *
 *  REAL-WORLD: GPS (fewest streets), social (fewest hops).
 * ================================================================== */
static void bfs(struct Graph *g, int start)
{
    int n = g->num_vertices;
    int *visited = (int*)calloc((size_t)n, sizeof(int));
    struct Queue *q = queue_create(n);
    int *order = (int*)malloc((size_t)n * sizeof(int));
    int order_size = 0;

    visited[start] = 1;
    queue_enqueue(q, start);

    printf("  BFS traversal from vertex %d:\n\n", start);

    while (!queue_is_empty(q)) {
        int v = queue_dequeue(q);
        order[order_size++] = v;

        printf("  Visit %d -> neighbors: {", v);
        for (int i = 0; i < g->sizes[v]; ++i) {
            printf("%d", g->adj_lists[v][i].vertex);
            if (i < g->sizes[v] - 1) printf(", ");
        }
        printf("}\n");

        for (int i = 0; i < g->sizes[v]; ++i) {
            int neighbor = g->adj_lists[v][i].vertex;
            if (!visited[neighbor]) {
                visited[neighbor] = 1;
                queue_enqueue(q, neighbor);
                printf("       Enqueue %d\n", neighbor);
            }
        }
    }

    printf("\n  BFS order: ");
    for (int i = 0; i < order_size; ++i) printf("%d ", order[i]);
    printf("\n");
    printf("  Lesson: BFS finds the SHORTEST path in unweighted graphs.\n\n");

    free(visited);
    free(order);
    queue_destroy(q);
}

/* ==================================================================
 *  STACK (for DFS)
 * ================================================================== */
struct Stack {
    int *data;
    int top;
    int capacity;
};

static struct Stack* stack_create(int cap)
{
    struct Stack *s = (struct Stack*)malloc(sizeof(struct Stack));
    s->data = (int*)malloc((size_t)cap * sizeof(int));
    s->top = -1;
    s->capacity = cap;
    return s;
}

static void stack_destroy(struct Stack *s)
{
    free(s->data);
    free(s);
}

static void stack_push(struct Stack *s, int val)
{
    if (s->top >= s->capacity - 1) return;
    s->data[++s->top] = val;
}

static int stack_pop(struct Stack *s)
{
    if (s->top < 0) return -1;
    return s->data[s->top--];
}

static int stack_is_empty(struct Stack *s)
{
    return s->top < 0;
}

/* ==================================================================
 *  DFS
 * ==================================================================
 *
 *  Uses a stack. Goes as deep as possible, then backtracks.
 *  Uses less memory on wide graphs than BFS.
 *
 *  REAL-WORLD: Maze solving, cycle detection, topological sort.
 * ================================================================== */
static void dfs(struct Graph *g, int start)
{
    int n = g->num_vertices;
    int *visited = (int*)calloc((size_t)n, sizeof(int));
    struct Stack *s = stack_create(n);
    int *order = (int*)malloc((size_t)n * sizeof(int));
    int order_size = 0;

    stack_push(s, start);

    printf("  DFS traversal from vertex %d:\n\n", start);

    while (!stack_is_empty(s)) {
        int v = stack_pop(s);

        if (visited[v]) continue;

        visited[v] = 1;
        order[order_size++] = v;

        printf("  Visit %d -> neighbors: {", v);
        for (int i = 0; i < g->sizes[v]; ++i) {
            printf("%d", g->adj_lists[v][i].vertex);
            if (i < g->sizes[v] - 1) printf(", ");
        }
        printf("}\n");

        /* Push in reverse order for natural exploration order */
        for (int i = g->sizes[v] - 1; i >= 0; --i) {
            int neighbor = g->adj_lists[v][i].vertex;
            if (!visited[neighbor]) {
                stack_push(s, neighbor);
                printf("       Push %d onto stack\n", neighbor);
            }
        }
    }

    printf("\n  DFS order: ");
    for (int i = 0; i < order_size; ++i) printf("%d ", order[i]);
    printf("\n");
    printf("  Lesson: DFS uses less memory on wide graphs.\n\n");

    free(visited);
    free(order);
    stack_destroy(s);
}

/* ==================================================================
 *  PRIORITY QUEUE (Min-Heap for Dijkstra)
 * ==================================================================
 *
 *  Elements are (distance, vertex) pairs. The smallest distance
 *  is always at the top (index 1).
 *
 *  Heap property: parent.distance <= child.distance
 * ================================================================== */

struct PQNode {
    int distance;
    int vertex;
};

struct MinHeap {
    struct PQNode *nodes;
    int size;
    int capacity;
};

static struct MinHeap* heap_create(int cap)
{
    struct MinHeap *h = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    h->nodes = (struct PQNode*)malloc((size_t)(cap + 1) * sizeof(struct PQNode));
    h->size = 0;
    h->capacity = cap;
    return h;
}

static void heap_destroy(struct MinHeap *h)
{
    free(h->nodes);
    free(h);
}

static void heap_swap(struct PQNode *a, struct PQNode *b)
{
    struct PQNode tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(struct MinHeap *h, int distance, int vertex)
{
    if (h->size >= h->capacity) return;

    h->size++;
    int i = h->size;
    h->nodes[i].distance = distance;
    h->nodes[i].vertex = vertex;

    /* Bubble up: while parent is larger, swap */
    while (i > 1 && h->nodes[i / 2].distance > h->nodes[i].distance) {
        heap_swap(&h->nodes[i], &h->nodes[i / 2]);
        i /= 2;
    }
}

static struct PQNode heap_pop(struct MinHeap *h)
{
    struct PQNode min = h->nodes[1];

    /* Move last element to root */
    h->nodes[1] = h->nodes[h->size];
    h->size--;

    /* Bubble down */
    int i = 1;
    while (1) {
        int smallest = i;
        int left = 2 * i;
        int right = 2 * i + 1;

        if (left <= h->size && h->nodes[left].distance < h->nodes[smallest].distance)
            smallest = left;
        if (right <= h->size && h->nodes[right].distance < h->nodes[smallest].distance)
            smallest = right;

        if (smallest == i) break;

        heap_swap(&h->nodes[i], &h->nodes[smallest]);
        i = smallest;
    }

    return min;
}

static int heap_is_empty(struct MinHeap *h)
{
    return h->size == 0;
}

/* ==================================================================
 *  DIJKSTRA'S SHORTEST PATH
 * ==================================================================
 *
 *  Like BFS, but for weighted graphs. Uses a priority queue to
 *  always explore the CLOSEST unvisited vertex first.
 *
 *  REAL-WORLD: Google Maps, network routing (OSPF), AI pathfinding.
 * ================================================================== */

/* Build the weighted test graph */
static struct Graph* build_weighted_graph(void)
{
    struct Graph *g = graph_create(6);
    graph_add_edge(g, 0, 1, 2);  graph_add_edge(g, 1, 0, 2);
    graph_add_edge(g, 0, 2, 4);  graph_add_edge(g, 2, 0, 4);
    graph_add_edge(g, 1, 3, 3);  graph_add_edge(g, 3, 1, 3);
    graph_add_edge(g, 2, 3, 1);  graph_add_edge(g, 3, 2, 1);
    graph_add_edge(g, 2, 5, 2);  graph_add_edge(g, 5, 2, 2);
    graph_add_edge(g, 3, 4, 1);  graph_add_edge(g, 4, 3, 1);
    graph_add_edge(g, 3, 5, 2);  graph_add_edge(g, 5, 3, 2);
    return g;
}

static void dijkstra(struct Graph *g, int start)
{
    int n = g->num_vertices;
    int *dist = (int*)malloc((size_t)n * sizeof(int));
    int *prev = (int*)malloc((size_t)n * sizeof(int));

    /* Initialize: all distances = INF, all prev = -1 */
    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;

    struct MinHeap *heap = heap_create(n * 2);
    heap_push(heap, 0, start);

    printf("  Dijkstra from vertex %d:\n\n", start);

    while (!heap_is_empty(heap)) {
        struct PQNode current = heap_pop(heap);
        int d = current.distance;
        int v = current.vertex;

        /* Skip if we already found a better path */
        if (d > dist[v]) continue;

        printf("  Visit %d (distance=%d)\n", v, d);
        printf("    Neighbors: ");

        for (int i = 0; i < g->sizes[v]; ++i) {
            int neighbor = g->adj_lists[v][i].vertex;
            int weight = g->adj_lists[v][i].weight;
            int new_dist = d + weight;

            printf("(%d, w=%d) ", neighbor, weight);

            if (new_dist < dist[neighbor]) {
                dist[neighbor] = new_dist;
                prev[neighbor] = v;
                heap_push(heap, new_dist, neighbor);
                printf("<< updated dist[%d]=%d", neighbor, new_dist);
            }
            printf("\n             ");
        }
        printf("\n");
    }

    /* Print results */
    printf("  Final distances from %d:\n", start);
    for (int i = 0; i < n; ++i) {
        printf("    %d -> %d: ", start, i);
        if (dist[i] == INT_MAX) {
            printf("unreachable\n");
        } else {
            printf("%d  (path: ", dist[i]);
            /* Reconstruct path */
            int path[100], path_len = 0;
            for (int v = i; v != -1; v = prev[v])
                path[path_len++] = v;
            for (int p = path_len - 1; p >= 0; --p) {
                printf("%d", path[p]);
                if (p > 0) printf(" -> ");
            }
            printf(")\n");
        }
    }

    printf("\n  Lesson: Dijkstra = BFS for weighted graphs.\n");
    printf("     Uses a priority queue to always expand the closest node.\n\n");

    free(dist);
    free(prev);
    heap_destroy(heap);
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_graph_module(void)
{
    int choice;

    printf("\n  --- Graph Algorithms ---\n");
    printf("  1) BFS\n");
    printf("  2) DFS\n");
    printf("  3) Dijkstra's Shortest Path\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 4) {
        printf("  >> BFS\n");
        printf("  Graph (adjacency list):\n");
        struct Graph *g = graph_create(6);
        graph_add_undirected(g, 0, 1);
        graph_add_undirected(g, 0, 2);
        graph_add_undirected(g, 1, 4);
        graph_add_undirected(g, 2, 3);
        graph_add_undirected(g, 2, 5);
        graph_add_undirected(g, 3, 4);
        graph_add_undirected(g, 3, 5);
        graph_print(g);
        printf("\n");
        bfs(g, 0);
        graph_destroy(g);
    }

    if (choice == 2 || choice == 4) {
        printf("  >> DFS\n");
        printf("  Graph (adjacency list):\n");
        struct Graph *g = graph_create(6);
        graph_add_undirected(g, 0, 1);
        graph_add_undirected(g, 0, 2);
        graph_add_undirected(g, 1, 4);
        graph_add_undirected(g, 2, 3);
        graph_add_undirected(g, 2, 5);
        graph_add_undirected(g, 3, 4);
        graph_add_undirected(g, 3, 5);
        graph_print(g);
        printf("\n");
        dfs(g, 0);
        graph_destroy(g);
    }

    if (choice == 3 || choice == 4) {
        printf("  >> DIJKSTRA'S SHORTEST PATH\n");
        printf("  Weighted graph:\n");
        struct Graph *g = build_weighted_graph();
        graph_print(g);
        printf("\n");
        dijkstra(g, 0);
        graph_destroy(g);
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
