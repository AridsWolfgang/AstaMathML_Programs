/*
 * ===================================================================
 *  MODULE 4: DYNAMIC PROGRAMMING  (C Edition)
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - Navigation: Shortest path (Google Maps, Waze)
 *    - Bioinformatics: DNA sequence alignment (Needleman-Wunsch)
 *    - Resource allocation: Budget optimization
 *    - Scheduling: Maximize profit, minimize cost
 *    - Finance: Portfolio optimization, option pricing
 *    - AI: Reinforcement learning uses DP (Bellman equations)
 *
 *  THE CORE IDEA:
 *    "Solve each subproblem once, store the answer, reuse it."
 *    Never recompute what you already know.
 *
 *  COMPLEXITIES:
 *    Fibonacci DP:  O(n) time, O(1) space (optimized)
 *    Knapsack:      O(n * W) time, O(n * W) space
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_dp_module(void);
extern void clear_input(void);

/* ==================================================================
 *  FIBONACCI (Bottom-Up DP)
 * ==================================================================
 *
 *  Instead of recursing from the top (which recomputes values),
 *  we build from the bottom up. Start with F(0) and F(1),
 *  then compute F(2), F(3), ... F(n).
 *
 *  Table for n=6:
 *    i:   0  1  2  3  4  5  6
 *    dp:  0  1  1  2  3  5  8
 *              ^
 *         dp[2] = dp[1] + dp[0] = 1 + 0 = 1
 *
 *  REAL-WORLD: Computing large Fibonacci numbers for
 *  financial models, algorithm analysis.
 * ================================================================== */
static long long fib_bottom_up(int n)
{
    if (n <= 1) return (long long)n;

    /* Allocate DP table */
    long long *dp = (long long*)malloc((size_t)(n + 1) * sizeof(long long));
    if (!dp) return -1;

    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; ++i)
        dp[i] = dp[i - 1] + dp[i - 2];

    long long result = dp[n];
    free(dp);  /* Clean up */
    return result;
}

/*
 * Even more optimized: only keep the last two values.
 * This is the "sliding window" technique — O(1) space!
 */
static long long fib_optimized(int n)
{
    if (n <= 1) return (long long)n;

    long long prev2 = 0;  /* F(0) */
    long long prev1 = 1;  /* F(1) */
    long long current;

    for (int i = 2; i <= n; ++i) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}

static void demo_fibonacci_dp(void)
{
    int n;
    printf("  Enter n: ");
    scanf("%d", &n);

    printf("  >> Bottom-up DP (O(n) space):\n");
    printf("     fib(%d) = %lld\n", n, fib_bottom_up(n));

    printf("  >> Optimized (O(1) space):\n");
    printf("     fib(%d) = %lld\n", n, fib_optimized(n));

    printf("  Lesson: Bottom-up builds a table from base cases.\n");
    printf("     The optimized version keeps only the last 2 values.\n\n");
}

/* ==================================================================
 *  0/1 KNAPSACK PROBLEM
 * ==================================================================
 *
 *  PROBLEM:
 *    Given items with weights and values, and a knapsack capacity W,
 *    choose items to maximize total value. Each item at most once.
 *
 *  DP TABLE (dp[i][w] = max value using first i items, weight <= w):
 *
 *  Items:        0: (w=2, v=3)  1: (w=3, v=4)  2: (w=4, v=5)  3: (w=5, v=6)
 *  Capacity: 8
 *
 *         w->  0  1  2  3  4  5  6  7  8
 *  i=0:      [0, 0, 0, 0, 0, 0, 0, 0, 0]   no items
 *  i=1:      [0, 0, 3, 3, 3, 3, 3, 3, 3]   item 0
 *  i=2:      [0, 0, 3, 4, 4, 7, 7, 7, 7]   items 0-1
 *  i=3:      [0, 0, 3, 4, 5, 7, 8, 9, 9]   items 0-2
 *  i=4:      [0, 0, 3, 4, 5, 7, 8, 9, 10]  items 0-3
 *
 *  Answer: dp[4][8] = 10
 *
 *  DECISION at each cell: "take or skip?"
 *    If item is too heavy -> skip (carry forward previous value)
 *    Otherwise -> max(skip, take) = max(dp[i-1][w], v_i + dp[i-1][w - w_i])
 *
 *  REAL-WORLD:
 *    - Shipping: maximize cargo value within weight limit
 *    - Budgeting: maximize value with limited money
 *    - Scheduling: maximize profit with limited time
 *    - Investment: maximize returns with limited capital
 * ================================================================== */
