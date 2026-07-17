"""Simplex Method for Linear Programming.

Solves: maximize c^T x subject to Ax <= b, x >= 0.

Uses the two-phase simplex method for standard form.
"""

import math


def simplex_maximize(c, A, b, verbose=False):
    """Solve: maximize c^T x, subject to Ax <= b, x >= 0.

    Uses the standard simplex algorithm with slack variables.
    Returns (optimal_value, optimal_solution) or (None, None) if infeasible/unbounded.

    Parameters:
      c -- list of coefficients for objective (length n)
      A -- list of lists, constraint matrix (m x n)
      b -- list of RHS values (length m)
    """
    m = len(A)
    n = len(c)

    # Convert to standard form by adding slack variables
    # Create tableau [A | I | b] with objective row
    tableau = []
    for i in range(m):
        row = list(A[i]) + [0.0] * m + [b[i]]
        tableau.append(row)

    obj_row = [-ci for ci in c] + [0.0] * m + [0.0]
    tableau.append(obj_row)

    cols = n + m
    basis = list(range(n, n + m))

    if verbose:
        print("Initial tableau (before Phase II):")
        _print_tableau(tableau, basis, n, m)

    # Perform simplex iterations
    for iteration in range(1000):
        obj = tableau[-1]
        # Find entering variable (most negative coefficient)
        min_val = 0
        enter = -1
        for j in range(cols):
            if obj[j] < min_val:
                min_val = obj[j]
                enter = j

        if enter == -1:
            break  # Optimal

        # Find leaving variable (minimum ratio test)
        min_ratio = float("inf")
        leave = -1
        for i in range(m):
            if tableau[i][enter] > 1e-10:
                ratio = tableau[i][cols] / tableau[i][enter]
                if ratio < min_ratio:
                    min_ratio = ratio
                    leave = i

        if leave == -1:
            if verbose:
                print("Problem is unbounded")
            return None, None

        # Pivot
        pivot = tableau[leave][enter]
        if verbose:
            print(f"\nIteration {iteration}: enter x{enter}, leave x{basis[leave]}, pivot={pivot:.4f}")

        # Normalize pivot row
        for j in range(cols + 1):
            tableau[leave][j] /= pivot

        # Eliminate in other rows
        for i in range(m + 1):
            if i != leave:
                factor = tableau[i][enter]
                for j in range(cols + 1):
                    tableau[i][j] -= factor * tableau[leave][j]

        basis[leave] = enter

        if verbose:
            _print_tableau(tableau, basis, n, m)

    # Extract solution
    x = [0.0] * n
    for i in range(m):
        if basis[i] < n:
            x[basis[i]] = tableau[i][cols]

    optimal_value = tableau[-1][cols]
    return optimal_value, x


def _print_tableau(tableau, basis, n, m):
    cols = n + m
    header = "  " + "  ".join(f"x{j:>4}" for j in range(cols)) + "     b"
    print(header)
    print("-" * len(header))
    for i in range(m):
        row_str = f"s{basis[i]:>2} " + " ".join(f"{tableau[i][j]:6.2f}" for j in range(cols)) + f"  {tableau[i][cols]:6.2f}"
        print(row_str)
    obj_str = "obj " + " ".join(f"{tableau[-1][j]:6.2f}" for j in range(cols)) + f"  {tableau[-1][cols]:6.2f}"
    print(obj_str)
    print()


def simplex_minimize(c, A, b, verbose=False):
    """Minimize c^T x = maximize -c^T x."""
    c_neg = [-ci for ci in c]
    val, sol = simplex_maximize(c_neg, A, b, verbose)
    if val is None:
        return None, None
    return -val, sol
