# Algorithm Problem Solver — A Complete Lecture

> "Algorithms are the computational content of mathematics."
> — Edsger W. Dijkstra

---

# Part I: What Is an Algorithm Problem Solver?

## 1.1 The Definition

An **algorithm** is a finite sequence of well-defined instructions for solving a problem. An **algorithm problem solver** is a system (or in this case, a codebase) that collects, implements, and demonstrates these instructions across a range of classic problems.

But this project is more than a collection. It is a **learning laboratory** — a place where you:

- See how algorithms actually work, step by step
- Compare the same algorithm in C (manual memory), C++ (high-level abstractions), and Python (readable pseudocode)
- Trace through execution with real output
- Build your own algorithms using the same patterns

## 1.2 The Philosophy

Every algorithm in this project is implemented **from scratch**. We do not rely on `std::sort`, `qsort`, or `sorted()`. We write the sorting ourselves. This is intentional:

**You cannot truly use an algorithm until you have built it.**

When you type every line of `merge()`, you understand why merge sort needs O(n) extra space. When you debug a segmentation fault in your C implementation of Dijkstra, you understand pointers at a visceral level. When you see Python's `dict` handle your graph adjacency list effortlessly, you appreciate what high-level languages give you.

## 1.3 The 5-Step Algorithm Design Process

Every algorithm in this project follows this process. You should too:

```
┌─────────────────────────────────────────────────────┐
│             THE ALGORITHM DESIGN PROCESS             │
├─────────────────────────────────────────────────────┤
│                                                      │
│  1. UNDERSTAND the problem                           │
│     └─ What are the inputs? What are the outputs?    │
│                                                      │
│  2. DESIGN a solution                                │
│     └─ Write pseudocode. Draw diagrams.              │
│        Pick a strategy (divide & conquer, DP, etc.)  │
│                                                      │
│  3. IMPLEMENT in your chosen language                │
│     └─ Start with the simplest working version.      │
│        Get it right. Then make it fast.              │
│                                                      │
│  4. TEST with small examples                         │
│     └─ Trace through manually. Verify each step.     │
│        Use the ASCII diagrams in this project.       │
│                                                      │
│  5. ANALYZE and OPTIMIZE                             │
│     └─ What is the time complexity? Space?           │
│        Can we do better?                             │
│                                                      │
└─────────────────────────────────────────────────────┘
```

Every file in this project is organized around this process. The header comments cover step 1 (understanding), the ASCII art covers step 2 (design), the code covers step 3 (implementation), and the interactive output covers step 4 (testing). The complexity annotations at the top of each file cover step 5.

---

# Part II: How to Use This Project

## 2.1 Structure at a Glance

```
algorithm-problem-solver/
├── README.md              ← You are here — the lecture
├── c/                     ← C99 implementation
│   ├── Makefile
│   └── src/
│       ├── main.c         ← Interactive menu
│       ├── 01_sorting.c   ← Bubble, Quick, Merge Sort
│       ├── 02_searching.c ← Linear & Binary Search
│       ├── 03_recursion.c ← Factorial, Fibonacci, Towers of Hanoi
│       ├── 04_dp.c        ← Fibonacci DP, 0/1 Knapsack
│       ├── 05_greedy.c    ← Activity Selection, Coin Change
│       ├── 06_graph.c     ← BFS, DFS, Dijkstra
│       └── 07_datastructs.c ← Stack, Queue, Binary Search Tree
├── cpp/                   ← C++17 implementation (same modules)
└── python/                ← Python 3 implementation (same modules)
```

Each module is **self-contained**. You can study one algorithm, one file, or one language in isolation.

## 2.2 The Learning Workflow

Here is the most effective way to use this project:

### Step 1: Run It

```bash
# Pick your language:
cd c && gcc -std=c99 src/*.c -o build/algorithm_solver.exe -lm && ./build/algorithm_solver
cd cpp && g++ -std=c++17 src/*.cpp -o build/algorithm_solver.exe && ./build/algorithm_solver
cd python && python main.py
```

The interactive menu lets you choose any module and watch it execute step by step. **Start here.** See the algorithm in motion before reading the code.

### Step 2: Read the Comments

Every file opens with a block that tells you:
- **Real-world use cases** — so you know *why* this matters
- **Learning objectives** — so you know *what* to focus on
- **Complexity analysis** — so you know *how efficient* it is

For example, opening `02_searching.c`:

