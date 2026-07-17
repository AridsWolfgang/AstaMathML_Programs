"""
=====================================================================
 MODULE 2: SEARCHING ALGORITHMS  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - Phone contacts: Binary search on sorted contacts
  - Dictionary lookup: log2(100,000) = ~17 comparisons vs 100,000
  - Git bisect: Binary search through commit history for bugs
  - Database index lookup

KEY INSIGHT:
  Sort once (O(n log n)), search forever in O(log n).

COMPLEXITIES:
  Linear Search:  O(n) time, O(1) space
  Binary Search:  O(log n) time, O(1) space (requires sorted!)
=====================================================================
"""

import math


# =================================================================
#  LINEAR SEARCH
# =================================================================
#
#  Search for 3 in [5, 2, 3, 1, 8]:
#    Check arr[0]=5 -> not 3
#    Check arr[1]=2 -> not 3
#    Check arr[2]=3 -> FOUND!
#
#  REAL-WORLD: Finding a sock in an unsorted drawer.
# =================================================================
def linear_search(arr, target, verbose=True):
    for i, val in enumerate(arr):
        if verbose:
            print(f"  Check arr[{i}] = {val}", end="")

        if val == target:
            if verbose:
                print("  << MATCH!")
            return i

        if verbose:
            print("  << not target")

    return -1


# =================================================================
#  BINARY SEARCH
# =================================================================
#
#  Search for 7 in [1, 3, 5, 7, 9, 11]:
#    low=0, high=5, mid=2 -> arr[2]=5 < 7  (go right)
#    low=3, high=5, mid=4 -> arr[4]=9 > 7  (go left)
#    low=3, high=3, mid=3 -> arr[3]=7 == 7 (FOUND!)
#
#  REAL-WORLD: Dictionary — never page by page.
# =================================================================
def binary_search(arr, target, verbose=True):
    low, high = 0, len(arr) - 1

    while low <= high:
        mid = low + (high - low) // 2  # Avoid overflow

        if verbose:
            print(f"  low={low} high={high} mid={mid} -> arr[{mid}]={arr[mid]}")

        if arr[mid] == target:
            if verbose:
                print(f"  *** FOUND at index {mid}!")
            return mid
        elif arr[mid] < target:
            low = mid + 1
            if verbose:
                print(f"     -> {target} is larger, search RIGHT half")
        else:
            high = mid - 1
            if verbose:
                print(f"     -> {target} is smaller, search LEFT half")

    return -1


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_searching_module():
    print("\n  --- Searching Algorithms ---")
    print("  1) Linear Search")
    print("  2) Binary Search")
    print("  3) Compare Both")
    choice = input("  Enter choice: ").strip()

    data = [8, 2, 5, 1, 9, 3, 7]
    sorted_data = sorted(data)  # Python's built-in sort (Timsort)

    print(f"\n  Unsorted data: {data}")
    print(f"  Sorted data:   {sorted_data}")

    target = int(input("\n  Enter value to search for: "))
    print()

    if choice in ("1", "3"):
        print("  >> LINEAR SEARCH (works on any data)")
        result = linear_search(data, target, verbose=True)
        if result != -1:
            print(f"  << Found at index {result}")
        else:
            print("  << Not found (checked every element)")
        print("  Checks elements one-by-one — O(n).\n")

    if choice in ("2", "3"):
        print("  >> BINARY SEARCH (requires sorted data!)")
        result = binary_search(sorted_data, target, verbose=True)
        if result != -1:
            print(f"  << Found at index {result}")
        else:
            print("  << Not found")
        print("  Halves the search space each step — O(log n).")
        max_checks = int(math.log2(len(sorted_data))) + 1
        print(f"     For n={len(sorted_data)}, max checks = ~{max_checks}\n")

    if choice == "3":
        print(f"  Comparison: Linear checks up to {len(data)}, Binary checks up to ~{max_checks}.\n")

    input("  Press Enter to return to menu...")
