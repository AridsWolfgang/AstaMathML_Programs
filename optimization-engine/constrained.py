"""Penalty method for constrained optimization.

Solves: minimize f(x) subject to g_i(x) <= 0, h_j(x) = 0.

Uses a quadratic penalty function to convert to unconstrained problem.
"""

import math


def penalty_method(objective, constraints_eq=None, constraints_ineq=None, bounds=None,
                   x0=None, penalty_init=1.0, penalty_mult=10.0, max_outer=50,
                   inner_optimizer=None, verbose=False):
    """Penalty method for constrained optimization.

    Penalty function: P(x, mu) = f(x) + mu/2 * (sum h_j(x)^2 + sum max(0, g_i(x))^2)

    As mu -> inf, the unconstrained minimizer of P approaches the constrained minimizer.

    Args:
        objective: f(x) function.
        constraints_eq: list of equality constraint functions h_j(x) (should be 0).
        constraints_ineq: list of inequality constraint functions g_i(x) (should be <= 0).
        bounds: list of (lo, hi) for each dimension.
        x0: initial point.
        penalty_init: initial penalty parameter.
        penalty_mult: multiplier for penalty in each outer iteration.
        max_outer: maximum outer iterations.
        inner_optimizer: function to minimize penalty function (e.g., SGD).
        verbose: print progress.

    Returns:
        (best_x, best_f, history)
    """
    if constraints_eq is None:
        constraints_eq = []
    if constraints_ineq is None:
        constraints_ineq = []
    if x0 is None:
        if bounds:
            x0 = [(lo + hi) / 2.0 for lo, hi in bounds]
        else:
            x0 = [0.0]
    if inner_optimizer is None:
        inner_optimizer = _simple_gd

    x = list(x0)
    mu = penalty_init
    best_x = list(x)
    best_f = float("inf")
    history = []

    for outer in range(max_outer):
        def penalty_func(xv):
            f = objective(xv)
            pen = 0.0
            for h in constraints_eq:
                pen += 0.5 * h(xv) ** 2
            for g in constraints_ineq:
                val = g(xv)
                if val > 0:
                    pen += 0.5 * val ** 2
            return f + mu * pen

        x = inner_optimizer(penalty_func, x, bounds=bounds, max_iter=200, tol=1e-4)

        current_f = objective(x)
        constraint_violation = sum(abs(h(x)) for h in constraints_eq) + sum(max(0, g(x)) for g in constraints_ineq)

        history.append((list(x), current_f, constraint_violation))
        if current_f < best_f:
            best_f = current_f
            best_x = list(x)

        if verbose:
            print(f"  Outer iter {outer}: f={current_f:.6f}, viol={constraint_violation:.6f}, mu={mu:.0f}")

        if constraint_violation < 1e-6:
            if verbose:
                print("  Constraints satisfied, stopping")
            break

        mu *= penalty_mult

    return best_x, best_f, history


def _simple_gd(func, x0, bounds=None, learning_rate=0.01, max_iter=500, tol=1e-4):
    """Simple finite-difference gradient descent for inner optimization."""
    x = list(x0)
    n = len(x)
    h = 1e-6

    for _ in range(max_iter):
        fx = func(x)
        grad = [0.0] * n
        for i in range(n):
            x_h = list(x)
            x_h[i] += h
            grad[i] = (func(x_h) - fx) / h

        for i in range(n):
            x[i] -= learning_rate * grad[i]
            if bounds:
                x[i] = max(bounds[i][0], min(bounds[i][1], x[i]))

        grad_norm = math.sqrt(sum(g * g for g in grad))
        if grad_norm < tol:
            break

    return x
