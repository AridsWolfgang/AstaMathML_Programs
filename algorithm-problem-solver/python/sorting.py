"""
=====================================================================
 MODULE 1: SORTING ALGORITHMS  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - E-commerce: Sort products by price, rating, or relevance
  - File Explorer: Sort files by name, date, or size
  - Databases: ORDER BY queries use sorting internally
  - Music Apps: Sort playlists by artist, album, or title

In Python, we don't have pointers or manual memory management.
Instead, we have list slicing, tuple assignment (for clean swaps),
and first-class functions. The ALGORITHMS are the same — only
the syntax changes.

COMPLEXITIES:
  Bubble Sort: O(n^2) time, O(1) space
  Quick Sort:  O(n log n) average, O(n^2) worst, O(log n) space
  Merge Sort:  O(n log n) time, O(n) space
=====================================================================
"""


def print_array(arr, label=""):
    """Print array with optional label."""
    prefix = f"  {label}: " if label else "  "
    print(f"{prefix}{arr}")


# =================================================================
#  BUBBLE SORT
# =================================================================
#
#  Pass 1:  [5, 3, 8, 1]
#     compare 5&3 -> swap -> [3, 5, 8, 1]
#     compare 5&8 -> no swap
#     compare 8&1 -> swap -> [3, 5, 1, 8]   8 bubbles to end
#
#  REAL-WORLD: Bubbles in soda — the biggest rise first.
# =================================================================
def bubble_sort(arr, verbose=True):
    n = len(arr)
    for i in range(n - 1):
        swapped = False
        for j in range(n - 1 - i):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]  # Python swap
                swapped = True

        if verbose:
            print(f"  Pass {i + 1}: {arr}")

        if not swapped:
            break  # Early exit if already sorted


# =================================================================
#  QUICK SORT
# =================================================================
#
#  Before: [3, 6, 2, 1, 8, 5]   pivot=5
#  After:  [3, 2, 1, 5, 8, 6]
#              ↑        ↑
#          smaller   pivot  larger
#
#  REAL-WORLD: What the C library qsort() and Python's
#  sorted() use internally.
# =================================================================
def partition(arr, low, high, verbose):
    pivot = arr[high]
    i = low - 1

    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]

    arr[i + 1], arr[high] = arr[high], arr[i + 1]

    if verbose:
        print(f"  Pivot={pivot} -> {arr}")

    return i + 1


def _quick_sort(arr, low, high, verbose):
    if low < high:
        pi = partition(arr, low, high, verbose)
        _quick_sort(arr, low, pi - 1, verbose)
        _quick_sort(arr, pi + 1, high, verbose)


def quick_sort(arr, verbose=True):
    _quick_sort(arr, 0, len(arr) - 1, verbose)


# =================================================================
#  MERGE SORT
# =================================================================
#
#  [5, 3, 8, 1, 2, 7]
#         /       \
#   [5, 3, 8]   [1, 2, 7]        <- split
#      /  \        /  \
#   [5] [3,8]    [1] [2,7]       <- split further
#       /  \        /  \
#     [3] [8]     [2] [7]        <- base case
#       \  /        \  /
#     [3, 8]      [2, 7]         <- merge (sorted!)
#         \         /
#   [3, 5, 8]   [1, 2, 7]       <- merge
#            \   /
#     [1, 2, 3, 5, 7, 8]        <- done!
#
#  REAL-WORLD: External sorting (data too large for RAM)
# =================================================================
def merge(arr, left, mid, right, verbose):
    """Merge two sorted subarrays arr[left..mid] and arr[mid+1..right]."""
    L = arr[left:mid + 1]   # Python slice: creates a copy
    R = arr[mid + 1:right + 1]

    i = j = 0
    k = left

    while i < len(L) and j < len(R):
        if L[i] <= R[j]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1

    while i < len(L):
        arr[k] = L[i]
        i += 1
        k += 1

    while j < len(R):
        arr[k] = R[j]
        j += 1
        k += 1

    if verbose:
        print(f"  Merge [{left}-{right}]: {arr[left:right + 1]}")


def _merge_sort(arr, left, right, verbose):
    if left < right:
        mid = (left + right) // 2
        _merge_sort(arr, left, mid, verbose)
        _merge_sort(arr, mid + 1, right, verbose)
        merge(arr, left, mid, right, verbose)


def merge_sort(arr, verbose=True):
    _merge_sort(arr, 0, len(arr) - 1, verbose)


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_sorting_module():
    print("\n  --- Sorting Algorithms ---")
    print("  1) Bubble Sort")
    print("  2) Quick Sort")
    print("  3) Merge Sort")
    print("  4) All (compare them)")
    choice = input("  Enter choice: ").strip()

    data = [5, 3, 8, 1, 2, 7]
    print(f"\n  Original array: {data}\n")

    if choice in ("1", "4"):
        arr = data.copy()
        print("  >> BUBBLE SORT")
        bubble_sort(arr, verbose=True)
        print(f"  Final: {arr}")
        print("  Key insight: Each pass bubbles the largest")
        print("     remaining element to its correct position.\n")

    if choice in ("2", "4"):
        arr = data.copy()
        print("  >> QUICK SORT (pivot = last element)")
        quick_sort(arr, verbose=True)
        print(f"  Final: {arr}")
        print("  Key insight: Partition around a pivot,")
        print("     then recursively sort each side.\n")

    if choice in ("3", "4"):
        arr = data.copy()
        print("  >> MERGE SORT")
        merge_sort(arr, verbose=True)
        print(f"  Final: {arr}")
        print("  Key insight: Split, sort recursively,")
        print("     then merge sorted halves together.\n")

    input("  Press Enter to return to menu...")
