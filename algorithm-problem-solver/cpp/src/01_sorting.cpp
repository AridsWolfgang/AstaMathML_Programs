/**
 * ===================================================================
 *  MODULE 1: SORTING ALGORITHMS
 * ===================================================================
 *
 * 📌 REAL-WORLD USE CASES:
 *   - E-commerce: Sort products by price, rating, or relevance
 *   - File Explorer: Sort files by name, date, or size
 *   - Databases: ORDER BY queries use sorting internally
 *   - Search Results: Ranking and ordering results
 *   - Music Apps: Sort playlists by artist, album, or title
 *
 * 🧠 WHAT YOU'LL LEARN:
 *   - How comparison-based sorting works under the hood
 *   - The trade-off between simple (Bubble) and efficient (Quick/Merge)
 *   - Divide-and-conquer strategy (Quick Sort, Merge Sort)
 *   - Why algorithms have different time/space complexities
 *   - How to trace through algorithm execution step by step
 *
 * ⏱ COMPLEXITIES:
 *   Bubble Sort: O(n^2) time, O(1) space
 *   Quick Sort:  O(n log n) average, O(n^2) worst, O(log n) space
 *   Merge Sort:  O(n log n) time, O(n) space
 * ===================================================================
 */

#include <iostream>
#include <vector>
#include <utility>   // for std::swap

// -----------------------------------------------------------------
// Forward declarations
// -----------------------------------------------------------------
void runSortingModule();

// -----------------------------------------------------------------
// Display helpers
// -----------------------------------------------------------------
void printArray(const std::vector<int>& arr, const std::string& label = "")
{
    if (!label.empty()) std::cout << "  " << label << ": ";
    else                std::cout << "  ";
    std::cout << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

// ==================================================================
//  BUBBLE SORT
// ==================================================================
//
//  HOW IT WORKS (visually):
//
//  Pass 1:  [5, 3, 8, 1]  → compare 5&3 → swap → [3, 5, 8, 1]
//                                   → compare 5&8 → no swap
//                                   → compare 8&1 → swap → [3, 5, 1, 8]
//           The largest element (8) "bubbles up" to its correct position.
//
//  Pass 2:  [3, 5, 1, 8]  → compare 3&5 → no swap
//                                   → compare 5&1 → swap → [3, 1, 5, 8]
//           The second largest (5) is now in place.
//
//  Pass 3:  [3, 1, 5, 8]  → compare 3&1 → swap → [1, 3, 5, 8]
//           Done! Array is sorted.
//
//  REAL-WORLD ANALOGY:
//   Bubbles in a glass of soda — the biggest bubbles rise first.
//   Similarly, the largest numbers bubble up to the end first.
// ==================================================================

void bubbleSort(std::vector<int>& arr, bool verbose)
{
    int n = static_cast<int>(arr.size());

    // Outer loop: each pass places the next largest element
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;  // Optimization: track if any swap happened

        // Inner loop: compare adjacent elements
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        if (verbose) {
            std::cout << "  Pass " << (i + 1) << ": ";
            printArray(arr);
        }

        // If no swaps happened, array is already sorted — EARLY EXIT!
        // This is why Bubble Sort performs well on nearly-sorted data.
        if (!swapped) break;
    }
}

// ==================================================================
//  QUICK SORT
// ==================================================================
//
//  HOW IT WORKS (visually):
//
//  Choose a "pivot" (here: last element).
//  Partition: arrange so that elements < pivot come first,
//  then pivot, then elements > pivot.
//
//  BEFORE: [3, 6, 2, 1, 8, 5]   pivot = 5
//                   ↓
//  AFTER:  [3, 2, 1, 5, 8, 6]
//               ↑     ↑
//           smaller pivot  larger
//
//  Then recursively sort [3,2,1] and [8,6].
//
//  REAL-WORLD ANALOGY:
//   Organizing a deck of cards: pick one card, put smaller cards
//   on the left, larger on the right, then repeat for each pile.
// ==================================================================

int partition(std::vector<int>& arr, int low, int high, bool verbose)
{
    int pivot = arr[high];  // Choose last element as pivot
    int i = low - 1;        // Index of the "smaller element" region

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }

    // Place pivot in its correct position
    std::swap(arr[i + 1], arr[high]);

    if (verbose) {
        std::cout << "  Pivot=" << pivot << " → ";
        printArray(arr);
    }

    return i + 1;  // Return pivot's final index
}

