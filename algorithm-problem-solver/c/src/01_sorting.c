/*
 * ===================================================================
 *  MODULE 1: SORTING ALGORITHMS  (C Edition)
 * ===================================================================
 *
 *  In C, unlike C++, there is no std::swap, no templates, and no
 *  std::vector. We write everything by hand — which means you see
 *  every pointer, every allocation, and every swap explicitly.
 *
 *  This is NOT a disadvantage. It is the POINT.
 *  When you write C, you are in control of every byte.
 *
 *  REAL-WORLD USE CASES:
 *    - qsort() in the C standard library uses Quick Sort internally
 *    - Database engines implement custom sort routines in C
 *    - Embedded systems sort sensor data without any STL
 *    - Operating system kernels sort process queues
 *    - Every sorting algorithm you implement here is how
 *      the inside of qsort() actually works
 *
 *  COMPLEXITIES:
 *    Bubble Sort: O(n^2) time, O(1) space
 *    Quick Sort:  O(n log n) average, O(n^2) worst, O(log n) space
 *    Merge Sort:  O(n log n) time, O(n) space
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void run_sorting_module(void);
extern void clear_input(void);

/* -----------------------------------------------------------------
 * Utility: print an integer array
 * ----------------------------------------------------------------- */
static void print_array(const int *arr, int n, const char *label)
{
    if (label) printf("  %s: ", label);
    else       printf("  ");
    putchar('[');
    for (int i = 0; i < n; ++i) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

/* -----------------------------------------------------------------
 * Utility: swap two integers (in C, we write this ourselves)
 * ----------------------------------------------------------------- */
static void swap_int(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ==================================================================
 *  BUBBLE SORT
 * ==================================================================
 *
 *  How it works (visually):
 *
 *  Pass 1:  [5, 3, 8, 1]
 *     compare 5&3 → swap → [3, 5, 8, 1]
 *     compare 5&8 → no swap
 *     compare 8&1 → swap → [3, 5, 1, 8]   ← 8 bubbles to end
 *
 *  Pass 2:  [3, 5, 1, 8]
 *     compare 3&5 → no swap
 *     compare 5&1 → swap → [3, 1, 5, 8]
 *
 *  REAL-WORLD ANALOGY: Bubbles in soda — the biggest rise first.
 * ================================================================== */
static void bubble_sort(int *arr, int n, int verbose)
{
    for (int i = 0; i < n - 1; ++i) {
        int swapped = 0;

        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap_int(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }

        if (verbose) {
            printf("  Pass %d: ", i + 1);
            print_array(arr, n, NULL);
        }

        /* Early exit: if no swaps, array is already sorted */
        if (!swapped) break;
    }
}

/* ==================================================================
 *  QUICK SORT
 * ==================================================================
 *
 *  How it works:
 *    1. Pick a pivot (here: last element)
 *    2. Partition: elements < pivot go left, > pivot go right
 *    3. Recursively sort left and right halves
 *
 *  Before: [3, 6, 2, 1, 8, 5]   pivot=5
 *  After:  [3, 2, 1, 5, 8, 6]
 *              ↑        ↑
 *          smaller   pivot  larger
 *
 *  REAL-WORLD: What the C library function qsort() uses.
 * ================================================================== */
static int partition(int *arr, int low, int high, int verbose)
{
    int pivot = arr[high];  /* Choose last element as pivot */
    int i = low - 1;        /* Index of smaller element */

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            swap_int(&arr[i], &arr[j]);
        }
    }

    swap_int(&arr[i + 1], &arr[high]);

    if (verbose) {
        printf("  Pivot=%d -> ", pivot);
        print_array(arr, high + 1, NULL);
    }

    return i + 1;  /* Return pivot's final position */
}

static void quick_sort_recursive(int *arr, int low, int high, int verbose)
{
    if (low < high) {
        int pi = partition(arr, low, high, verbose);
        quick_sort_recursive(arr, low, pi - 1, verbose);
        quick_sort_recursive(arr, pi + 1, high, verbose);
    }
}

static void quick_sort(int *arr, int n, int verbose)
{
    quick_sort_recursive(arr, 0, n - 1, verbose);
}

