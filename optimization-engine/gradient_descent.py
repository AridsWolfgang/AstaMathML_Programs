import math
import random


def sgd(gradient_func, x0, learning_rate=0.01, max_iter=1000, tol=1e-6, verbose=False):
    """Vanilla Stochastic Gradient Descent.

    Uses full gradient (standard gradient descent) since we're doing
    deterministic optimization on known functions.

    x_k+1 = x_k - lr * grad(x_k)
    """
    x = [xi for xi in x0]
    history = [list(x)]
    f_prev = float("inf")

    for iteration in range(max_iter):
        grad = gradient_func(x)
        for i in range(len(x)):
            x[i] -= learning_rate * grad[i]

        history.append(list(x))

        if verbose and iteration % 100 == 0:
            fval = _func_value(gradient_func, x)
            print(f"  iter {iteration:4d}: x={[f'{v:.6f}' for v in x]}, f={fval:.6e}")

        grad_norm = math.sqrt(sum(g * g for g in grad))
        if grad_norm < tol:
            break

    return x, history


def momentum(gradient_func, x0, learning_rate=0.01, beta=0.9, max_iter=1000, tol=1e-6, verbose=False):
    """Gradient descent with momentum.

    v_{k+1} = beta * v_k + lr * grad(x_k)
    x_{k+1} = x_k - v_{k+1}

    Momentum accelerates convergence in directions of consistent gradient.
    """
    x = [xi for xi in x0]
    v = [0.0] * len(x)
    history = [list(x)]

    for iteration in range(max_iter):
        grad = gradient_func(x)
        for i in range(len(x)):
            v[i] = beta * v[i] + learning_rate * grad[i]
            x[i] -= v[i]

        history.append(list(x))

        if verbose and iteration % 100 == 0:
            fval = _func_value(gradient_func, x)
            print(f"  iter {iteration:4d}: x={[f'{v:.6f}' for v in x]}, f={fval:.6e}")

        grad_norm = math.sqrt(sum(g * g for g in grad))
        if grad_norm < tol:
            break

    return x, history


def adam(gradient_func, x0, learning_rate=0.001, beta1=0.9, beta2=0.999, eps=1e-8,
         max_iter=1000, tol=1e-6, verbose=False):
    """Adam optimizer (Adaptive Moment Estimation).

    Combines momentum with per-parameter adaptive learning rates.

    m_{k+1} = beta1 * m_k + (1-beta1) * grad
    v_{k+1} = beta2 * v_k + (1-beta2) * grad^2
    m_hat = m / (1 - beta1^k)
    v_hat = v / (1 - beta2^k)
    x_{k+1} = x_k - lr * m_hat / (sqrt(v_hat) + eps)
    """
    x = [xi for xi in x0]
    m = [0.0] * len(x)
    v = [0.0] * len(x)
    history = [list(x)]

    for iteration in range(1, max_iter + 1):
        grad = gradient_func(x)
        for i in range(len(x)):
            m[i] = beta1 * m[i] + (1 - beta1) * grad[i]
            v[i] = beta2 * v[i] + (1 - beta2) * grad[i] * grad[i]
            m_hat = m[i] / (1 - beta1 ** iteration)
            v_hat = v[i] / (1 - beta2 ** iteration)
            x[i] -= learning_rate * m_hat / (math.sqrt(v_hat) + eps)

        history.append(list(x))

        if verbose and iteration % 100 == 0:
            fval = _func_value(gradient_func, x)
            print(f"  iter {iteration:4d}: x={[f'{v:.6f}' for v in x]}, f={fval:.6e}")

        grad_norm = math.sqrt(sum(g * g for g in grad))
        if grad_norm < tol:
            break

    return x, history


def _func_value(gradient_func, x):
    """Approximate function value from gradient using finite difference.

    We don't have the actual function, so this is a dummy.
    The actual function value is computed externally.
    This is just for display.
    """
    return sum(xi * xi for xi in x)  # placeholder, replaced by caller
