/*
 * ===================================================================
 *  MODULE 3: RECURSION  (C Edition)
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - File system traversal (recursive directory listing)
 *    - Parsing HTML/XML/JSON (recursive descent parser)
 *    - Compilers: expression parsing with nested parentheses
 *    - AI: Minimax algorithm for game trees (chess, tic-tac-toe)
 *    - Graphics: Fractals, recursive subdivision
 *
 *  THE THREE LAWS OF RECURSION:
 *    1. A recursive function must have a BASE CASE (stopping condition)
 *    2. It must call itself with a SMALLER version of the problem
 *    3. It must make PROGRESS toward the base case
 *
 *  COMPLEXITIES:
 *    Factorial:         O(n) time, O(n) stack space
 *    Fibonacci (naive): O(2^n) time — EXPONENTIAL!
 *    Fibonacci (memo):  O(n) time
 *    Towers of Hanoi:   O(2^n) time
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_recursion_module(void);
extern void clear_input(void);

/* ==================================================================
 *  FACTORIAL
 * ==================================================================
 *
 *  n! = n * (n-1) * (n-2) * ... * 1
 *
 *  Call stack visualization for fact(4):
 *
 *    fact(4) = 4 * fact(3)
 *      fact(3) = 3 * fact(2)
 *        fact(2) = 2 * fact(1)
 *          fact(1) = 1 * fact(0)
 *            fact(0) = 1          << BASE CASE
 *          returns 1 * 1 = 1
 *        returns 2 * 1 = 2
 *      returns 3 * 2 = 6
 *    returns 4 * 6 = 24
 *
 *  REAL-WORLD: Permutations (arrangements), combinations,
 *  probability calculations, algorithm analysis.
 * ================================================================== */
static int factorial(int n)
{
    /* BASE CASE: 0! = 1 by definition */
    if (n <= 1) return 1;

    /* RECURSIVE CASE: n! = n * (n-1)! */
    return n * factorial(n - 1);
}

/* ==================================================================
 *  FIBONACCI SEQUENCE
 * ==================================================================
 *
 *  F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)
 *  Sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
 *
 *  The naive recursive version is a GREAT example of why
 *  recursion alone is not enough — it recomputes the same
 *  values exponentially many times.
 *
 *  fib(5) calls fib(4) and fib(3)
 *    fib(4) calls fib(3) and fib(2)    <- fib(3) computed AGAIN
 *      fib(3) calls fib(2) and fib(1)  <- fib(2) computed AGAIN (and again)
 *        ...
 *
 *  REAL-WORLD: Population modeling, Fibonacci heap,
 *  stock market retracement levels, natural spirals.
 * ================================================================== */

/* Naive recursion — O(2^n). Simple but SLOW. */
static int fib_naive(int n, int *call_count)
{
    (*call_count)++;
    if (n <= 1) return n;
    return fib_naive(n - 1, call_count) + fib_naive(n - 2, call_count);
}

/* Memoized version — O(n). Stores results to avoid recomputation.
 * In C, we use a dynamically allocated array for the memo table. */
static int fib_memo(int n, int *memo, int *call_count)
{
    (*call_count)++;

    if (n <= 1) return n;

    /* If we've already computed this value, return it immediately */
    if (memo[n] != -1) return memo[n];

    /* Compute and store for future use */
    memo[n] = fib_memo(n - 1, memo, call_count) +
              fib_memo(n - 2, memo, call_count);
    return memo[n];
}

