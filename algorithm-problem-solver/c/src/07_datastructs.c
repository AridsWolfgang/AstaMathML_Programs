/*
 * ===================================================================
 *  MODULE 7: DATA STRUCTURES  (C Edition)
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - Stack:  Undo/Redo in editors, browser back button, expression parsing
 *    - Queue:  Print spooling, task scheduling, BFS, message queues
 *    - BST:    Database indexing, in-memory sorted collections, symbol tables
 *
 *  In C, there are no classes, no templates, no STL containers.
 *  Everything is a struct + functions. Memory management is manual.
 *  This is NOT a limitation — you see every allocation, every pointer,
 *  every byte. This is how the internals of every language runtime work.
 *
 *  COMPLEXITIES:
 *    Stack:   Push/Pop/Top: O(1)
 *    Queue:   Enqueue/Dequeue: O(1) amortized
 *    BST:     Search/Insert: O(log n) average, O(n) worst
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void run_datastructs_module(void);
extern void clear_input(void);

/* ==================================================================
 *  STACK (LIFO - Last In, First Out)
 * ==================================================================
 *
 *  A dynamic array where elements are added and removed from the end.
 *  Like a stack of plates: you put plates on top, take from top.
 *
 *  Visual:
 *    Push 10    Push 20    Push 30     Pop
 *    [10]       [20]       [30]<-top   [20]<-top
 *               [10]       [20]        [10]
 *                          [10]
 *
 *  REAL-WORLD: Undo (Ctrl+Z) pushes each edit, pops to undo.
 *  Browser back button: pushes visited URLs, pops on back.
 *  Expression evaluation: (1+2)*3 uses stack for operators.
 * ================================================================== */

struct Stack {
    int *data;
    int top;        /* Index of top element, -1 = empty */
    int capacity;
};

static struct Stack* stack_create(int cap)
{
    struct Stack *s = (struct Stack*)malloc(sizeof(struct Stack));
    if (!s) return NULL;
    s->data = (int*)malloc((size_t)cap * sizeof(int));
    s->top = -1;
    s->capacity = cap;
    return s;
}

static void stack_destroy(struct Stack *s)
{
    if (!s) return;
    free(s->data);
    free(s);
}

static void stack_push(struct Stack *s, int val)
{
    if (s->top >= s->capacity - 1) {
        printf("    Stack overflow!\n");
        return;
    }
    s->data[++s->top] = val;
    printf("    Push %d  [size: %d]\n", val, s->top + 1);
}

static int stack_pop(struct Stack *s)
{
    if (s->top < 0) {
        printf("    Stack underflow!\n");
        return 0;
    }
    int val = s->data[s->top--];
    printf("    Pop %d  [size: %d]\n", val, s->top + 1);
    return val;
}

static int stack_peek(struct Stack *s)
{
    if (s->top < 0) {
        printf("    Stack is empty\n");
        return 0;
    }
    return s->data[s->top];
}

static int stack_is_empty(struct Stack *s)
{
    return s->top < 0;
}

static void demo_stack(void)
{
    printf("  Stack demonstration (LIFO):\n\n");

    struct Stack *s = stack_create(10);
    if (!s) {
        printf("  Failed to allocate stack!\n");
        return;
    }

    stack_push(s, 10);
    stack_push(s, 20);
    stack_push(s, 30);

    printf("  Top element: %d\n\n", stack_peek(s));

    stack_pop(s);
    stack_pop(s);

    printf("  Now top is: %d\n", stack_peek(s));
    printf("  Lesson: Last In, First Out - like a stack of plates.\n");
    printf("     Used in: undo/redo, browser history, expression parsing.\n\n");

    stack_destroy(s);
}

/* ==================================================================
 *  QUEUE (FIFO - First In, First Out)
 * ==================================================================
 *
 *  A circular buffer where elements are added at the REAR and
 *  removed from the FRONT. Like a line at a ticket counter.
 *
 *  Visual:
 *    Enqueue 10    Enqueue 20    Enqueue 30     Dequeue
 *    [10]          [10, 20]      [10, 20, 30]   [20, 30]
 *    front=rear    front  rear   front     rear  front  rear
 *
 *  REAL-WORLD: Printer queue, task scheduling, BFS, message queues.
 * ================================================================== */

struct Queue {
    int *data;
    int front;
    int rear;
    int size;       /* Number of elements */
    int capacity;
};

static struct Queue* queue_create(int cap)
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    if (!q) return NULL;
    q->data = (int*)malloc((size_t)cap * sizeof(int));
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    q->capacity = cap;
    return q;
}

static void queue_destroy(struct Queue *q)
{
    if (!q) return;
    free(q->data);
    free(q);
}

static void queue_enqueue(struct Queue *q, int val)
{
    if (q->size >= q->capacity) {
        printf("    Queue overflow!\n");
        return;
    }
    q->data[q->rear] = val;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
    printf("    Enqueue %d  [size: %d]\n", val, q->size);
}

static int queue_dequeue(struct Queue *q)
{
    if (q->size <= 0) {
        printf("    Queue underflow!\n");
        return 0;
    }
    int val = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    printf("    Dequeue %d  [size: %d]\n", val, q->size);
    return val;
}

static int queue_front(struct Queue *q)
{
    if (q->size <= 0) {
        printf("    Queue is empty\n");
        return 0;
    }
    return q->data[q->front];
}

static int queue_is_empty(struct Queue *q)
{
    return q->size == 0;
}

