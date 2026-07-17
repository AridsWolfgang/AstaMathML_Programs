"""
=====================================================================
 MODULE 7: DATA STRUCTURES  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - Stack:  Undo/Redo in editors, browser back, expression parsing
  - Queue:  Print spooling, task scheduling, BFS, message queues
  - BST:    Database indexing, sorted collections, symbol tables

In Python, we don't have pointers or manual memory management.
Instead, we use object references and let the garbage collector
handle cleanup. The ALGORITHMS are the same — only the syntax
and memory model differ.

COMPLEXITIES:
  Stack:   Push/Pop/Top: O(1)
  Queue:   Enqueue/Dequeue: O(1) amortized
  BST:     Search/Insert: O(log n) average, O(n) worst
=====================================================================
"""


# =================================================================
#  STACK (LIFO - Last In, First Out)
# =================================================================
#
#  Visual:
#    Push 10    Push 20    Push 30     Pop
#    [10]       [20]       [30] <- top [20] <- top
#               [10]       [20]        [10]
#                          [10]
#
#  REAL-WORLD: Undo (Ctrl+Z), browser back button, expression parsing.
# =================================================================
class Stack:
    def __init__(self, capacity=100):
        self._data = []
        self._capacity = capacity

    def push(self, val):
        if len(self._data) >= self._capacity:
            print(f"    Stack overflow!")
            return
        self._data.append(val)
        print(f"    Push {val}  [size: {len(self._data)}]")

    def pop(self):
        if not self._data:
            print("    Stack underflow!")
            return None
        val = self._data.pop()
        print(f"    Pop {val}  [size: {len(self._data)}]")
        return val

    def peek(self):
        if not self._data:
            print("    Stack is empty")
            return None
        return self._data[-1]

    def is_empty(self):
        return len(self._data) == 0

    def __len__(self):
        return len(self._data)


def demo_stack():
    print("  Stack demonstration (LIFO):\n")

    s = Stack(10)
    s.push(10)
    s.push(20)
    s.push(30)

    print(f"  Top element: {s.peek()}\n")
    s.pop()
    s.pop()
    print(f"  Now top is: {s.peek()}")
    print("  Lesson: Last In, First Out — like a stack of plates.")
    print("     Used in: undo/redo, browser history, expression parsing.\n")


# =================================================================
#  QUEUE (FIFO - First In, First Out)
# =================================================================
#
#  Visual:
#    Enqueue 10    Enqueue 20    Enqueue 30     Dequeue
#    [10]          [10, 20]      [10, 20, 30]   [20, 30]
#
#  REAL-WORLD: Printer queue, task scheduling, BFS.
# =================================================================
class Queue:
    def __init__(self):
        self._data = []

    def enqueue(self, val):
        self._data.append(val)
        print(f"    Enqueue {val}  [size: {len(self._data)}]")

    def dequeue(self):
        if not self._data:
            print("    Queue underflow!")
            return None
        val = self._data.pop(0)
        print(f"    Dequeue {val}  [size: {len(self._data)}]")
        return val

    def front(self):
        if not self._data:
            print("    Queue is empty")
            return None
        return self._data[0]

    def is_empty(self):
        return len(self._data) == 0

    def __len__(self):
        return len(self._data)


def demo_queue():
    print("  Queue demonstration (FIFO):\n")

    q = Queue()
    q.enqueue(10)
    q.enqueue(20)
    q.enqueue(30)

    print(f"  Front element: {q.front()}\n")
    q.dequeue()
    q.dequeue()
    print(f"  Now front is: {q.front()}")
    print("  Lesson: First In, First Out — like a line at the store.")
    print("     Used in: printer queues, task scheduling, BFS.\n")


# =================================================================
#  BINARY SEARCH TREE (BST)
# =================================================================
#
#  Properties:
#    - Each node has 0-2 children (LEFT and RIGHT)
#    - LEFT subtree: all values < node value
#    - RIGHT subtree: all values > node value
#
#  Visual:
#           8
#         /   \
#        3     10
#       / \      \
#      1   6      14
#         / \    /
#        4   7  13
#
#  Search for 7: 8 -> 3 -> 6 -> 7  (4 steps vs 7)
#
#  REAL-WORLD: Database indexes, sorted collections, symbol tables.
# =================================================================
class BSTNode:
    def __init__(self, val):
        self.data = val
        self.left = None
        self.right = None


class BST:
    def __init__(self):
        self.root = None

    def insert(self, val):
        """Insert a value into the BST."""
        self.root = self._insert(self.root, val)

    def _insert(self, node, val):
        if node is None:
            return BSTNode(val)

        if val < node.data:
            node.left = self._insert(node.left, val)
        elif val > node.data:
            node.right = self._insert(node.right, val)

        return node

    def search(self, val):
        """Search for a value. Returns True if found."""
        return self._search(self.root, val)

    def _search(self, node, val):
        if node is None:
            return False
        if val == node.data:
            return True
        if val < node.data:
            return self._search(node.left, val)
        return self._search(node.right, val)

    def inorder(self):
        """In-order traversal (prints sorted order!)."""
        result = []
        self._inorder(self.root, result)
        return result

    def _inorder(self, node, result):
        if node is None:
            return
        self._inorder(node.left, result)
        result.append(node.data)
        self._inorder(node.right, result)

    def print_tree(self):
        """Print tree rotated 90 degrees left."""
        print("  Tree structure (rotated 90 degrees left):\n")
        self._print_tree(self.root, 0)

    def _print_tree(self, node, depth):
        if node is None:
            return
        # Print right subtree (appears at top)
        self._print_tree(node.right, depth + 1)
        # Print this node
        print("    " * depth + f"{node.data}")
        # Print left subtree (appears at bottom)
        self._print_tree(node.left, depth + 1)


def demo_bst():
    print("  Binary Search Tree demonstration:\n")

    bst = BST()
    values = [8, 3, 10, 1, 6, 14, 4, 7, 13]

    print(f"  Inserting: {values}\n")
    for v in values:
        bst.insert(v)

    bst.print_tree()
    print(f"\n  In-order traversal (sorts the data!): {bst.inorder()}\n")

    # Search demonstrations
    for val in (7, 9):
        print(f"  Search for {val}: {'FOUND' if bst.search(val) else 'NOT FOUND'}")

    print("\n  Lesson: BST property — left < node < right.")
    print("     In-order traversal always gives SORTED order!")
    print("     Used in: database indexing, sorted collections.\n")


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_datastructs_module():
    print("\n  --- Data Structures ---")
    print("  1) Stack (LIFO)")
    print("  2) Queue (FIFO)")
    print("  3) Binary Search Tree")
    print("  4) All")
    choice = input("  Enter choice: ").strip()

    print()

    if choice in ("1", "4"):
        print("  >> STACK")
        demo_stack()

    if choice in ("2", "4"):
        print("  >> QUEUE")
        demo_queue()

    if choice in ("3", "4"):
        print("  >> BINARY SEARCH TREE")
        demo_bst()

    input("  Press Enter to return to menu...")