static void demo_fibonacci(void)
{
    int n;
    printf("  Enter n to compute Fibonacci(n): ");
    scanf("%d", &n);

    /* Naive version */
    int call_count = 0;
    int result_naive = fib_naive(n, &call_count);

    printf("  >> NAIVE recursion:\n");
    printf("     fib(%d) = %d\n", n, result_naive);
    printf("     Function calls: %d (O(2^n))\n", call_count);

    /* Memoized version */
    call_count = 0;
    int *memo = (int*)malloc((size_t)(n + 1) * sizeof(int));
    if (!memo) {
        printf("     Memory allocation failed!\n");
        return;
    }
    /* Initialize memo table with -1 (meaning "not computed") */
    memset(memo, -1, (size_t)(n + 1) * sizeof(int));

    int result_memo = fib_memo(n, memo, &call_count);

    printf("  >> MEMOIZED recursion:\n");
    printf("     fib(%d) = %d\n", n, result_memo);
    printf("     Function calls: %d (O(n))\n", call_count);
    printf("     For n=%d, naive would need ~%lld calls!\n",
           n, (1LL << n));

    printf("  Lesson: Memoization turns exponential into linear.\n");
    printf("     This is the foundation of Dynamic Programming.\n\n");

    free(memo);  /* C programmers ALWAYS clean up */
}

/* ==================================================================
 *  TOWERS OF HANOI
 * ==================================================================
 *
 *  Rules:
 *    1. Move one disk at a time
 *    2. A larger disk cannot sit on a smaller disk
 *    3. Move all disks from peg A to peg C using peg B
 *
 *  For 3 disks:
 *
 *    Step 1: Move disk 1 from A to C
 *    Step 2: Move disk 2 from A to B
 *    Step 3: Move disk 1 from C to B
 *    Step 4: Move disk 3 from A to C
 *    Step 5: Move disk 1 from B to A
 *    Step 6: Move disk 2 from B to C
 *    Step 7: Move disk 1 from A to C
 *
 *  Total moves for n disks: 2^n - 1
 *
 *  REAL-WORLD: Backup rotation strategies, recursive
 *  problem decomposition in AI planning.
 * ================================================================== */
static void towers_of_hanoi(int n, char from, char to, char aux,
                            int *move_count, int show_moves)
{
    if (n == 1) {
        (*move_count)++;
        if (show_moves)
            printf("     Move disk 1 from %c to %c\n", from, to);
        return;
    }

    /* Move n-1 disks from 'from' to 'aux' */
    towers_of_hanoi(n - 1, from, aux, to, move_count, show_moves);

    /* Move the largest disk */
    (*move_count)++;
    if (show_moves)
        printf("     Move disk %d from %c to %c\n", n, from, to);

    /* Move n-1 disks from 'aux' to 'to' */
    towers_of_hanoi(n - 1, aux, to, from, move_count, show_moves);
}

static void demo_hanoi(void)
{
    int n;
    printf("  Enter number of disks: ");
    scanf("%d", &n);

    int move_count = 0;
    int show_moves = (n <= 5);  /* Only show for small inputs */

    if (!show_moves)
        printf("  (Moves hidden for n > 5 - too many!)\n");

    towers_of_hanoi(n, 'A', 'C', 'B', &move_count, show_moves);

    printf("  Total moves: %d\n", move_count);
    printf("  Formula: 2^n - 1 = %d moves\n", (1 << n) - 1);
    printf("  Lesson: Each additional disk DOUBLES the work.\n");
    printf("     Exponential growth is FAST.\n\n");
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_recursion_module(void)
{
    int choice;

    printf("\n  --- Recursion ---\n");
    printf("  1) Factorial\n");
    printf("  2) Fibonacci (naive vs. memoized)\n");
    printf("  3) Towers of Hanoi\n");
    printf("  4) Run All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 4) {
        int n;
        printf("  >> FACTORIAL\n");
        printf("  Enter a number: ");
        scanf("%d", &n);
        printf("  %d! = %d\n", n, factorial(n));
        printf("  Lesson: Recursive definition: n! = n * (n-1)!\n\n");
    }

    if (choice == 2 || choice == 4) {
        printf("  >> FIBONACCI\n");
        demo_fibonacci();
    }

    if (choice == 3 || choice == 4) {
        printf("  >> TOWERS OF HANOI\n");
        demo_hanoi();
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
