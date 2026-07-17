"""
=====================================================================
 MODULE 3: RECURSION  (Python Edition)
=====================================================================

REAL-WORLD USE CASES:
  - File system: Traversing nested folders
  - Parsing: HTML/XML/JSON (recursive descent parser)
  - AI: Minimax for game-playing (chess, tic-tac-toe)
  - Graphics: Fractals, recursive subdivision

THE THREE LAWS OF RECURSION:
  1. Must have a BASE CASE (stopping condition)
  2. Must call itself with a SMALLER problem
  3. Must make PROGRESS toward the base case

COMPLEXITIES:
  Factorial:         O(n) time, O(n) stack space
  Fibonacci (naive): O(2^n) time — EXPONENTIAL!
  Fibonacci (memo):  O(n) time
  Towers of Hanoi:   O(2^n) time
=====================================================================
"""


# =================================================================
#  FACTORIAL
# =================================================================
#
#  Call stack for fact(4):
#    fact(4) = 4 * fact(3)
#      fact(3) = 3 * fact(2)
#        fact(2) = 2 * fact(1)
#          fact(1) = 1 * fact(0)
#            fact(0) = 1          << BASE CASE!
#          returns 1
#        returns 2
#      returns 6
#    returns 24
#
#  REAL-WORLD: Permutations, combinations, probability.
# =================================================================
def factorial(n):
    if n <= 1:
        return 1          # BASE CASE
    return n * factorial(n - 1)  # RECURSIVE CASE


# =================================================================
#  FIBONACCI
# =================================================================
#
#  F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2)
#  0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
#
#  Naive recursion recomputes the SAME values exponentially:
#    fib(5) calls fib(4) and fib(3)
#      fib(4) calls fib(3) AGAIN and fib(2)
#        fib(3) calls fib(2) AGAIN and fib(1)
#          ...
#
#  REAL-WORLD: Population modeling, Fibonacci heap,
#  stock retracement, natural spirals.
# =================================================================

# Naive — O(2^n). Simple but painfully slow.
_call_counter = 0

def fib_naive(n):
    global _call_counter
    _call_counter += 1
    if n <= 1:
        return n
    return fib_naive(n - 1) + fib_naive(n - 2)


# Memoized — O(n). Cache results to avoid recomputation.
def fib_memo(n, memo=None):
    global _call_counter
    _call_counter += 1

    if memo is None:
        memo = {}

    if n <= 1:
        return n

    if n in memo:
        return memo[n]

    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo)
    return memo[n]


def demo_fibonacci():
    n = int(input("  Enter n to compute Fibonacci(n): "))

    global _call_counter
    _call_counter = 0
    result_naive = fib_naive(n)
    naive_calls = _call_counter

    print(f"  >> NAIVE recursion:")
    print(f"     fib({n}) = {result_naive}")
    print(f"     Function calls: {naive_calls} (O(2^n))")

    _call_counter = 0
    result_memo = fib_memo(n)
    memo_calls = _call_counter

    print(f"  >> MEMOIZED recursion:")
    print(f"     fib({n}) = {result_memo}")
    print(f"     Function calls: {memo_calls} (O(n))")
    print(f"     For n={n}, naive would need ~{2**n:,} calls!")
    print(f"     This is WHY Dynamic Programming exists.\n")


# =================================================================
#  TOWERS OF HANOI
# =================================================================
#
#  Rules:
#    1. Move one disk at a time
#    2. Larger disk never on smaller disk
#    3. Move all from A to C using B
#
#  Total moves for n disks: 2^n - 1
#
#  REAL-WORLD: Backup rotation strategies, AI planning.
# =================================================================
def towers_of_hanoi(n, from_peg, to_peg, aux_peg, move_count, show_moves):
    if n == 1:
        move_count[0] += 1
        if show_moves:
            print(f"     Move disk 1 from {from_peg} to {to_peg}")
        return

    towers_of_hanoi(n - 1, from_peg, aux_peg, to_peg, move_count, show_moves)

    move_count[0] += 1
    if show_moves:
        print(f"     Move disk {n} from {from_peg} to {to_peg}")

    towers_of_hanoi(n - 1, aux_peg, to_peg, from_peg, move_count, show_moves)


def demo_hanoi():
    n = int(input("  Enter number of disks: "))
    move_count = [0]
    show_moves = n <= 5

    if not show_moves:
        print("  (Moves hidden for n > 5 — too many!)")

    towers_of_hanoi(n, 'A', 'C', 'B', move_count, show_moves)
    print(f"  Total moves: {move_count[0]}")
    print(f"  Formula: 2^n - 1 = {2**n - 1} moves")
    print(f"  Lesson: Each additional disk DOUBLES the work.\n")


# =================================================================
#  MODULE RUNNER
# =================================================================
def run_recursion_module():
    print("\n  --- Recursion ---")
    print("  1) Factorial")
    print("  2) Fibonacci (naive vs. memoized)")
    print("  3) Towers of Hanoi")
    print("  4) Run All")
    choice = input("  Enter choice: ").strip()

    print()

    if choice in ("1", "4"):
        n = int(input("  >> FACTORIAL\n  Enter a number: "))
        print(f"  {n}! = {factorial(n)}")
        print("  Lesson: n! = n * (n-1)!\n")

    if choice in ("2", "4"):
        print("  >> FIBONACCI")
        demo_fibonacci()

    if choice in ("3", "4"):
        print("  >> TOWERS OF HANOI")
        demo_hanoi()

    input("  Press Enter to return to menu...")
