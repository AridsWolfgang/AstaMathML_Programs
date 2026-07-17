/**
 * ===================================================================
 *  MODULE 7: DATA STRUCTURES
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - Stack:  Undo/Redo in editors, browser back button, expression evaluation
 *   - Queue:  Print spooling, task scheduling, message queues (Kafka, RabbitMQ)
 *   - BST:    Database indexing, in-memory sorted data, symbol tables
 *   - Linked List: Memory allocation (free lists), music player playlists
 *   - Hash Table:  Dictionary, cache (LRU cache), symbol table in compilers
 *   - Heap:        Priority queue (task scheduling), Dijkstra's algorithm
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - How fundamental data structures work internally
 *   - LIFO (Stack) vs. FIFO (Queue) behavior
 *   - Binary Search Tree properties and why they enable fast search
 *   - How to implement these from scratch (no STL containers in these demos!)
 *   - Time complexity trade-offs between different structures
 *
 * ⏱ COMPLEXITIES:
 *   Stack (array):   Push/Pop/Top: O(1)
 *   Queue (array):   Enqueue/Dequeue: O(1) amortized
 *   BST:             Search/Insert/Delete: O(log n) average, O(n) worst
 * ===================================================================
 */

#include <iostream>
#include <string>
#include <stdexcept>

void runDataStructsModule();

// ==================================================================
//  STACK (LIFO — Last In, First Out)
// ==================================================================
//
//  HOW IT WORKS:
//   Elements are added (pushed) and removed (popped) from the TOP.
//   Like a stack of plates — you put plates on top and take from top.
//
//  VISUAL:
//
//      Push 10    Push 20    Push 30     Pop
//      ┌───┐      ┌───┐      ┌───┐      ┌───┐
//      │10 │      │20 │      │30 │←top  │   │
//      └───┘      ├───┤      ├───┤      ├───┤
//                 │10 │      │20 │      │20 │←top
//                 └───┘      ├───┤      ├───┤
//                            │10 │      │10 │
//                            └───┘      └───┘
//
//  REAL-WORLD USE:
//   - Undo (Ctrl+Z): each edit is pushed, undo pops the last edit
//   - Browser back button: visited URLs pushed, back pops the last
//   - Expression evaluation:  (1 + 2) * 3 → use stack for operators
//   - Function call stack: each function call pushes a frame
// ==================================================================

template<typename T>
class Stack {
private:
    T* arr;
    int capacity;
    int topIndex;

public:
    Stack(int cap = 100) : capacity(cap), topIndex(-1) {
        arr = new T[capacity];
    }

    ~Stack() { delete[] arr; }

    void push(const T& val) {
        if (topIndex >= capacity - 1) {
            std::cout << "    Stack overflow! (capacity=" << capacity << ")\n";
            return;
        }
        arr[++topIndex] = val;
        std::cout << "    Push " << val << "  [stack size: " << (topIndex + 1) << "]\n";
    }

    T pop() {
        if (isEmpty()) {
            std::cout << "    Stack underflow! (nothing to pop)\n";
            return T();
        }
        T val = arr[topIndex--];
        std::cout << "    Pop " << val << "  [stack size: " << (topIndex + 1) << "]\n";
        return val;
    }

    T top() const {
        if (isEmpty()) {
            std::cout << "    Stack is empty\n";
            return T();
        }
        return arr[topIndex];
    }

    bool isEmpty() const { return topIndex == -1; }
    int size() const { return topIndex + 1; }
};

void demoStack()
{
    std::cout << "  Stack demonstration (LIFO):\n\n";

    Stack<int> s(10);
    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "  Top element: " << s.top() << "\n\n";

    s.pop();
    s.pop();

    std::cout << "  Now top is: " << s.top() << "\n";
    std::cout << "  📝 Last In, First Out — like a stack of plates.\n";
    std::cout << "     Used in: undo/redo, browser history, expression parsing.\n\n";
}

// ==================================================================
//  QUEUE (FIFO — First In, First Out)
// ==================================================================
//
//  HOW IT WORKS:
//   Elements are added (enqueued) at the REAR and removed (dequeued)
//   from the FRONT. Like a line at a ticket counter.
//
//  VISUAL:
//
//    Enqueue 10  Enqueue 20  Enqueue 30   Dequeue
//    ┌───┐       ┌───┬───┐   ┌───┬───┬───┐   ┌───┬───┐
//    │10│       │10│20│   │10│20│30│   │20│30│
//    └───┘       └───┴───┘   └───┴───┴───┘   └───┴───┘
//   front=rear   front  rear front    rear  front  rear
//
//  REAL-WORLD USE:
//   - Printer queue: documents print in the order they were submitted
//   - Task scheduler: CPU processes tasks in FIFO order
//   - Message queues: RabbitMQ, Kafka deliver messages in order
//   - BFS traversal: uses a queue to track "next to visit"
// ==================================================================

template<typename T>
class Queue {
private:
    T* arr;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;  // Number of elements currently in queue

public:
    Queue(int cap = 100) : capacity(cap), frontIndex(0), rearIndex(0), count(0) {
        arr = new T[capacity];
    }

    ~Queue() { delete[] arr; }

    void enqueue(const T& val) {
        if (count == capacity) {
            std::cout << "    Queue overflow!\n";
            return;
        }
        arr[rearIndex] = val;
        rearIndex = (rearIndex + 1) % capacity;  // Circular behavior
        ++count;
        std::cout << "    Enqueue " << val << "  [queue size: " << count << "]\n";
    }

