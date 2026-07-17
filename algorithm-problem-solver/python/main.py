#!/usr/bin/env python3
"""
=====================================================================
 ALGORITHM & DATA STRUCTURES SOLVER  (Python Edition)
=====================================================================

Why Python? Because Python gives you:
  - Readable code that looks almost like pseudocode
  - Dynamic typing — focus on logic, not type declarations
  - Built-in lists, dicts, and high-level constructs
  - The language of data science, ML, and rapid prototyping

Each algorithm is implemented from scratch (no cheating with
stdlib sorts or containers) to show you how it really works.
=====================================================================
"""

from sorting import run_sorting_module
from searching import run_searching_module
from recursion import run_recursion_module
from dp import run_dp_module
from greedy import run_greedy_module
from graph import run_graph_module
from datastructs import run_datastructs_module


def show_main_menu():
    print()
    print("  ========================================")
    print("     ALGORITHM & DATA STRUCTURES SOLVER   ")
    print("               (Python Edition)           ")
    print("  ========================================")
    print("  Learn by running, reading, and tracing!")
    print("  ----------------------------------------")
    print("  1) Sorting Algorithms")
    print("  2) Searching Algorithms")
    print("  3) Recursion")
    print("  4) Dynamic Programming")
    print("  5) Greedy Algorithms")
    print("  6) Graph Algorithms")
    print("  7) Data Structures")
    print("  Q) Quit")
    print("  ----------------------------------------")
    print("  Choice: ", end="")


def main():
    while True:
        show_main_menu()
        choice = input().strip().lower()

        if choice == "1":
            run_sorting_module()
        elif choice == "2":
            run_searching_module()
        elif choice == "3":
            run_recursion_module()
        elif choice == "4":
            run_dp_module()
        elif choice == "5":
            run_greedy_module()
        elif choice == "6":
            run_graph_module()
        elif choice == "7":
            run_datastructs_module()
        elif choice == "q":
            print("  Goodbye! Keep learning.")
            break
        else:
            print("  Invalid choice. Try again.")


if __name__ == "__main__":
    main()
