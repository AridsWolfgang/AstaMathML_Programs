/**
 * ===================================================================
 *  MODULE 5: GREEDY ALGORITHMS
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - GPS Navigation: Dijkstra's algorithm (greedy for shortest paths)
 *   - Scheduling: Meeting room booking (maximize number of meetings)
 *   - Compression: Huffman coding (build optimal prefix codes)
 *   - Networking: Minimum spanning tree (connect all nodes with least cost)
 *   - Vending Machines: Coin change with canonical coin systems
 *   - Task Scheduling: CPU task prioritization
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - The greedy choice property: make the best LOCAL choice, hope for GLOBAL optimum
 *   - When greedy works (and when it doesn't!)
 *   - How to prove a greedy algorithm is optimal
 *   - The difference between greedy and DP
 *
 * ⏱ COMPLEXITIES:
 *   Activity Selection: O(n log n) for sorting + O(n) for selection
 *   Coin Change:        O(1) or O(n) depending on coin system
 * ===================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>

void runGreedyModule();

// ==================================================================
//  ACTIVITY SELECTION PROBLEM
// ==================================================================
//
//  PROBLEM:
//   Given activities with start and end times, select the maximum
//   number of activities that don't overlap.
//
//  VISUAL:
//   Activities: A(1,3) B(2,5) C(3,7) D(5,9) E(6,8)
//
//   Timeline: 1--2--3--4--5--6--7--8--9--10
//   A:        [====]
//   B:           [========]
//   C:              [==========]
//   D:                    [==========]
//   E:                       [======]
//
//   Optimal selection: A(1,3), D(5,9) or A(1,3), E(6,8)
//   Why not A, C? Because C overlaps with both D and E.
//
//  GREEDY STRATEGY: Always pick the activity with the EARLIEST FINISH TIME.
//  This leaves the most room for remaining activities.
//
//  REAL-WORLD USE:
//   - Scheduling meetings in a conference room
//   - CPU process scheduling (Shortest Job First is similar)
//   - TV/Radio program scheduling
//   - Task planning with deadline constraints
// ==================================================================

struct Activity {
    int start;
    int end;
};

// Sort by end time (ascending) — the greedy criterion
void activitySelection()
{
    std::vector<Activity> activities = {
        {1, 3}, {2, 5}, {3, 7}, {5, 9}, {6, 8}
    };

    std::cout << "  Activities:\n";
    for (size_t i = 0; i < activities.size(); ++i) {
        std::cout << "    " << i << ": [" << activities[i].start
                  << ", " << activities[i].end << "]\n";
    }

    // Sort by end time (the greedy choice is always the earliest finish)
    std::sort(activities.begin(), activities.end(),
              [](const Activity& a, const Activity& b) {
                  return a.end < b.end;
              });

    std::cout << "\n  Sorted by end time:\n";
    for (size_t i = 0; i < activities.size(); ++i) {
        std::cout << "    [" << activities[i].start
                  << ", " << activities[i].end << "]\n";
    }

    // Greedy selection
    std::cout << "\n  ▶ Selected activities:\n";

    int lastEnd = -1;
    int count = 0;

    for (size_t i = 0; i < activities.size(); ++i) {
        if (activities[i].start >= lastEnd) {
            // This activity doesn't overlap — SELECT IT!
            std::cout << "    ✅ [" << activities[i].start
                      << ", " << activities[i].end << "]\n";
            lastEnd = activities[i].end;
            ++count;
        } else {
            std::cout << "    ❌ [" << activities[i].start
                      << ", " << activities[i].end << "] (overlaps)\n";
        }
    }

    std::cout << "  Total selected: " << count << " out of "
              << activities.size() << "\n";
    std::cout << "  📝 Why earliest finish? It maximizes remaining time.\n";
    std::cout << "     This is the 'greedy choice property' in action.\n\n";
}

// ==================================================================
//  COIN CHANGE (Greedy vs. DP)
// ==================================================================
//
//  PROBLEM:
//   Given coin denominations and a target amount, find the minimum
//   number of coins needed.
//
//  GREEDY APPROACH:
//   Always use the largest coin denomination that fits.
//   Works for "canonical" systems (e.g., USD: 25, 10, 5, 1).
//   FAILS for non-canonical systems (e.g., {1, 3, 4}, target=6).
//     Greedy: 4 + 1 + 1 = 3 coins
//     Optimal: 3 + 3 = 2 coins
//
//  REAL-WORLD USE:
//   - Vending machines (USD, EUR are canonical)
//   - Cashier's algorithm for making change
//   - Resource allocation with standard units
//
//  📝 THE KEY LESSON:
//   Greedy is NOT always correct! Always verify the problem
//   has the "greedy choice property" before using it.
// ==================================================================

void coinChangeDemo()
{
    // Canonical coin system (USD-like)
    std::vector<int> coins = {25, 10, 5, 1};
    int amount = 63;

    std::cout << "  Coins available: {25, 10, 5, 1}\n";
    std::cout << "  Target amount: " << amount << "\n\n";

    std::cout << "  ▶ Greedy approach:\n";

    int remaining = amount;
    int totalCoins = 0;

    for (int coin : coins) {
        if (remaining <= 0) break;

        int numCoins = remaining / coin;  // Use as many of this coin as possible
        if (numCoins > 0) {
            std::cout << "     " << coin << "¢ × " << numCoins << " = "
                      << (coin * numCoins) << "¢  ("
                      << numCoins << " coin" << (numCoins > 1 ? "s" : "") << ")\n";
            remaining -= coin * numCoins;
            totalCoins += numCoins;
        }
    }

    std::cout << "  Total coins: " << totalCoins << "\n";
    std::cout << "  📝 Greedy works for USD because each coin\n";
    std::cout << "     is at least twice the next smaller one.\n";

    // Show a counterexample where greedy FAILS
    std::cout << "\n  ⚠️ Counterexample: coins {1, 3, 4}, amount=6\n";
    std::cout << "     Greedy: 4 + 1 + 1 = 3 coins\n";
    std::cout << "     Optimal: 3 + 3 = 2 coins  ← DP needed!\n";
    std::cout << "     📝 Always check if greedy is correct for YOUR problem.\n\n";
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runGreedyModule()
{
    int choice;

    std::cout << "\n  --- Greedy Algorithms ---\n";
    std::cout << "  1) Activity Selection\n";
    std::cout << "  2) Coin Change (Greedy)\n";
    std::cout << "  3) Both\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    std::cout << "\n";

    if (choice == 1 || choice == 3) {
        std::cout << "  ▶ ACTIVITY SELECTION\n";
        activitySelection();
    }

    if (choice == 2 || choice == 3) {
        std::cout << "  ▶ COIN CHANGE\n";
        coinChangeDemo();
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
