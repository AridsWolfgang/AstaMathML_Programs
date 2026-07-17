/**
 * ===================================================================
 *  MODULE 3: RECURSION
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - File System: Traversing nested folders (recursive directory listing)
 *   - Parsing: HTML/XML/JSON parsers use recursive descent
 *   - AI: Minimax algorithm for game-playing (chess, tic-tac-toe)
 *   - Graphics: Fractals (Mandelbrot, tree branching)
 *   - Compilers: Parsing expressions with nested parentheses
 *   - Divide & Conquer: Quick Sort, Merge Sort (see Module 1)
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - The three essential parts of every recursive function
 *   - How the call stack works (and what happens with infinite recursion)
 *   - Why recursion isn't always efficient (Fibonacci naive vs. DP)
 *   - How to convert between recursion and iteration
 *   - The "recursive leap of faith" — trust that the recursive call works
 *
 * ⏱ COMPLEXITIES:
 *   Factorial:         O(n) time, O(n) stack space
 *   Fibonacci (naive): O(2^n) time — EXPONENTIAL! Slow.
 *   Fibonacci (DP):    O(n) time — linear.
 *   Towers of Hanoi:   O(2^n) time
 * ===================================================================
 */

#include <iostream>
#include <vector>
#include <string>

void runRecursionModule();

// ==================================================================
//  WHAT IS RECURSION?
// ==================================================================
//
//  A function that calls itself, with three essential parts:
//
//   1. BASE CASE:  The simplest version of the problem (stops recursion)
//   2. RECURSIVE CASE:  Function calls itself with a SMALLER problem
//   3. PROGRESS:  Each call moves closer to the base case
//
//  Visualizing the call stack for factorial(4):
//
//  Call 1: factorial(4) = 4 * factorial(3)
//    Call 2: factorial(3) = 3 * factorial(2)
//      Call 3: factorial(2) = 2 * factorial(1)
//        Call 4: factorial(1) = 1 * factorial(0)
//          Call 5: factorial(0) = 1          ← BASE CASE reached!
//        Call 4 returns: 1 * 1 = 1
//      Call 3 returns: 2 * 1 = 2
//    Call 2 returns: 3 * 2 = 6
//  Call 1 returns: 4 * 6 = 24
//
//  Each call waits for the next call to finish — like stacking plates.
// ==================================================================

// ==================================================================
//  FACTORIAL
// ==================================================================
//
//  n! = n × (n-1) × (n-2) × ... × 1
//  Example: 5! = 5 × 4 × 3 × 2 × 1 = 120
//
//  REAL-WORLD USE:
//   - Permutations: How many ways to arrange n items?
//   - Combinations: nCr = n! / (r! × (n-r)!)
//   - Probability calculations
// ==================================================================

int factorial(int n)
{
    // BASE CASE: 0! = 1 (by definition)
    if (n <= 1) return 1;

    // RECURSIVE CASE: n! = n × (n-1)!
    return n * factorial(n - 1);
}

// ==================================================================
//  FIBONACCI SEQUENCE
// ==================================================================
//
//  Each term is the sum of the two previous terms:
//  0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
//
//  RECURRENCE: F(0) = 0, F(1) = 1, F(n) = F(n-1) + F(n-2)
//
//  REAL-WORLD USE:
//   - Population growth modeling (rabbit breeding problem)
//   - Stock market analysis (Fibonacci retracement)
//   - Algorithm analysis (Fibonacci heap, Fibonacci search)
//   - Nature: spiral patterns in shells, flower petals
// ==================================================================

// NAIVE recursion — simple but SLOW (O(2^n))
// Why? It recomputes the same values over and over!
// fib(5) calls fib(4) and fib(3). fib(4) calls fib(3) and fib(2)...
// fib(3) is computed TWICE. fib(2) is computed THREE times.
int fibNaive(int n, int& callCount)
{
    ++callCount;
    if (n <= 1) return n;
    return fibNaive(n - 1, callCount) + fibNaive(n - 2, callCount);
}

// DP version — uses memoization to cache results (O(n))
// Like taking notes so you never solve the same subproblem twice.
int fibDP(int n, std::vector<int>& memo, int& callCount)
{
    ++callCount;

    // Base case
    if (n <= 1) return n;

    // If already computed, return memoized result (NO RECURSION NEEDED)
    if (memo[n] != -1) return memo[n];

    // Compute and store for future use
    memo[n] = fibDP(n - 1, memo, callCount) + fibDP(n - 2, memo, callCount);
    return memo[n];
}

