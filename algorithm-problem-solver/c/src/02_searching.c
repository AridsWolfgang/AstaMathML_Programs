/*
 * ===================================================================
 *  MODULE 2: SEARCHING ALGORITHMS  (C Edition)
 * ===================================================================
 *
 *  REAL-WORLD USE CASES:
 *    - bsearch() in the C standard library (binary search)
 *    - Phone contact search: binary search on sorted list
 *    - Dictionary lookup: log2(100,000 words) = ~17 comparisons
 *    - Git bisect: binary search through commit history
 *    - Database index lookup (B-trees are binary search tree variants)
 *
 *  KEY INSIGHT:
 *    Sorting data once (O(n log n)) enables searching it
 *    forever in O(log n) time. The upfront cost pays off
 *    after just a few searches.
 *
 *  COMPLEXITIES:
 *    Linear Search:  O(n) time, O(1) space
 *    Binary Search:  O(log n) time, O(1) space (sorted input required!)
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void run_searching_module(void);
extern void clear_input(void);

/* -----------------------------------------------------------------
 * Utility: compare function for qsort (ascending)
 * ----------------------------------------------------------------- */
static int cmp_int(const void *a, const void *b)
{
    return *(const int*)a - *(const int*)b;
}

/* ==================================================================
 *  LINEAR SEARCH
 * ==================================================================
 *
 *  Checks every element one by one. Works on ANY data (sorted or not).
 *  Simple, but slow for large datasets.
 *
 *  REAL-WORLD ANALOGY: Finding a sock in an unsorted drawer.
 * ================================================================== */
static int linear_search(const int *arr, int n, int target, int verbose)
{
    for (int i = 0; i < n; ++i) {
        if (verbose) printf("  Check arr[%d] = %d", i, arr[i]);

        if (arr[i] == target) {
            if (verbose) printf("  << MATCH!\n");
            return i;
        }

        if (verbose) printf("  << not target\n");
    }
    return -1;  /* Not found */
}

/* ==================================================================
 *  BINARY SEARCH
 * ==================================================================
 *
 *  Only works on SORTED arrays. Cuts the search space in half
 *  at every step. For n=1,000,000, binary search takes at most
 *  20 comparisons. Linear search could take 1,000,000.
 *
 *  How it works:
 *    Sorted: [1, 3, 5, 7, 9, 11]   search for 7
 *
 *    Step 1: low=0, high=5, mid=2  -> arr[2]=5 < 7  (go right)
 *    Step 2: low=3, high=5, mid=4  -> arr[4]=9 > 7  (go left)
 *    Step 3: low=3, high=3, mid=3  -> arr[3]=7 == 7 (FOUND!)
 *
 *  REAL-WORLD ANALOGY: Dictionary lookup — open to the middle,
 *  decide left or right, repeat. Never page by page.
 * ================================================================== */
static int binary_search(const int *arr, int n, int target, int verbose)
{
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        /* Calculate mid like this to avoid integer overflow */
        int mid = low + (high - low) / 2;

        if (verbose) {
            printf("  low=%d high=%d mid=%d -> arr[%d]=%d\n",
                   low, high, mid, mid, arr[mid]);
        }

        if (arr[mid] == target) {
            if (verbose) printf("  *** FOUND at index %d!\n", mid);
            return mid;
        }

        if (arr[mid] < target) {
            low = mid + 1;  /* Discard left half */
            if (verbose) printf("     -> %d is larger, search RIGHT half\n", target);
        } else {
            high = mid - 1; /* Discard right half */
            if (verbose) printf("     -> %d is smaller, search LEFT half\n", target);
        }
    }

    return -1;  /* Not found */
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_searching_module(void)
{
    int choice, target, result;

    printf("\n  --- Searching Algorithms ---\n");
    printf("  1) Linear Search\n");
    printf("  2) Binary Search\n");
    printf("  3) Compare Both\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    int data[] = {8, 2, 5, 1, 9, 3, 7};
    int n = sizeof(data) / sizeof(data[0]);

    /* Create sorted copy */
    int *sorted = (int*)malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; ++i) sorted[i] = data[i];
    qsort(sorted, (size_t)n, sizeof(int), cmp_int);

    printf("\n  Unsorted data: ");
    for (int i = 0; i < n; ++i) printf("%d ", data[i]);
    printf("\n  Sorted data:   ");
    for (int i = 0; i < n; ++i) printf("%d ", sorted[i]);

    printf("\n\n  Enter value to search for: ");
    scanf("%d", &target);

    printf("\n");

    if (choice == 1 || choice == 3) {
        printf("  >> LINEAR SEARCH (works on any data)\n");
        result = linear_search(data, n, target, 1);
        if (result != -1)
            printf("  << Found at index %d\n", result);
        else
            printf("  << Not found (checked every element)\n");
        printf("  Cheques elements one-by-one - O(n).\n\n");
    }

    if (choice == 2 || choice == 3) {
        printf("  >> BINARY SEARCH (requires sorted data!)\n");
        result = binary_search(sorted, n, target, 1);
        if (result != -1)
            printf("  << Found at index %d\n", result);
        else
            printf("  << Not found\n");
        printf("  Halves the search space each step - O(log n).\n");
        printf("     For n=%d, max comparisons = ~%d\n\n", n,
               (int)(log2((double)n)) + 1);
    }

    free(sorted);

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
