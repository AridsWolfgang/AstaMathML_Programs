/*
 * ===================================================================
 *  MODULE 5: GREEDY ALGORITHMS  (C Edition)
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - Dijkstra's algorithm (shortest path) — greedy at each step
 *    - Huffman coding (data compression) — optimal prefix codes
 *    - Minimum spanning tree (Kruskal, Prim) — network design
 *    - Scheduling: maximize number of meetings in a room
 *    - Vending machines: coin change with USD/EUR systems
 *
 *  THE GREEDY MANTRA:
 *    "Make the best local choice, hope for the global optimum."
 *    Sometimes it works. Sometimes it doesn't. Prove it first!
 *
 *  COMPLEXITIES:
 *    Activity Selection: O(n log n) for sorting + O(n) selection
 *    Coin Change:        O(n) with canonical coin systems
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void run_greedy_module(void);
extern void clear_input(void);

/* ==================================================================
 *  ACTIVITY SELECTION
 * ==================================================================
 *
 *  PROBLEM: Given activities with start and end times, select the
 *  maximum number that don't overlap.
 *
 *  GREEDY STRATEGY: Always pick the activity with the EARLIEST FINISH TIME.
 *  This leaves the most room for remaining activities.
 *
 *  Visual:
 *    A: [1,3]   B: [2,5]   C: [3,7]   D: [5,9]   E: [6,8]
 *
 *    Timeline: 1--2--3--4--5--6--7--8--9--10
 *    A:       [====]
 *    B:          [========]
 *    C:             [==========]
 *    D:                   [==========]
 *    E:                      [======]
 *
 *  Greedy picks A(1,3), then D(5,9) [or E(6,8)]
 *
 *  REAL-WORLD: Meeting room scheduling, CPU job scheduling,
 *  TV program scheduling, task planning.
 * ================================================================== */

struct Activity {
    int start;
    int end;
};

/* Comparison function for qsort: sort by end time ascending */
static int cmp_activity(const void *a, const void *b)
{
    return ((const struct Activity*)a)->end -
           ((const struct Activity*)b)->end;
}

static void activity_selection(void)
{
    struct Activity activities[] = {
        {1, 3}, {2, 5}, {3, 7}, {5, 9}, {6, 8}
    };
    int n = sizeof(activities) / sizeof(activities[0]);

    printf("  Activities:\n");
    for (int i = 0; i < n; ++i)
        printf("    %d: [%d, %d]\n", i, activities[i].start, activities[i].end);

    /* Sort by end time (the greedy choice criterion) */
    qsort(activities, (size_t)n, sizeof(struct Activity), cmp_activity);

    printf("\n  Sorted by end time:\n");
    for (int i = 0; i < n; ++i)
        printf("    [%d, %d]\n", activities[i].start, activities[i].end);

    /* Greedy selection */
    printf("\n  >> Selected activities:\n");

    int last_end = -1;
    int count = 0;

    for (int i = 0; i < n; ++i) {
        if (activities[i].start >= last_end) {
            printf("    << [%d, %d] SELECTED\n",
                   activities[i].start, activities[i].end);
            last_end = activities[i].end;
            count++;
        } else {
            printf("    xx [%d, %d] (overlaps, skipped)\n",
                   activities[i].start, activities[i].end);
        }
    }

    printf("  Total selected: %d out of %d\n", count, n);
    printf("  Lesson: Earliest finish time maximizes remaining room.\n");
    printf("     This is the 'greedy choice property'.\n\n");
}

/* ==================================================================
 *  COIN CHANGE (Greedy)
 * ==================================================================
 *
 *  PROBLEM: Given coin denominations and target amount, find minimum
 *  number of coins.
 *
 *  GREEDY: Always use the LARGEST coin that fits.
 *
 *  WORKS for canonical systems (USD: 25, 10, 5, 1)
 *  FAILS for non-canonical systems: {1, 3, 4}, target=6
 *    Greedy: 4 + 1 + 1 = 3 coins
 *    Optimal: 3 + 3 = 2 coins
 *
 *  REAL-WORLD: Vending machines, cash registers.
 *  Lesson: GREEDY IS NOT ALWAYS CORRECT. Prove it first!
 * ================================================================== */
static void coin_change_demo(void)
{
    int coins[] = {25, 10, 5, 1};
    int n = sizeof(coins) / sizeof(coins[0]);
    int amount = 63;

    printf("  Coins available: {25, 10, 5, 1}\n");
    printf("  Target amount: %d\n\n", amount);

    printf("  >> Greedy approach:\n");

    int remaining = amount;
    int total_coins = 0;

    for (int i = 0; i < n; ++i) {
        if (remaining <= 0) break;

        int num_coins = remaining / coins[i];
        if (num_coins > 0) {
            printf("     %d cent(s) x %d = %d cents  (%d coin%s)\n",
                   coins[i], num_coins, coins[i] * num_coins,
                   num_coins, num_coins > 1 ? "s" : "");
            remaining -= coins[i] * num_coins;
            total_coins += num_coins;
        }
    }

    printf("  Total coins: %d\n", total_coins);
    printf("  Lesson: Greedy works for USD because each coin\n");
    printf("     is at least twice the value of the next smaller.\n");

    /* Counterexample */
    printf("\n  ** Counterexample: coins {1, 3, 4}, amount=6\n");
    printf("     Greedy: 4 + 1 + 1 = 3 coins\n");
    printf("     Optimal: 3 + 3 = 2 coins  (DP needed!)\n");
    printf("     Lesson: Always verify if greedy is correct\n");
    printf("     for YOUR specific problem.\n\n");
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_greedy_module(void)
{
    int choice;

    printf("\n  --- Greedy Algorithms ---\n");
    printf("  1) Activity Selection\n");
    printf("  2) Coin Change (Greedy)\n");
    printf("  3) Both\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 3) {
        printf("  >> ACTIVITY SELECTION\n");
        activity_selection();
    }

    if (choice == 2 || choice == 3) {
        printf("  >> COIN CHANGE\n");
        coin_change_demo();
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
