/**
 * ===================================================================
 *  MODULE 2: SEARCHING ALGORITHMS
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - Phone Contacts: Search for a name (Binary Search on sorted list)
 *   - Dictionary: Find a word's definition
 *   - Git Bisect: Binary search through commit history to find a bug
 *   - Autocomplete: Find prefix matches in a sorted list
 *   - Database Index Lookup: B-tree search (binary search variant)
 *   - Finding a book on a shelf: Linear search if unsorted
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - Why sorted data enables much faster searching
 *   - The divide-and-conquer idea behind binary search
 *   - How to recognize when binary search applies
 *   - The importance of a sorted precondition
 *
 * ⏱ COMPLEXITIES:
 *   Linear Search:  O(n) time, O(1) space
 *   Binary Search:  O(log n) time, O(1) space (requires sorted input!)
 * ===================================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>  // for std::sort
#include <cmath>      // for log2

void runSearchingModule();

// ==================================================================
//  LINEAR SEARCH
// ==================================================================
//
//  HOW IT WORKS (visually):
//
//  Search for value 3 in [5, 2, 3, 1, 8]
//
//  Step 1: Check arr[0]=5  → not 3
//  Step 2: Check arr[1]=2  → not 3
//  Step 3: Check arr[2]=3  → FOUND! Return index 2
//
//  REAL-WORLD ANALOGY:
//   Looking for a specific sock in an unsorted drawer —
//   you check each sock one by one until you find the match.
// ==================================================================

int linearSearch(const std::vector<int>& arr, int target, bool verbose)
{
    for (size_t i = 0; i < arr.size(); ++i) {
        if (verbose) {
            std::cout << "  Check arr[" << i << "] = " << arr[i];
        }

        if (arr[i] == target) {
            if (verbose) std::cout << "  ← MATCH!\n";
            return static_cast<int>(i);
        }

        if (verbose) std::cout << "  ← not target\n";
    }

    return -1;  // Not found
}

// ==================================================================
//  BINARY SEARCH
// ==================================================================
//
//  HOW IT WORKS (visually):
//
//  Search for value 7 in sorted array [1, 3, 5, 7, 9, 11]
//
//  Step 1:  low=0, high=5, mid=2  → arr[2]=5 < 7  → search RIGHT half
//           [1, 3, 5, 7, 9, 11]
//                 ↑
//                mid (5 < 7, go right)
//
//  Step 2:  low=3, high=5, mid=4  → arr[4]=9 > 7  → search LEFT half
//           [1, 3, 5, 7, 9, 11]
//                       ↑
//                      mid (9 > 7, go left)
//
//  Step 3:  low=3, high=3, mid=3  → arr[3]=7 == 7 → FOUND!
//           [1, 3, 5, 7, 9, 11]
//                    ↑
//                   mid (7 == 7, found!)
//
//  REAL-WORLD ANALOGY:
//   Looking for a word in a dictionary — open to the middle,
//   if your word comes later, open the right half, and repeat.
//   You NEVER search page by page.
// ==================================================================

int binarySearch(const std::vector<int>& arr, int target, bool verbose)
{
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;  // Avoid overflow: (low+high)/2 can overflow

        if (verbose) {
            std::cout << "  low=" << low << " high=" << high
                      << " mid=" << mid << " → arr[" << mid << "]=" << arr[mid] << "\n";
        }

        if (arr[mid] == target) {
            if (verbose) std::cout << "  ★ FOUND at index " << mid << "!\n";
            return mid;
        }

        if (arr[mid] < target) {
            low = mid + 1;   // Discard left half
            if (verbose) std::cout << "     → " << target << " is larger, search RIGHT half\n";
        } else {
            high = mid - 1;  // Discard right half
            if (verbose) std::cout << "     → " << target << " is smaller, search LEFT half\n";
        }
    }

    return -1;  // Not found
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runSearchingModule()
{
    int choice, target, result;

    std::cout << "\n  --- Searching Algorithms ---\n";
    std::cout << "  1) Linear Search\n";
    std::cout << "  2) Binary Search\n";
    std::cout << "  3) Compare Both\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    std::vector<int> data = {8, 2, 5, 1, 9, 3, 7};
    std::vector<int> sortedData = data;
    std::sort(sortedData.begin(), sortedData.end());

    std::cout << "\n  Unsorted data: ";
    for (int x : data) std::cout << x << " ";
    std::cout << "\n  Sorted data:   ";
    for (int x : sortedData) std::cout << x << " ";

    std::cout << "\n\n  Enter value to search for: ";
    std::cin >> target;

    std::cout << "\n";

    if (choice == 1 || choice == 3) {
        std::cout << "  ▶ LINEAR SEARCH (works on any data)\n";
        result = linearSearch(data, target, true);
        if (result != -1)
            std::cout << "  ✅ Found at index " << result << "\n";
        else
            std::cout << "  ❌ Not found (checked every element)\n";
        std::cout << "  📝 Checks elements one-by-one — O(n).\n\n";
    }

    if (choice == 2 || choice == 3) {
        std::cout << "  ▶ BINARY SEARCH (requires sorted data!)\n";
        result = binarySearch(sortedData, target, true);
        if (result != -1)
            std::cout << "  ✅ Found at index " << result << "\n";
        else
            std::cout << "  ❌ Not found\n";
        std::cout << "  📝 Halves the search space each step — O(log n).\n";
        std::cout << "     Try a large array: binary search finds in ~30 steps\n";
        std::cout << "     what linear search needs 1 billion steps to do!\n\n";
    }

    if (choice == 3) {
        std::cout << "  ⚡ Comparison: For n=" << data.size()
                  << ", Linear checks up to " << data.size()
                  << ", Binary checks up to ~" << (static_cast<int>(log2(data.size())) + 1) << ".\n\n";
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
