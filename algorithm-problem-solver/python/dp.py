"""
=====================================================================
 MODULE 4: DYNAMIC PROGRAMMING  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - Navigation: Shortest path (Google Maps)
  - Bioinformatics: DNA sequence alignment
  - Resource allocation: Budget optimization
  - Finance: Portfolio optimization, option pricing
  - AI: Reinforcement learning (Bellman equations)

THE CORE IDEA:
  "Solve each subproblem once, store the answer, reuse it."

COMPLEXITIES:
  Fibonacci DP:  O(n) time, O(1) space (optimized)
  Knapsack:      O(n * W) time, O(n * W) space
=====================================================================
"""


# =================================================================
#  FIBONACCI (Bottom-Up DP)
# =================================================================
#
#  Build from the bottom up:
#    i:   0  1  2  3  4  5  6
#    dp:  0  1  1  2  3  5  8
#
#  dp[2] = dp[1] + dp[0] = 1 + 0 = 1
#  dp[3] = dp[2] + dp[1] = 1 + 1 = 2
#  ...
#
#  REAL-WORLD: Large Fibonacci numbers, teaching DP basics.
# =================================================================
def fib_bottom_up(n):
    if n <= 1:
        return n

    dp = [0] * (n + 1)
    dp[0] = 0
    dp[1] = 1

    for i in range(2, n + 1):
        dp[i] = dp[i - 1] + dp[i - 2]

    return dp[n]


def fib_optimized(n):
    """O(1) space — only keep last two values."""
    if n <= 1:
        return n

    prev2, prev1 = 0, 1
    for _ in range(2, n + 1):
        current = prev1 + prev2
        prev2, prev1 = prev1, current

    return prev1


def demo_fibonacci_dp():
    n = int(input("  Enter n: "))

    print(f"  >> Bottom-up DP (O(n) space):")
    print(f"     fib({n}) = {fib_bottom_up(n)}")

    print(f"  >> Optimized (O(1) space):")
    print(f"     fib({n}) = {fib_optimized(n)}")

    print("  Lesson: Bottom-up builds a table from base cases.")
    print("     The optimized version keeps only last 2 values.\n")


# =================================================================
#  0/1 KNAPSACK PROBLEM
# =================================================================
#
#  PROBLEM: Items with (weight, value) and capacity W.
#  Maximize total value. Each item at most once.
#
#  DP TABLE:
#         w->  0  1  2  3  4  5  6  7  8
#  i=0:      [0, 0, 0, 0, 0, 0, 0, 0, 0]
#  i=1:      [0, 0, 3, 3, 3, 3, 3, 3, 3]   item 0 (w=2, v=3)
#  i=2:      [0, 0, 3, 4, 4, 7, 7, 7, 7]   items 0,1
#  i=3:      [0, 0, 3, 4, 5, 7, 8, 9, 9]   items 0,1,2
#  i=4:      [0, 0, 3, 4, 5, 7, 8, 9, 10]  items 0,1,2,3
#
#  DECISION: max(skip, take) = max(dp[i-1][w], v_i + dp[i-1][w-w_i])
#
#  REAL-WORLD: Shipping, budgeting, scheduling, investment.
# =================================================================
def knapsack_demo():
    weights = [2, 3, 4, 5]
    values  = [3, 4, 5, 6]
    capacity = 8
    n = len(weights)

    # dp[i][w] = max value with first i items, weight <= w
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    for i in range(1, n + 1):
        w_i = weights[i - 1]
        v_i = values[i - 1]

        for w in range(1, capacity + 1):
            if w_i > w:
                dp[i][w] = dp[i - 1][w]  # Too heavy, skip
            else:
                skip = dp[i - 1][w]
                take = v_i + dp[i - 1][w - w_i]
                dp[i][w] = max(skip, take)

    # Print the DP table
    print("  DP Table:")
    print("       ", end="")
    for w in range(capacity + 1):
        print(f" {w:2}", end="")
    print()

    for i in range(n + 1):
        print(f"  i={i}: {dp[i]}")

    print(f"\n  << Maximum value: {dp[n][capacity]}")

    # Backtrack to find selected items
    print("  Selected items: ", end="")
    w = capacity
    for i in range(n, 0, -1):
        if dp[i][w] != dp[i - 1][w]:
            print(f"{i - 1} (w={weights[i - 1]}, v={values[i - 1]})  ", end="")
            w -= weights[i - 1]
    print()

    print("  Lesson: At each cell we decide 'take or skip'.")
    print("     The optimal solution comes from optimal sub-solutions.\n")


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_dp_module():
    print("\n  --- Dynamic Programming ---")
    print("  1) Fibonacci (Bottom-Up DP)")
    print("  2) 0/1 Knapsack")
    print("  3) Both")
    choice = input("  Enter choice: ").strip()

    print()

    if choice in ("1", "3"):
        print("  >> FIBONACCI (DP approach)")
        demo_fibonacci_dp()

    if choice in ("2", "3"):
        print("  >> 0/1 KNAPSACK PROBLEM")
        knapsack_demo()

    input("  Press Enter to return to menu...")