static void demo_queue(void)
{
    printf("  Queue demonstration (FIFO):\n\n");

    struct Queue *q = queue_create(10);
    if (!q) {
        printf("  Failed to allocate queue!\n");
        return;
    }

    queue_enqueue(q, 10);
    queue_enqueue(q, 20);
    queue_enqueue(q, 30);

    printf("  Front element: %d\n\n", queue_front(q));

    queue_dequeue(q);
    queue_dequeue(q);

    printf("  Now front is: %d\n", queue_front(q));
    printf("  Lesson: First In, First Out - like a line at the store.\n");
    printf("     Used in: printer queues, task scheduling, BFS.\n\n");

    queue_destroy(q);
}

/* ==================================================================
 *  BINARY SEARCH TREE (BST)
 * ==================================================================
 *
 *  Each node has at most 2 children: LEFT (smaller) and RIGHT (larger).
 *  For any node: left subtree < node < right subtree.
 *
 *  Visual:
 *           8          <- root
 *         /   \
 *        3     10
 *       / \      \
 *      1   6      14
 *         / \    /
 *        4   7  13
 *
 *  Search for 7:  8->3->6->7  (4 comparisons instead of 7)
 *
 *  REAL-WORLD: Database indexes (B-trees are BST variants),
 *  std::map/std::set internals, compiler symbol tables.
 * ================================================================== */

struct BSTNode {
    int data;
    struct BSTNode *left;
    struct BSTNode *right;
};

struct BST {
    struct BSTNode *root;
};

static struct BSTNode* bst_node_create(int val)
{
    struct BSTNode *node = (struct BSTNode*)malloc(sizeof(struct BSTNode));
    if (!node) return NULL;
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Recursive insert */
static struct BSTNode* bst_insert_node(struct BSTNode *node, int val)
{
    if (node == NULL)
        return bst_node_create(val);

    if (val < node->data)
        node->left = bst_insert_node(node->left, val);   /* Go LEFT */
    else if (val > node->data)
        node->right = bst_insert_node(node->right, val); /* Go RIGHT */
    /* If equal, do nothing (no duplicates) */

    return node;
}

static void bst_insert(struct BST *tree, int val)
{
    tree->root = bst_insert_node(tree->root, val);
}

/* Recursive search */
static int bst_search_node(struct BSTNode *node, int val)
{
    if (node == NULL) return 0;      /* Not found */
    if (val == node->data) return 1; /* Found! */

    if (val < node->data)
        return bst_search_node(node->left, val);   /* Search LEFT */
    else
        return bst_search_node(node->right, val);  /* Search RIGHT */
}

static int bst_search(struct BST *tree, int val)
{
    return bst_search_node(tree->root, val);
}

/* In-order traversal (prints sorted order!) */
static void bst_inorder_node(struct BSTNode *node)
{
    if (node == NULL) return;
    bst_inorder_node(node->left);           /* LEFT */
    printf("%d ", node->data);              /* NODE */
    bst_inorder_node(node->right);          /* RIGHT */
}

static void bst_inorder(struct BST *tree)
{
    bst_inorder_node(tree->root);
}

/* Visual tree print (rotated 90 degrees left) */
static void bst_print_node(struct BSTNode *node, int depth)
{
    if (node == NULL) return;

    /* Print right subtree first (appears at top) */
    bst_print_node(node->right, depth + 1);

    /* Print this node */
    for (int i = 0; i < depth; ++i) printf("    ");
    printf("%d\n", node->data);

    /* Print left subtree (appears at bottom) */
    bst_print_node(node->left, depth + 1);
}

static void bst_print(struct BST *tree)
{
    printf("  Tree structure (rotated 90 degrees left):\n\n");
    if (tree->root == NULL) {
        printf("    (empty)\n");
    } else {
        bst_print_node(tree->root, 0);
    }
}

/* Clean up memory */
static void bst_destroy_node(struct BSTNode *node)
{
    if (node == NULL) return;
    bst_destroy_node(node->left);
    bst_destroy_node(node->right);
    free(node);
}

static void bst_destroy(struct BST *tree)
{
    bst_destroy_node(tree->root);
    tree->root = NULL;
}

static void demo_bst(void)
{
    printf("  Binary Search Tree demonstration:\n\n");

    struct BST tree;
    tree.root = NULL;

    printf("  Inserting: 8, 3, 10, 1, 6, 14, 4, 7, 13\n\n");
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    for (int i = 0; i < 9; ++i)
        bst_insert(&tree, values[i]);

    bst_print(&tree);

    printf("\n  In-order traversal (sorts the data!): ");
    bst_inorder(&tree);
    printf("\n\n");

    /* Search demonstrations */
    int search_vals[] = {7, 9};
    for (int i = 0; i < 2; ++i) {
        printf("  Search for %d: %s\n", search_vals[i],
               bst_search(&tree, search_vals[i]) ? "FOUND" : "NOT FOUND");
    }

    printf("\n  Lesson: BST property: left < node < right.\n");
    printf("     In-order traversal always gives SORTED order!\n");
    printf("     Used in: database indexing, sorted collections.\n\n");

    bst_destroy(&tree);
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_datastructs_module(void)
{
    int choice;

    printf("\n  --- Data Structures ---\n");
    printf("  1) Stack (LIFO)\n");
    printf("  2) Queue (FIFO)\n");
    printf("  3) Binary Search Tree\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 4) {
        printf("  >> STACK\n");
        demo_stack();
    }

    if (choice == 2 || choice == 4) {
        printf("  >> QUEUE\n");
        demo_queue();
    }

    if (choice == 3 || choice == 4) {
        printf("  >> BINARY SEARCH TREE\n");
        demo_bst();
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
