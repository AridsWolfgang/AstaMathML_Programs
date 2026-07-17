/**
 * ===================================================================
 *  MODULE 4: DYNAMIC PROGRAMMING (DP)
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - Navigation Apps: Find shortest path (Google Maps, Waze)
 *   - Bioinformatics: DNA sequence alignment
 *   - Resource Allocation: Budget optimization with constraints
 *   - Scheduling: Maximize profit with limited time slots
 *   - Inventory Management: Optimal stock levels under constraints
 *   - AI: Reinforcement learning (value iteration is DP)
 *   - Finance: Portfolio optimization, option pricing
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - The core DP mantra: "Solve subproblems once, reuse the answer"
 *   - Top-down (memoization) vs. bottom-up (tabulation) approaches
 *   - How to identify optimal substructure in problems
 *   - The two signs DP might apply: optimal substructure + overlapping subproblems
 *   - Why naive recursion becomes exponential without memoization
 *
 * ⏱ COMPLEXITIES:
 *   Fibonacci DP:     O(n) time, O(n) space
 *   Knapsack:         O(n × W) time, O(n × W) space
 * ===================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>

void runDPModule();

// ==================================================================
//  FIBONACCI (already covered in recursion, shown here as pure DP)
// ==================================================================
//
//  BOTTOM-UP approach: start from the base and build up.
//  No recursion overhead, no stack overflow risk.
//
//  Table illustration for n=6:
//
//  Index: 0  1  2  3  4  5  6
//         ↓  ↓  ↓  ↓  ↓  ↓  ↓
//  Value: 0  1  1  2  3  5  8
//              ↑
//            dp[2] = dp[1] + dp[0] = 1 + 0 = 1
//                    ↑
//                 dp[3] = dp[2] + dp[1] = 1 + 1 = 2
//  ...
//
//  REAL-WORLD USE:
//   - Computing large Fibonacci numbers (finance, nature)
//   - Teaching the concept of "build from base cases"
// ==================================================================

long long fibBottomUp(int n)
{
    if (n <= 1) return n;

    // dp[i] = Fibonacci number at position i
    std::vector<long long> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];  // Recurrence: F(n) = F(n-1) + F(n-2)
    }

    return dp[n];
}

// Even more optimized: only keep the last two values
long long fibOptimized(int n)
{
    if (n <= 1) return n;

    long long prev2 = 0;  // F(0)
    long long prev1 = 1;  // F(1)
    long long current;

    for (int i = 2; i <= n; ++i) {
        current = prev1 + prev2;  // F(n) = F(n-1) + F(n-2)
        prev2 = prev1;            // Shift: old F(n-1) becomes new F(n-2)
        prev1 = current;          // Shift: old F(n) becomes new F(n-1)
    }

    return current;
}

void demoFibonacciDP()
{
    int n;
    std::cout << "  Enter n: ";
    std::cin >> n;

    std::cout << "  ▶ Bottom-up DP:\n";
    std::cout << "     fib(" << n << ") = " << fibBottomUp(n) << "\n";

    std::cout << "  ▶ Optimized (O(1) space):\n";
    std::cout << "     fib(" << n << ") = " << fibOptimized(n) << "\n";

    std::cout << "  📝 Bottom-up builds a table from base cases.\n";
    std::cout << "     The optimized version only keeps last 2 values.\n\n";
}

// ==================================================================
//  0/1 KNAPSACK PROBLEM
// ==================================================================
//
//  PROBLEM:
//   Given items each with a weight and value, and a knapsack with
//   weight capacity W, choose items to maximize total value.
//   Each item can be taken at most once (0 or 1).
//
//  EXAMPLE:
//   Items:     (weight, value)
//           0:  (2, 3)
//           1:  (3, 4)
//           2:  (4, 5)
//           3:  (5, 6)
//   Capacity: 8
//
//  DECISION TABLE (DP[i][w] = max value with first i items, weight ≤ w):
//
//           w → 0  1  2  3  4  5  6  7  8
//   i=0:   [0, 0, 0, 0, 0, 0, 0, 0, 0]   no items
//   i=1:   [0, 0, 3, 3, 3, 3, 3, 3, 3]   item 0 (w=2, v=3)
//   i=2:   [0, 0, 3, 4, 4, 7, 7, 7, 7]   items 0,1
//   i=3:   [0, 0, 3, 4, 5, 7, 8, 9, 9]   items 0,1,2
//   i=4:   [0, 0, 3, 4, 5, 7, 8, 9, 10]  items 0,1,2,3
//
//  Answer: dp[4][8] = 10
//
//  REAL-WORLD USE:
//   - Budgeting: maximize value given limited money
//   - Shipping: maximize cargo value given weight limit
//   - CPU Scheduling: maximize throughput given time limit
//   - Investment: maximize returns with limited capital
// ==================================================================

struct Item {
    int weight;
    int value;
};

void knapsackDemo()
{
    // Sample items
    std::vector<Item> items = {
        {2, 3},   // item 0: weight=2, value=3
        {3, 4},   // item 1: weight=3, value=4
        {4, 5},   // item 2: weight=4, value=5
        {5, 6}    // item 3: weight=5, value=6
    };

    int capacity = 8;
    int n = static_cast<int>(items.size());

    // dp[i][w] = maximum value achievable with first i items and max weight w
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

    // Build the DP table
    for (int i = 1; i <= n; ++i) {
        int w_i = items[i - 1].weight;
        int v_i = items[i - 1].value;

        for (int w = 1; w <= capacity; ++w) {
            if (w_i > w) {
                // Item too heavy — can't take it, carry forward previous value
                dp[i][w] = dp[i - 1][w];
            } else {
                // Can take or skip — choose the BETTER option
                // Option 1: Skip this item → dp[i-1][w]
                // Option 2: Take this item → v_i + dp[i-1][w - w_i]
                dp[i][w] = std::max(dp[i - 1][w], v_i + dp[i - 1][w - w_i]);
            }
        }
    }

    // Print the DP table
    std::cout << "  DP Table:\n";
    std::cout << "       ";
    for (int w = 0; w <= capacity; ++w)
        std::cout << " " << w << " ";
    std::cout << "\n";

    for (int i = 0; i <= n; ++i) {
        std::cout << "  i=" << i << ": [";
        for (int w = 0; w <= capacity; ++w) {
            std::cout << dp[i][w];
            if (w < capacity) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    std::cout << "\n  ✅ Maximum value: " << dp[n][capacity] << "\n";

    // Backtrack to find which items were selected
    std::cout << "  Selected items: ";
    int w = capacity;
    for (int i = n; i > 0 && w > 0; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            // This item was taken
            std::cout << (i - 1) << " (w=" << items[i - 1].weight
                      << ", v=" << items[i - 1].value << ")  ";
            w -= items[i - 1].weight;
        }
    }
    std::cout << "\n";

    std::cout << "  📝 Key insight: At each cell, we decide 'take or skip'.\n";
    std::cout << "     The optimal solution emerges from optimal sub-solutions.\n";
    std::cout << "     This is the 'optimal substructure' property of DP.\n\n";
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runDPModule()
{
    int choice;

    std::cout << "\n  --- Dynamic Programming ---\n";
    std::cout << "  1) Fibonacci (Bottom-Up DP)\n";
    std::cout << "  2) 0/1 Knapsack\n";
    std::cout << "  3) Both\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    std::cout << "\n";

    if (choice == 1 || choice == 3) {
        std::cout << "  ▶ FIBONACCI (DP approach)\n";
        demoFibonacciDP();
    }

    if (choice == 2 || choice == 3) {
        std::cout << "  ▶ 0/1 KNAPSACK PROBLEM\n";
        knapsackDemo();
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