void quickSortRecursive(std::vector<int>& arr, int low, int high, bool verbose)
{
    if (low < high) {
        // Partition the array and get the pivot index
        int pi = partition(arr, low, high, verbose);

        // Recursively sort elements before and after pivot
        quickSortRecursive(arr, low, pi - 1, verbose);
        quickSortRecursive(arr, pi + 1, high, verbose);
    }
}

void quickSort(std::vector<int>& arr, bool verbose)
{
    quickSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1, verbose);
}

// ==================================================================
//  MERGE SORT
// ==================================================================
//
//  HOW IT WORKS (visually):
//
//  [5, 3, 8, 1, 2, 7]
//         ↙       ↘
//   [5, 3, 8]   [1, 2, 7]         ← split
//     ↙    ↘      ↙    ↘
//  [5] [3, 8]  [1] [2, 7]         ← split further
//      ↙  ↘       ↙  ↘
//    [3] [8]     [2] [7]          ← base case (single element)
//      ↘  ↙       ↘  ↙
//    [3, 8]      [2, 7]           ← merge while sorting
//      ↘          ↙
//   [3, 5, 8]   [1, 2, 7]        ← merge
//         ↘      ↙
//     [1, 2, 3, 5, 7, 8]         ← sorted!
//
//  REAL-WORLD ANALOGY:
//   Merging two sorted piles of papers into one sorted pile —
//   you compare the top of each pile and pick the smaller one.
// ==================================================================

void merge(std::vector<int>& arr, int left, int mid, int right, bool verbose)
{
    // Create temporary arrays for the two halves
    std::vector<int> L(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<int> R(arr.begin() + mid + 1, arr.begin() + right + 1);

    size_t i = 0, j = 0;
    int k = left;

    // Merge: repeatedly pick the smaller element from the two halves
    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    // Copy any remaining elements from left half
    while (i < L.size()) arr[k++] = L[i++];

    // Copy any remaining elements from right half
    while (j < R.size()) arr[k++] = R[j++];

    if (verbose) {
        std::cout << "  Merge [" << left << "-" << right << "]: ";
        for (int idx = left; idx <= right; ++idx)
            std::cout << arr[idx] << " ";
        std::cout << "\n";
    }
}

void mergeSortRecursive(std::vector<int>& arr, int left, int right, bool verbose)
{
    if (left < right) {
        int mid = left + (right - left) / 2;  // Avoid overflow

        // Recursively split into halves
        mergeSortRecursive(arr, left, mid, verbose);
        mergeSortRecursive(arr, mid + 1, right, verbose);

        // Merge the sorted halves
        merge(arr, left, mid, right, verbose);
    }
}

void mergeSort(std::vector<int>& arr, bool verbose)
{
    mergeSortRecursive(arr, 0, static_cast<int>(arr.size()) - 1, verbose);
}

// ==================================================================
//  MODULE MENU & RUNNER
// ==================================================================

void runSortingModule()
{
    int choice;
    bool verbose = true;

    std::cout << "\n  --- Sorting Algorithms ---\n";
    std::cout << "  1) Bubble Sort\n";
    std::cout << "  2) Quick Sort\n";
    std::cout << "  3) Merge Sort\n";
    std::cout << "  4) All (compare them)\n";
    std::cout << "  Enter choice: ";
    std::cin >> choice;

    // Sample data — try changing these numbers!
    std::vector<int> data = {5, 3, 8, 1, 2, 7};
    std::cout << "\n  Original array: ";
    printArray(data);
    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        std::vector<int> arr = data;
        std::cout << "  ▶ BUBBLE SORT\n";
        bubbleSort(arr, verbose);
        std::cout << "  Final: ";
        printArray(arr);
        std::cout << "  📝 Key insight: Each pass bubbles the largest\n";
        std::cout << "     remaining element to its correct position.\n\n";
    }

    if (choice == 2 || choice == 4) {
        std::vector<int> arr = data;
        std::cout << "  ▶ QUICK SORT (pivot = last element)\n";
        quickSort(arr, verbose);
        std::cout << "  Final: ";
        printArray(arr);
        std::cout << "  📝 Key insight: Partition around a pivot,\n";
        std::cout << "     then recursively sort each side.\n\n";
    }

    if (choice == 3 || choice == 4) {
        std::vector<int> arr = data;
        std::cout << "  ▶ MERGE SORT\n";
        mergeSort(arr, verbose);
        std::cout << "  Final: ";
        printArray(arr);
        std::cout << "  📝 Key insight: Split, sort recursively,\n";
        std::cout << "     then merge sorted halves together.\n\n";
    }

    std::cout << "  Press Enter to return to menu...";
    std::cin.ignore(10000, '\n').get();
}
