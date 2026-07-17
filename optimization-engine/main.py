import math
import os
import sys
import random

from gradient_descent import sgd, momentum, adam
from linear_programming import simplex_maximize, simplex_minimize
from evolutionary import genetic_algorithm
from constrained import penalty_method
from data.problems import (
    rosenbrock, rastrigin, sphere, ackley, beale, himmelblau,
    rosenbrock_gradient, rastrigin_gradient, sphere_gradient,
    make_problem_nd, make_gradient_nd,
    FUNCTIONS_2D, FUNCTIONS_ND,
)


def clear():
    os.system("cls" if os.name == "nt" else "clear")


def pause():
    input("\nPress Enter to continue...")


def ascii_convergence(history, width=60, title="Convergence"):
    """Plot convergence history as ASCII chart."""
    if not history:
        return "[No history]"
    mn = min(history)
    mx = max(history)
    if mx == mn:
        return f"[All values = {mn}]"
    lines = [f"--- {title} ---"]
    n_points = len(history)
    n_bars = min(width, n_points)
    step = max(1, n_points // n_bars)
    sampled = [history[min(i * step, n_points - 1)] for i in range(n_bars)]
    min_s = min(sampled)
    max_s = max(sampled)
    for i, v in enumerate(sampled):
        bar_len = max(1, int((v - min_s) / (max_s - min_s + 1e-10) * 40))
        bar = "#" * bar_len
        lines.append(f"  {v:10.4f} |{bar}")
    return "\n".join(lines)


def menu_gd():
    clear()
    print("=" * 60)
    print("          GRADIENT DESCENT OPTIMIZER")
    print("=" * 60)
    print("Select function:")
    functions = ["Sphere", "Rosenbrock", "Rastrigin"]
    for i, name in enumerate(functions):
        print(f"  {i+1}. {name}")
    choice = input("Choice: ").strip()

    if choice == "1":
        fname, n_dim = "Sphere", 5
        grad_func = make_gradient_nd("Sphere", n_dim)
        true_min = 0
    elif choice == "2":
        fname, n_dim = "Rosenbrock", 5
        grad_func = make_gradient_nd("Rosenbrock", n_dim)
        true_min = 0
    elif choice == "3":
        fname, n_dim = "Rastrigin", 5
        grad_func = make_gradient_nd("Rastrigin", n_dim)
        true_min = 0
    else:
        return

    x0_str = input(f"Initial point (default 2.0 for all dims): ") or "2.0"
    try:
        x0 = [float(x0_str)] * n_dim
    except ValueError:
        x0 = [2.0] * n_dim

    lr = float(input("Learning rate (default 0.01): ") or 0.01)
    iters = int(input("Max iterations (default 500): ") or 500)

    func = make_problem_nd(fname, n_dim)

    def wrapped_grad(x):
        return grad_func(x)

    def func_val(x):
        return func(x)

    print(f"\nOptimizing {fname} (n={n_dim}) from x0={x0}")
    print(f"True minimum: {true_min}")
    print()

    print("--- SGD ---")
    x_sgd, hist_sgd = sgd(wrapped_grad, x0, lr, iters, verbose=False)
    f_sgd = func_val(x_sgd)
    print(f"  Final x: {[f'{v:.4f}' for v in x_sgd]}")
    print(f"  f(x): {f_sgd:.6e}")

    print("\n--- Momentum ---")
    beta = float(input("Momentum beta (default 0.9): ") or 0.9)
    x_mom, hist_mom = momentum(wrapped_grad, x0, lr, beta, iters, verbose=False)
    f_mom = func_val(x_mom)
    print(f"  Final x: {[f'{v:.4f}' for v in x_mom]}")
    print(f"  f(x): {f_mom:.6e}")

    print("\n--- Adam ---")
    x_adam, hist_adam = adam(wrapped_grad, x0, min(lr * 0.1, 0.001), verbose=False)
    f_adam = func_val(x_adam)
    print(f"  Final x: {[f'{v:.4f}' for v in x_adam]}")
    print(f"  f(x): {f_adam:.6e}")

    print()
    sgd_hist = [func_val(x) for x in hist_sgd]
    mom_hist = [func_val(x) for x in hist_mom]
    adam_hist = [func_val(x) for x in hist_adam]

    print(ascii_convergence(sgd_hist[:100], title="SGD Convergence (first 100)"))
    print(ascii_convergence(mom_hist[:100], title="Momentum Convergence (first 100)"))
    print(ascii_convergence(adam_hist[:100], title="Adam Convergence (first 100)"))

    pause()


def menu_simplex():
    clear()
    print("=" * 60)
    print("           SIMPLEX METHOD FOR LP")
    print("=" * 60)
    print("Solve: maximize/minimize c^T x subject to Ax <= b, x >= 0")
    print()
    print("Example problem:")
    print("  Maximize: 3x + 2y")
    print("  Subject to: x + y <= 4")
    print("              2x + y <= 5")
    print("              x, y >= 0")
    print("  Optimal: x=1, y=3, value=9")
    print()

    n = int(input("Number of variables (default 2): ") or 2)
    m = int(input("Number of constraints (default 2): ") or 2)

    print("\nEnter objective coefficients (c):")
    c = []
    for i in range(n):
        val = float(input(f"  c[{i}] (default {3 if i==0 else 2 if i==1 else 0}): ") or (3 if i==0 else 2 if i==1 else 0))
        c.append(val)

    print("\nEnter constraint matrix (A) and RHS (b):")
    A = []
    b = []
    for i in range(m):
        row = []
        for j in range(n):
            val = float(input(f"  A[{i}][{j}] (default {1 if i==j else 1 if i==0 and j==1 else 0}): ")
                       or (1 if (i==0 and j==0) or (i==0 and j==1) else
                           2 if i==1 and j==0 else
                           1 if i==1 and j==1 else 0))
            row.append(val)
        A.append(row)
        val = float(input(f"  b[{i}] (default {4 if i==0 else 5}): ") or (4 if i==0 else 5))
        b.append(val)

    mode = input("Maximize or minimize (max/min, default max): ").strip().lower()
    verbose = input("Show tableau iterations? (y/n, default n): ").strip().lower() == "y"

    print("\nSolving...")
    if mode.startswith("min"):
        val, sol = simplex_minimize(c, A, b, verbose)
    else:
        val, sol = simplex_maximize(c, A, b, verbose)

    if val is None:
        print("Problem is infeasible or unbounded")
    else:
        print(f"\nOptimal value: {val:.4f}")
        print(f"Optimal solution: {sol}")

    pause()


def menu_ga():
    clear()
    print("=" * 60)
    print("             GENETIC ALGORITHM")
    print("=" * 60)
    print("Select function to optimize:")
    funcs = list(FUNCTIONS_2D.keys())
    for i, name in enumerate(funcs):
        desc = FUNCTIONS_2D[name][1]
        print(f"  {i+1}. {name} — {desc}")
    choice = input("Choice: ").strip()

    if not choice.isdigit() or int(choice) < 1 or int(choice) > len(funcs):
        return
    fname = funcs[int(choice) - 1]
    func = FUNCTIONS_2D[fname][0]

    bounds = []
    if fname in ("Rosenbrock", "Sphere"):
        bounds = [(-5, 5), (-5, 5)]
    elif fname == "Rastrigin":
        bounds = [(-5.12, 5.12), (-5.12, 5.12)]
    elif fname == "Ackley":
        bounds = [(-5, 5), (-5, 5)]
    elif fname in ("Beale",):
        bounds = [(-4.5, 4.5), (-4.5, 4.5)]
    elif fname == "Himmelblau":
        bounds = [(-6, 6), (-6, 6)]

    pop = int(input("Population size (default 100): ") or 100)
    gen = int(input("Generations (default 200): ") or 200)
    mr = float(input("Mutation rate (default 0.1): ") or 0.1)

    print(f"\nRunning GA on {fname}...")
    best_x, best_f, history = genetic_algorithm(
        lambda x: func(x[0], x[1]),
        bounds,
        pop_size=pop,
        generations=gen,
        mutation_rate=mr,
        verbose=True,
    )

    print(f"\nBest solution: x={best_x[0]:.6f}, y={best_x[1]:.6f}")
    print(f"Best fitness:  {best_f:.6e}")
    print()
    print(ascii_convergence(history, title="GA Convergence"))
    pause()


def menu_compare():
    clear()
    print("=" * 60)
    print("       COMPARE ALL METHODS ON BENCHMARKS")
    print("=" * 60)

    benchmark_funcs = [
        ("Sphere", sphere, [2.0, 2.0], True, [(-5, 5), (-5, 5)]),
        ("Rosenbrock", lambda x, y: rosenbrock(x, y), [2.0, 2.0], True, [(-5, 5), (-5, 5)]),
        ("Rastrigin", rastrigin, [2.0, 2.0], True, [(-5.12, 5.12), (-5.12, 5.12)]),
    ]

    for fname, func, x0, minimize, bounds in benchmark_funcs:
        print(f"\n{'='*50}")
        print(f"  BENCHMARK: {fname}")
        print(f"{'='*50}")

        # GA
        print(f"\n  --- Genetic Algorithm ---")
        best_x, best_f, hist = genetic_algorithm(
            lambda x: func(x[0], x[1]),
            bounds,
            pop_size=60,
            generations=100,
            mutation_rate=0.1,
            verbose=False,
        )
        print(f"    Best: f({best_x[0]:.4f}, {best_x[1]:.4f}) = {best_f:.6e}")

        # SGD
        print(f"  --- SGD ---")
        grad_funcs = {
            "Sphere": sphere_gradient,
            "Rosenbrock": rosenbrock_gradient,
            "Rastrigin": rastrigin_gradient,
        }
        gf = grad_funcs.get(fname, sphere_gradient)
        x_sgd, _ = sgd(gf, x0, 0.01, 500, verbose=False)
        f_sgd = func(x_sgd[0], x_sgd[1])
        print(f"    Best: f({x_sgd[0]:.4f}, {x_sgd[1]:.4f}) = {f_sgd:.6e}")

        # Adam
        print(f"  --- Adam ---")
        x_adam, _ = adam(gf, x0, 0.001, verbose=False)
        f_adam = func(x_adam[0], x_adam[1])
        print(f"    Best: f({x_adam[0]:.4f}, {x_adam[1]:.4f}) = {f_adam:.6e}")

        print()

    pause()


def main():
    while True:
        clear()
        print("=" * 60)
        print("              OPTIMIZATION ENGINE")
        print("=" * 60)
        print("1. Gradient Descent Optimizer (SGD / Momentum / Adam)")
        print("2. Simplex Method for Linear Programming")
        print("3. Genetic Algorithm")
        print("4. Compare All Methods on Benchmark Functions")
        print("5. Exit")
        choice = input("\nSelect option: ").strip()

        if choice == "1":
            menu_gd()
        elif choice == "2":
            menu_simplex()
        elif choice == "3":
            menu_ga()
        elif choice == "4":
            menu_compare()
        elif choice == "5":
            print("\nGoodbye!")
            break


if __name__ == "__main__":
    main()