static void knapsack_demo(void)
{
    /* Item structure: weight and value */
    int weights[] = {2, 3, 4, 5};
    int values[]  = {3, 4, 5, 6};
    int n = 4;           /* Number of items */
    int capacity = 8;    /* Knapsack capacity */

    /*
     * In C, we allocate a 2D array as a 1D array of pointers.
     * dp[i][w] = max value with first i items, weight <= w
     */
    int **dp = (int**)malloc((size_t)(n + 1) * sizeof(int*));
    for (int i = 0; i <= n; ++i)
        dp[i] = (int*)calloc((size_t)(capacity + 1), sizeof(int));

    if (!dp[0]) {
        printf("  Memory allocation failed!\n");
        /* Clean up partial allocations */
        for (int i = 0; i <= n; ++i) free(dp[i]);
        free(dp);
        return;
    }

    /* Build the DP table */
    for (int i = 1; i <= n; ++i) {
        int w_i = weights[i - 1];
        int v_i = values[i - 1];

        for (int w = 1; w <= capacity; ++w) {
            if (w_i > w) {
                /* Item too heavy -> carry forward previous value */
                dp[i][w] = dp[i - 1][w];
            } else {
                /*
                 * Can take or skip -> choose the BETTER option
                 * Option 1: Skip -> dp[i-1][w]
                 * Option 2: Take -> v_i + dp[i-1][w - w_i]
                 */
                int skip = dp[i - 1][w];
                int take = v_i + dp[i - 1][w - w_i];
                dp[i][w] = (take > skip) ? take : skip;
            }
        }
    }

    /* Print the DP table */
    printf("  DP Table:\n       ");
    for (int w = 0; w <= capacity; ++w)
        printf(" %2d", w);
    printf("\n");

    for (int i = 0; i <= n; ++i) {
        printf("  i=%d: [", i);
        for (int w = 0; w <= capacity; ++w) {
            printf("%2d", dp[i][w]);
            if (w < capacity) printf(", ");
        }
        printf("]\n");
    }

    printf("\n  << Maximum value: %d\n", dp[n][capacity]);

    /* Backtrack to find which items were selected */
    printf("  Selected items: ");
    int w = capacity;
    for (int i = n; i > 0 && w > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            printf("%d (w=%d, v=%d)  ", i - 1, weights[i - 1], values[i - 1]);
            w -= weights[i - 1];
        }
    }
    printf("\n");

    printf("  Lesson: At each cell, we decide 'take or skip'.\n");
    printf("     The optimal solution emerges from optimal\n");
    printf("     sub-solutions (optimal substructure).\n\n");

    /* Clean up */
    for (int i = 0; i <= n; ++i) free(dp[i]);
    free(dp);
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_dp_module(void)
{
    int choice;

    printf("\n  --- Dynamic Programming ---\n");
    printf("  1) Fibonacci (Bottom-Up DP)\n");
    printf("  2) 0/1 Knapsack\n");
    printf("  3) Both\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 3) {
        printf("  >> FIBONACCI (DP approach)\n");
        demo_fibonacci_dp();
    }

    if (choice == 2 || choice == 3) {
        printf("  >> 0/1 KNAPSACK PROBLEM\n");
        knapsack_demo();
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