void demoFibonacci()
{
    int n;
    std::cout << "  Enter n to compute Fibonacci(n): ";
    std::cin >> n;

    int callCount = 0;
    int resultNaive = fibNaive(n, callCount);

    std::cout << "  ▶ NAIVE recursion:\n";
    std::cout << "     fib(" << n << ") = " << resultNaive << "\n";
    std::cout << "     Function calls: " << callCount << " (O(2^n))\n";

    callCount = 0;
    std::vector<int> memo(n + 1, -1);
    int resultDP = fibDP(n, memo, callCount);

    std::cout << "  ▶ DP with memoization:\n";
    std::cout << "     fib(" << n << ") = " << resultDP << "\n";
    std::cout << "     Function calls: " << callCount << " (O(n))\n";

    std::cout << "  📝 For n=" << n
              << ", naive calls=" << callCount
              << " would be ~" << (1 << n) << " without memoization!\n";
    std::cout << "     This is WHY Dynamic Programming exists —\n";
    printf("     to avoid recomputing the same subproblems.\n\n");
}

// ==================================================================
//  TOWERS OF HANOI
// ==================================================================
//
//  RULES:
//   1. Move one disk at a time
//   2. A larger disk cannot sit on a smaller disk
//   3. Move all disks from peg A to peg C using peg B as helper
//
//  VISUALIZATION for 3 disks:
//
//  Initial:        Step 1:         Step 2:
//   |  |  |         |  |  |         |  |  |
//  _|_ |  |        |  |  |        _|_ |  |
// __|__|  |       _|_ |  |        |  |  |
// ___|___|___     ___|___|___     ___|___|___
//  A  B  C         A  B  C         A  B  C
//  (start)         (1→C)           (2→B)
//
//  REAL-WORLD USE:
//   - Backup rotation strategies
//   - Recursive problem-solving in AI planning
//   - Teaching recursion conceptually
// ==================================================================

void towersOfHanoi(int n, char from, char to, char aux, int& moveCount, bool showMoves)
{
    if (n == 1) {
        ++moveCount;
        if (showMoves) {
            std::cout << "     Move disk 1 from " << from << " to " << to << "\n";
        }
        return;
    }

    // Move n-1 disks from 'from' to 'aux' using 'to' as helper
    towersOfHanoi(n - 1, from, aux, to, moveCount, showMoves);

    // Move the largest disk from 'from' to 'to'
    ++moveCount;
    if (showMoves) {
        std::cout << "     Move disk " << n << " from " << from << " to " << to << "\n";
    }

    // Move n-1 disks from 'aux' to 'to' using 'from' as helper
    towersOfHanoi(n - 1, aux, to, from, moveCount, showMoves);
}

void demoTowersOfHanoi()
{
    int n;
    std::cout << "  Enter number of disks: ";
    std::cin >> n;

    int moveCount = 0;
    bool showMoves = (n <= 5);  // Only show moves for small inputs

    if (!showMoves) {
        std::cout << "  (Moves hidden for n > 5 — too many!)\n";
    }

    towersOfHanoi(n, 'A', 'C', 'B', moveCount, showMoves);

    std::cout << "  Total moves needed: " << moveCount << "\n";
    std::cout << "  Formula: 2^n - 1 = " << ((1 << n) - 1) << " moves\n";
    std::cout << "  📝 Each disk doubles the work — exponential growth!\n\n";
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runRecursionModule()
{
    int choice;

    std::cout << "\n  --- Recursion ---\n";
    std::cout << "  1) Factorial\n";
    std::cout << "  2) Fibonacci (naive vs. DP)\n";
    std::cout << "  3) Towers of Hanoi\n";
    std::cout << "  4) Run All\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        int n;
        std::cout << "  ▶ FACTORIAL\n";
        std::cout << "  Enter a number: ";
        std::cin >> n;
        std::cout << "  " << n << "! = " << factorial(n) << "\n";
        std::cout << "  📝 Recursive definition: n! = n × (n-1)!\n\n";
    }

    if (choice == 2 || choice == 4) {
        std::cout << "  ▶ FIBONACCI\n";
        demoFibonacci();
    }

    if (choice == 3 || choice == 4) {
        std::cout << "  ▶ TOWERS OF HANOI\n";
        demoTowersOfHanoi();
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