```c
/*
 * ===================================================================
 *  MODULE 2: SEARCHING ALGORITHMS
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - Phone contacts: Binary search on sorted contacts
 *    - Dictionary lookup: log2(100,000) = ~17 comparisons
 *    - Git bisect: Binary search through commit history
 *    - Database index lookup (B-trees)
 *
 *  KEY INSIGHT:
 *    Sort once (O(n log n)), search forever in O(log n).
 *
 *  COMPLEXITIES:
 *    Linear Search:  O(n) time, O(1) space
 *    Binary Search:  O(log n) time, O(1) space (sorted required!)
 * ===================================================================
 */
```

### Step 3: Study the ASCII Diagrams

Before each implementation, there is a visual diagram of the algorithm. For example, in the sorting module:

```
//  Pass 1:  [5, 3, 8, 1]  -> compare 5&3 -> swap -> [3, 5, 8, 1]
//                                     -> compare 5&8 -> no swap
//                                     -> compare 8&1 -> swap -> [3, 5, 1, 8]
//             The largest element (8) "bubbles up" to its correct position.
```

Trace through these diagrams **on paper**. Write the array state at each step. This builds intuition.

### Step 4: Read the Step-by-Step Code

The code is commented to explain *why*, not just *what*:

```c
// Outer loop: each pass places the next largest element
for (int i = 0; i < n - 1; ++i) {
    int swapped = 0;  // Optimization: track if any swap happened

    // Inner loop: compare adjacent elements
    for (int j = 0; j < n - 1 - i; ++j) {
        if (arr[j] > arr[j + 1]) {
            swap_int(&arr[j], &arr[j + 1]);
            swapped = 1;
        }
    }

    // If no swaps happened, array is already sorted — EARLY EXIT!
    if (!swapped) break;
}
```

### Step 5: Compare Across Languages

This is where the real learning happens. Take Bubble Sort and see it in three languages:

**C (`c/src/01_sorting.c`):**
```c
static void bubble_sort(int *arr, int n, int verbose) {
    for (int i = 0; i < n - 1; ++i) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
```
*Notice: explicit temp variable, manual swap, `int* arr` pointer — you control every byte.*

**C++ (`cpp/src/01_sorting.cpp`):**
```cpp
void bubbleSort(std::vector<int>& arr, bool verbose) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
```
*Notice: `std::vector` manages memory, `std::swap` is a function call, reference parameter `&arr`.*

**Python (`python/sorting.py`):**
```python
def bubble_sort(arr, verbose=True):
    n = len(arr)
    for i in range(n - 1):
        swapped = False
        for j in range(n - 1 - i):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]  # tuple swap
                swapped = True
        if not swapped:
            break
```
*Notice: no types at all, `arr[j], arr[j+1] = arr[j+1], arr[j]` is a single swap expression.*

**The algorithm is the same. The expression changes. Learn the algorithm once, write it anywhere.**

## 2.3 What Each Module Teaches

| Module | Core Concept | What You'll Master |
|--------|-------------|-------------------|
| `01_sorting` | Comparison-based sorting | Divide & conquer, recursion, partitioning |
| `02_searching` | Search strategies | Sorted vs unsorted data, divide & conquer |
| `03_recursion` | Self-similar problems | Base cases, call stack, memoization |
| `04_dp` | Optimal sub-structure | Overlapping subproblems, tabulation |
| `05_greedy` | Local vs global optimality | When greedy works, when it doesn't |
| `06_graph` | Graph traversal & paths | BFS/DFS, priority queues, shortest paths |
| `07_datastructs` | Fundamental containers | LIFO/FIFO, tree properties, pointers |

---

# Part III: How to Build Algorithms From Scratch

## 3.1 The Framework

Building an algorithm from scratch is not magic. It is a repeatable process. Here is the framework used to write every algorithm in this project:

### Step 1: Define the Problem Precisely

```
Input:  What goes in? What type? What constraints?
Output: What comes out? What type?
Goal:   What are we optimizing? (speed, memory, accuracy)
```