/* ==================================================================
 *  MERGE SORT
 * ==================================================================
 *
 *  How it works:
 *    1. Split array in half
 *    2. Recursively sort each half
 *    3. Merge the two sorted halves
 *
 *  [5, 3, 8, 1, 2, 7]
 *         /       \
 *   [5, 3, 8]   [1, 2, 7]       ← split
 *      /  \        /  \
 *   [5] [3,8]    [1] [2,7]      ← split further
 *       /  \        /  \
 *     [3] [8]     [2] [7]       ← base case
 *       \  /        \  /
 *     [3, 8]      [2, 7]        ← merge (sorted!)
 *         \         /
 *   [3, 5, 8]   [1, 2, 7]      ← merge
 *            \   /
 *     [1, 2, 3, 5, 7, 8]       ← done!
 *
 *  REAL-WORLD: External sorting (sorting data too large for RAM)
 * ================================================================== */
static void merge(int *arr, int left, int mid, int right, int verbose)
{
    int n1 = mid - left + 1;   /* Size of left half */
    int n2 = right - mid;      /* Size of right half */

    /* In C, we must manually allocate temporary storage */
    int *L = (int*)malloc((size_t)n1 * sizeof(int));
    int *R = (int*)malloc((size_t)n2 * sizeof(int));

    if (!L || !R) {
        printf("    Memory allocation failed!\n");
        free(L); free(R);
        return;
    }

    /* Copy data to temp arrays */
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    /* Merge: pick the smaller element from each half */
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else              arr[k++] = R[j++];
    }

    /* Copy remaining elements (if any) */
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    /* In C, we FREE what we MALLOC — no garbage collector! */
    free(L);
    free(R);

    if (verbose) {
        printf("  Merge [%d-%d]: ", left, right);
        for (int idx = left; idx <= right; ++idx)
            printf("%d ", arr[idx]);
        printf("\n");
    }
}

static void merge_sort_recursive(int *arr, int left, int right, int verbose)
{
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_recursive(arr, left, mid, verbose);
        merge_sort_recursive(arr, mid + 1, right, verbose);
        merge(arr, left, mid, right, verbose);
    }
}

static void merge_sort(int *arr, int n, int verbose)
{
    merge_sort_recursive(arr, 0, n - 1, verbose);
}

/* ==================================================================
 *  MODULE MENU & RUNNER
 * ================================================================== */
void run_sorting_module(void)
{
    int choice;
    int data[] = {5, 3, 8, 1, 2, 7};
    int n = sizeof(data) / sizeof(data[0]);
    int verbose = 1;

    printf("\n  --- Sorting Algorithms ---\n");
    printf("  1) Bubble Sort\n");
    printf("  2) Quick Sort\n");
    printf("  3) Merge Sort\n");
    printf("  4) All (compare them)\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n  Original array: ");
    print_array(data, n, NULL);
    printf("\n");

    if (choice == 1 || choice == 4) {
        /* Make a copy because sorting modifies the array */
        int *arr = (int*)malloc((size_t)n * sizeof(int));
        for (int i = 0; i < n; ++i) arr[i] = data[i];

        printf("  >> BUBBLE SORT\n");
        bubble_sort(arr, n, verbose);
        printf("  Final: ");
        print_array(arr, n, NULL);
        printf("  Key insight: Each pass bubbles the largest\n");
        printf("     remaining element to its correct position.\n\n");

        free(arr);
    }

    if (choice == 2 || choice == 4) {
        int *arr = (int*)malloc((size_t)n * sizeof(int));
        for (int i = 0; i < n; ++i) arr[i] = data[i];

        printf("  >> QUICK SORT (pivot = last element)\n");
        quick_sort(arr, n, verbose);
        printf("  Final: ");
        print_array(arr, n, NULL);
        printf("  Key insight: Partition around a pivot,\n");
        printf("     then recursively sort each side.\n\n");

        free(arr);
    }

    if (choice == 3 || choice == 4) {
        int *arr = (int*)malloc((size_t)n * sizeof(int));
        for (int i = 0; i < n; ++i) arr[i] = data[i];

        printf("  >> MERGE SORT\n");
        merge_sort(arr, n, verbose);
        printf("  Final: ");
        print_array(arr, n, NULL);
        printf("  Key insight: Split, sort recursively,\n");
        printf("     then merge sorted halves together.\n\n");

        free(arr);
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