    T dequeue() {
        if (isEmpty()) {
            std::cout << "    Queue underflow! (nothing to dequeue)\n";
            return T();
        }
        T val = arr[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        --count;
        std::cout << "    Dequeue " << val << "  [queue size: " << count << "]\n";
        return val;
    }

    T front() const {
        if (isEmpty()) {
            std::cout << "    Queue is empty\n";
            return T();
        }
        return arr[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
};

void demoQueue()
{
    std::cout << "  Queue demonstration (FIFO):\n\n";

    Queue<int> q(10);
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    std::cout << "  Front element: " << q.front() << "\n\n";

    q.dequeue();
    q.dequeue();

    std::cout << "  Now front is: " << q.front() << "\n";
    std::cout << "  📝 First In, First Out — like a line at the store.\n";
    std::cout << "     Used in: printer queues, task scheduling, BFS.\n\n";
}

// ==================================================================
//  BINARY SEARCH TREE (BST)
// ==================================================================
//
//  PROPERTIES:
//   - Each node has at most 2 children: LEFT and RIGHT
//   - LEFT subtree contains only values LESS than the node
//   - RIGHT subtree contains only values GREATER than the node
//   - No duplicate values (in our implementation)
//
//  VISUAL:
//
//          8         ← root
//        /   \
//       3     10     ← children
//      / \      \
//     1   6      14
//        / \    /
//       4   7  13
//
//  Search for 7:
//    8 > 7 → go LEFT  → to 3
//    3 < 7 → go RIGHT → to 6
//    6 < 7 → go RIGHT → to 7 → FOUND!
//    Only 4 comparisons instead of 7 (if we had searched linearly).
//
//  REAL-WORLD USE:
//   - Database indexes (B-trees are a generalization of BSTs)
//   - In-memory sorted collections (std::map, std::set)
//   - Symbol tables in compilers
//   - File system directory structures
// ==================================================================

template<typename T>
class BinarySearchTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Recursive insert
    Node* insert(Node* node, const T& val) {
        if (node == nullptr) {
            return new Node(val);
        }

        if (val < node->data) {
            node->left = insert(node->left, val);   // Go LEFT
        } else if (val > node->data) {
            node->right = insert(node->right, val); // Go RIGHT
        }
        // If equal, do nothing (no duplicates)

        return node;
    }

    // Recursive search
    bool search(Node* node, const T& val) const {
        if (node == nullptr) return false;
        if (val == node->data) return true;

        if (val < node->data) {
            return search(node->left, val);   // Search LEFT
        } else {
            return search(node->right, val);  // Search RIGHT
        }
    }

    // In-order traversal (prints sorted order!)
    void inorder(Node* node) const {
        if (node == nullptr) return;
        inorder(node->left);             // Visit LEFT
        std::cout << node->data << " ";  // Visit NODE
        inorder(node->right);            // Visit RIGHT
    }

    // Visual representation (indented)
    void printTree(Node* node, int depth) const {
        if (node == nullptr) return;
        printTree(node->right, depth + 1);
        for (int i = 0; i < depth; ++i) std::cout << "    ";
        std::cout << node->data << "\n";
        printTree(node->left, depth + 1);
    }

    // Clean up memory
    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroy(root); }

    void insert(const T& val) {
        root = insert(root, val);
    }

    bool search(const T& val) const {
        return search(root, val);
    }

    void inorder() const {
        inorder(root);
    }

    void printTree() const {
        std::cout << "  Tree structure (rotated 90° left):\n\n";
        if (root == nullptr) {
            std::cout << "    (empty)\n";
        } else {
            printTree(root, 0);
        }
    }
};

void demoBST()
{
    std::cout << "  Binary Search Tree demonstration:\n\n";

    BinarySearchTree<int> bst;

    // Insert values — notice we insert in a specific order
    std::cout << "  Inserting: 8, 3, 10, 1, 6, 14, 4, 7, 13\n\n";
    bst.insert(8);
    bst.insert(3);
    bst.insert(10);
    bst.insert(1);
    bst.insert(6);
    bst.insert(14);
    bst.insert(4);
    bst.insert(7);
    bst.insert(13);

    // Print tree (rotated — root is at left, right subtrees above, left below)
    bst.printTree();
    std::cout << "\n  In-order traversal (sorts the data!): ";
    bst.inorder();
    std::cout << "\n\n";

    // Search demonstrations
    int searchVals[] = {7, 9};
    for (int val : searchVals) {
        std::cout << "  Search for " << val << ": "
                  << (bst.search(val) ? "FOUND" : "NOT FOUND") << "\n";
    }

    std::cout << "\n  📝 BST property: left < node < right\n";
    std::cout << "     In-order traversal always gives SORTED order!\n";
    std::cout << "     Used in: database indexing, sorted collections.\n\n";
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runDataStructsModule()
{
    int choice;

    std::cout << "\n  --- Data Structures ---\n";
    std::cout << "  1) Stack (LIFO)\n";
    std::cout << "  2) Queue (FIFO)\n";
    std::cout << "  3) Binary Search Tree\n";
    std::cout << "  4) All\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        std::cout << "  ▶ STACK\n";
        demoStack();
    }

    if (choice == 2 || choice == 4) {
        std::cout << "  ▶ QUEUE\n";
        demoQueue();
    }

    if (choice == 3 || choice == 4) {
        std::cout << "  ▶ BINARY SEARCH TREE\n";
        demoBST();
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
