"""
=====================================================================
 MODULE 5: GREEDY ALGORITHMS  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - Dijkstra: Shortest path (greedy at each step)
  - Huffman coding: Data compression
  - MST (Kruskal, Prim): Network design
  - Scheduling: Maximize meetings in a room
  - Vending machines: Change with USD/EUR

THE GREEDY MANTRA:
  "Make the best LOCAL choice, hope for GLOBAL optimum."
  Sometimes correct. Sometimes not. Prove it first!

COMPLEXITIES:
  Activity Selection: O(n log n) for sorting + O(n) selection
  Coin Change:        O(n) with canonical systems
=====================================================================
"""


# =================================================================
#  ACTIVITY SELECTION
# =================================================================
#
#  PROBLEM: Maximize non-overlapping activities.
#  GREEDY: Always pick the activity with EARLIEST FINISH TIME.
#
#  Visual:
#    A: [1,3]  B: [2,5]  C: [3,7]  D: [5,9]  E: [6,8]
#
#  Greedy picks A(1,3), then D(5,9) [or E(6,8)]
#
#  REAL-WORLD: Conference room scheduling, CPU job scheduling.
# =================================================================
def activity_selection():
    activities = [
        (1, 3), (2, 5), (3, 7), (5, 9), (6, 8)
    ]

    print("  Activities:")
    for i, (s, e) in enumerate(activities):
        print(f"    {i}: [{s}, {e}]")

    # Sort by end time (the greedy criterion)
    sorted_acts = sorted(activities, key=lambda x: x[1])

    print("\n  Sorted by end time:")
    for s, e in sorted_acts:
        print(f"    [{s}, {e}]")

    # Greedy selection
    print("\n  >> Selected activities:")

    last_end = -1
    count = 0

    for s, e in sorted_acts:
        if s >= last_end:
            print(f"    << [{s}, {e}] SELECTED")
            last_end = e
            count += 1
        else:
            print(f"    xx [{s}, {e}] (overlaps, skipped)")

    print(f"  Total selected: {count} out of {len(activities)}")
    print("  Lesson: Earliest finish time maximizes remaining room.")
    print("     This is the 'greedy choice property'.\n")


# =================================================================
#  COIN CHANGE (Greedy)
# =================================================================
#
#  PROBLEM: Given coin denominations and target amount, find
#  minimum number of coins.
#
#  GREEDY: Always use the LARGEST coin that fits.
#
#  WORKS for canonical systems (USD: 25, 10, 5, 1)
#  FAILS for {1, 3, 4}, target=6:
#    Greedy: 4 + 1 + 1 = 3 coins
#    Optimal: 3 + 3 = 2 coins
#
#  REAL-WORLD: Vending machines, cash registers.
#  Lesson: GREEDY IS NOT ALWAYS CORRECT!
# =================================================================
def coin_change_demo():
    coins = [25, 10, 5, 1]
    amount = 63

    print(f"  Coins available: {coins}")
    print(f"  Target amount: {amount}\n")
    print("  >> Greedy approach:")

    remaining = amount
    total_coins = 0

    for coin in coins:
        if remaining <= 0:
            break

        num_coins = remaining // coin
        if num_coins > 0:
            plural = "s" if num_coins > 1 else ""
            print(f"     {coin} cent(s) x {num_coins} = {coin * num_coins} cents  ({num_coins} coin{plural})")
            remaining -= coin * num_coins
            total_coins += num_coins

    print(f"  Total coins: {total_coins}")
    print("  Lesson: Greedy works for USD because each coin")
    print("     is at least twice the next smaller one.")

    print("\n  ** Counterexample: coins {1, 3, 4}, amount=6")
    print("     Greedy: 4 + 1 + 1 = 3 coins")
    print("     Optimal: 3 + 3 = 2 coins  (DP needed!)")
    print("     Lesson: Always verify if greedy is correct")
    print("     for YOUR specific problem.\n")


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_greedy_module():
    print("\n  --- Greedy Algorithms ---")
    print("  1) Activity Selection")
    print("  2) Coin Change (Greedy)")
    print("  3) Both")
    choice = input("  Enter choice: ").strip()

    print()

    if choice in ("1", "3"):
        print("  >> ACTIVITY SELECTION")
        activity_selection()

    if choice in ("2", "3"):
        print("  >> COIN CHANGE")
        coin_change_demo()

    input("  Press Enter to return to menu...")