**Example — Insertion Sort (not in the project, let's build it):**

```
Input:  An array A of n integers, unsorted.
Output: The same array A, sorted in ascending order.
Goal:   Sort in-place (O(1) extra space), stable sort.
```

### Step 2: Write Pseudocode Before Code

Pseudocode is language-agnostic. It lets you design the logic without worrying about syntax.

```
INSERTION-SORT(A, n):
    for i = 1 to n-1:
        key = A[i]           // The card we're inserting
        j = i - 1            // Start comparing with the previous element
        while j >= 0 and A[j] > key:
            A[j + 1] = A[j]  // Shift element right
            j = j - 1
        A[j + 1] = key       // Place key in its correct position
```

### Step 3: Draw the Algorithm in Motion

Use paper. Or use ASCII. Here's Insertion Sort on `[5, 3, 8, 1]`:

```
i=1: key=3
  Compare 5 > 3 -> shift 5 right: [_, 5, 8, 1]
  Place 3:                        [3, 5, 8, 1]

i=2: key=8
  Compare 5 > 8? No. Place 8:     [3, 5, 8, 1]

i=3: key=1
  Compare 8 > 1 -> shift 8 right: [3, 5, _, 8]
  Compare 5 > 1 -> shift 5 right: [3, _, 5, 8]
  Compare 3 > 1 -> shift 3 right: [_, 3, 5, 8]
  Place 1:                        [1, 3, 5, 8]
```

Notice: Insertion Sort is like sorting a hand of playing cards — you take one card at a time and insert it into the correct position among the already-sorted cards.

### Step 4: Implement

Now translate the pseudocode into your language of choice:

```c
void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```

```cpp
void insertionSort(std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}
```

```python
def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and arr[j] > key:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key
```

### Step 5: Test with Tiny Examples

```c
int arr[] = {5, 3, 8, 1, 2};
int n = 5;
insertion_sort(arr, n);
// Expected: [1, 2, 3, 5, 8]
```

Test edge cases:
- Empty array `[]` — loop doesn't execute, returns immediately
- Single element `[1]` — loop doesn't execute, already sorted
- Already sorted `[1, 2, 3]` — each key is already in place, O(n)
- Reverse sorted `[3, 2, 1]` — worst case, O(n²)
- All equal `[5, 5, 5]` — stable, no unnecessary swaps

### Step 6: Analyze Complexity

- **Time:** O(n²) worst case (reverse sorted), O(n) best case (already sorted)
- **Space:** O(1) — sorts in place with only a few variables
- **Stable:** Yes — equal elements keep their relative order
- **Adaptive:** Yes — performance improves on nearly-sorted data

### Step 7: Ask "Can I Do Better?"

For sorting, yes — Quick Sort and Merge Sort are O(n log n). But Insertion Sort is:
- The simplest sorting algorithm to implement correctly
- The fastest for small arrays (n < 50)
- The best for nearly-sorted data
- Used internally by Timsort (Python's sort) for small subarrays

**The best algorithm depends on your constraints.** This is why you need to know multiple algorithms.

## 3.2 Common Algorithm Design Patterns

As you study the algorithms in this project, you will notice recurring patterns:

### Divide and Conquer (Quick Sort, Merge Sort, Binary Search)

```
1. DIVIDE the problem into smaller subproblems
2. CONQUER each subproblem recursively
3. COMBINE the results
```

**Recognize it when:** A problem can be split into independent parts whose solutions can be merged.

### Dynamic Programming (Fibonacci DP, Knapsack)

```
1. Identify overlapping subproblems
2. Define the recurrence relation
3. Solve bottom-up (tabulation) or top-down with memoization
```

**Recognize it when:** The problem asks for an optimal value, and the optimal solution depends on optimal solutions to subproblems.

### Greedy (Activity Selection, Coin Change)

```
1. Make the best LOCAL choice at each step
2. Hope it leads to a GLOBAL optimum
```

**Recognize it when:** A locally optimal choice leads to a globally optimal solution (requires proof).

### Graph Traversal (BFS, DFS, Dijkstra)

```
Model the problem as a graph (nodes + edges),
then traverse or find paths through it.
```

**Recognize it when:** The problem involves relationships, connections, or paths between entities.

## 3.3 How to Design a New Algorithm

When you face a problem that doesn't match any known algorithm:

1. **Brute force first.** Solve it the simplest way possible, even if it's slow. This gives you a correct baseline.
2. **Look for structure.** Can you sort the data? Can you use a hash table? Is it a graph problem?
3. **Try the patterns.** Does divide and conquer help? Can you define a DP recurrence?
4. **Analyze your bottleneck.** Where is the time going? Can you precompute? Can you use a better data structure?
5. **Iterate.** Your first design will not be your best. Algorithm design is revision.

---

# Part IV: How to Use Algorithms in Real Projects

## 4.1 Integration Patterns

There are three main ways to use algorithms from this project in real applications:

### Pattern A: Library Module

Extract the algorithm into a reusable library file:

```
my_project/
├── lib/
│   ├── sort.h       ← Declarations from this project
│   └── sort.c       ← Implementations from this project
└── src/
    └── main.c       ← #include "lib/sort.h"
```

This is the **cleanest** approach — your algorithm code is separate, testable, and reusable across projects.

### Pattern B: Inline (for small algorithms)

For tiny algorithms like linear search or factorial, just write the function directly where you need it:

```c
int find_maximum(int *arr, int n) {
    int max = arr[0];
    for (int i = 1; i < n; ++i)
        if (arr[i] > max) max = arr[i];
    return max;
}
```

This is appropriate when the algorithm is 5-10 lines and used only once.

### Pattern C: Adapted and Extended

Most real-world uses require adapting a textbook algorithm:

| Original | Real-World Adaptation |
|----------|----------------------|
| Binary search (integers) | Binary search on sorted database records by key |
| Dijkstra (static graph) | Dijkstra with dynamic edge weights (traffic data) |
| BFS (simple graph) | BFS on social graph with degree-of-separation limits |
| Quick Sort (array) | Quick Sort with custom comparator for structs |

**Example: Custom comparator for sorting structs.**

If you have an array of `Product` structs and need to sort by price:

```c
typedef struct {
    char name[100];
    double price;
    int rating;
} Product;

int compare_by_price(const void *a, const void *b) {
    double diff = ((const Product*)a)->price - ((const Product*)b)->price;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

// Now pass compare_by_price to any sorting function
quick_sort(products, n, compare_by_price);
```

The algorithms from this project give you the **engine**; you provide the **rules**.

## 4.2 Real Project Examples by Category

### Sorting in Projects

| Use Case | Algorithm | Why |
|----------|-----------|-----|
| Display products by price | Quick Sort | Fast average-case, in-place |
| Merge external data streams | Merge Sort | Stable, works with sequential files |
| Nearly-sorted log file | Insertion Sort | O(n) on nearly-sorted data |
| Real-time leaderboard | Heap Sort | O(n log n) worst case, no recursion |

### Searching in Projects

| Use Case | Algorithm | Why |
|----------|-----------|-----|
| Find user by ID in sorted list | Binary Search | O(log n), database index-like |
| Auto-complete suggestions | Trie + DFS | Prefix-based, fast |
| Find file in filesystem | DFS (recursive) | Naturally recursive structure |

### Graph Algorithms in Projects

| Use Case | Algorithm | Why |
|----------|-----------|-----|
| Friend suggestions (2nd degree) | BFS | Level-order traversal, shortest hop path |
| GPS route planning | Dijkstra | Weighted shortest path |
| Web crawler | BFS | Level-order, polite crawling |
| Dependency resolution | Topological Sort (DFS) | Directed acyclic graph ordering |

### Dynamic Programming in Projects

| Use Case | Algorithm | Why |
|----------|-----------|-----|
| Inventory optimization | Knapsack | Limited space, maximize profit |
| Spell checker | Edit Distance (Levenshtein) | String similarity |
| Stock market prediction | DP (Viterbi) | Hidden Markov models |

## 4.3 Testing Your Algorithm in a Project

Never trust an algorithm until you test it. Here is the minimal test framework:

```c
void test_sorting_algorithm(void (*sort)(int*, int)) {
    int test1[] = {5, 3, 8, 1, 2};
    int expected1[] = {1, 2, 3, 5, 8};
    int n1 = 5;

    sort(test1, n1);

    for (int i = 0; i < n1; ++i) {
        if (test1[i] != expected1[i]) {
            printf("FAIL: test1 at index %d: got %d, expected %d\n",
                   i, test1[i], expected1[i]);
            return;
        }
    }
    printf("PASS: all tests\n");
}
```

For Python, this becomes simpler:

```python
def test_sorting_algorithm(sort_fn):
    tests = [
        ([], []),
        ([1], [1]),
        ([5, 3, 8, 1, 2], [1, 2, 3, 5, 8]),
        ([3, 3, 3], [3, 3, 3]),
        ([9, 8, 7, 6], [6, 7, 8, 9]),
    ]

    for input_arr, expected in tests:
        result = input_arr.copy()
        sort_fn(result)
        assert result == expected, f"Failed: {input_arr} -> {result}, expected {expected}"

    print("All tests passed!")
```

## 4.4 When to Write From Scratch vs. Use a Library

| Write From Scratch When | Use a Library When |
|------------------------|-------------------|
| You're learning (your goal is understanding) | You need production reliability |
| Your data has unique constraints | The standard implementation is fine |
| No library exists for your language | The library is well-tested and maintained |
| You need a specific variant not in the library | Performance is already optimal |
| Your algorithm IS the product | You are building something else |

**The best engineers know how to do both.** They can write a custom algorithm when needed and know when to trust the standard library.

## 4.5 A Complete Workflow: Algorithm to Project

Here is the end-to-end workflow for bringing an algorithm from this project into a real application:

```
1. IDENTIFY the need
   "I need to find the shortest path between two points
    in my game's map."

2. SELECT the algorithm
   "Dijkstra's algorithm works for weighted graphs.
    My map has weighted edges (road length, terrain difficulty)."

3. STUDY the implementation
   Open `06_graph.c` and read the Dijkstra implementation.
   Understand how it uses the priority queue.

4. ADAPT to your data
   Replace `int` coordinates with your game's `struct Position`.
   Replace the simple graph with your map's tile-based grid.

5. TEST in isolation
   Create a small map with known shortest paths.
   Verify the algorithm produces correct results.

6. INTEGRATE into your project
   Call your adapted Dijkstra when the player sets a destination.
   Use the result to render the path on screen.

7. PROFILE and OPTIMIZE (if needed)
   Is the path calculation fast enough?
   Can you precompute paths between important nodes?
   Can you use a simpler algorithm (BFS if unweighted)?
```

---

# Part V: The Project as a Reference

## 5.1 Build & Run

### C++ version
```bash
cd cpp && g++ -std=c++17 src/*.cpp -o build/algorithm_solver.exe && ./build/algorithm_solver
```

### C version
```bash
cd c && gcc -std=c99 src/*.c -o build/algorithm_solver.exe -lm && ./build/algorithm_solver
```

### Python version
```bash
cd python && python main.py
```

## 5.2 Module Reference

| Module | File (C/C++/Python) | What It Contains |
|--------|---------------------|------------------|
| Sorting | `01_sorting.*` | Bubble Sort (O(n²)), Quick Sort (O(n log n)), Merge Sort (O(n log n)) |
| Searching | `02_searching.*` | Linear Search (O(n)), Binary Search (O(log n)) |
| Recursion | `03_recursion.*` | Factorial, Fibonacci (naive + memoized), Towers of Hanoi |
| Dynamic Programming | `04_dp.*` | Fibonacci DP (bottom-up), 0/1 Knapsack with DP table |
| Greedy | `05_greedy.*` | Activity Selection, Coin Change (with counterexample) |
| Graph | `06_graph.*` | BFS, DFS, Dijkstra with priority queue |
| Data Structures | `07_datastructs.*` | Stack (LIFO), Queue (FIFO), Binary Search Tree |

## 5.3 Complexity Cheat Sheet

| Algorithm | Time (Best) | Time (Average) | Time (Worst) | Space |
|-----------|-------------|----------------|--------------|-------|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Linear Search | O(1) | O(n) | O(n) | O(1) |
| Binary Search | O(1) | O(log n) | O(log n) | O(1) |
| Factorial | O(n) | O(n) | O(n) | O(n) |
| Fibonacci (naive) | O(1) | O(2ⁿ) | O(2ⁿ) | O(n) |
| Fibonacci (DP) | O(n) | O(n) | O(n) | O(1) |
| Knapsack (DP) | O(nW) | O(nW) | O(nW) | O(nW) |
| Activity Selection | O(n log n) | O(n log n) | O(n log n) | O(1) |
| BFS/DFS | O(V+E) | O(V+E) | O(V+E) | O(V) |
| Dijkstra | O(V log V + E) | O((V+E) log V) | O((V+E) log V) | O(V) |

---

# Final Words

This project is a starting point, not an ending point. The algorithms here are the **classics** — the ones that appear in every interview, every CS curriculum, every production system. Master these, and you will:

1. **Recognize** algorithm patterns in new problems
2. **Design** solutions with confidence
3. **Implement** them correctly in any language
4. **Know** when to use a library and when to write your own

The next step is yours. Modify these algorithms. Break them. Fix them. Add new ones. Take them into your own projects.

**An algorithm is not something you learn. It is something you do.**

---

*"Bad programmers worry about the code. Good programmers worry about data structures and their relationships."*
— Linus Torvalds
