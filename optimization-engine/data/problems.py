import math


def rosenbrock(x, y=None, a=1, b=100):
    """Rosenbrock function: f(x) = sum(100*(x[i+1]-x[i]^2)^2 + (1-x[i])^2)

    Global minimum at (1, 1, ..., 1) = 0.
    Known as the 'banana function' — tricky for gradient descent.
    """
    if y is not None:
        return (a - x) ** 2 + b * (y - x * x) ** 2
    return sum(
        b * (x[i + 1] - x[i] ** 2) ** 2 + (a - x[i]) ** 2
        for i in range(len(x) - 1)
    )


def rosenbrock_gradient(x):
    n = len(x)
    grad = [0.0] * n
    a, b = 1, 100
    for i in range(n):
        if i == 0:
            grad[i] = -2 * (a - x[i]) - 4 * b * x[i] * (x[i + 1] - x[i] ** 2)
        elif i == n - 1:
            grad[i] = 2 * b * (x[i] - x[i - 1] ** 2)
        else:
            grad[i] = -2 * (a - x[i]) - 4 * b * x[i] * (x[i + 1] - x[i] ** 2) + 2 * b * (x[i] - x[i - 1] ** 2)
    return grad


def rastrigin(x, y=None, A=10):
    """Rastrigin function: f(x) = 10n + sum(xi^2 - 10*cos(2*pi*xi))

    Many local minima; global minimum at (0, 0, ..., 0) = 0.
    """
    if y is not None:
        return A * 2 + (x * x - A * math.cos(2 * math.pi * x)) + (y * y - A * math.cos(2 * math.pi * y))
    n = len(x)
    return A * n + sum(xi * xi - A * math.cos(2 * math.pi * xi) for xi in x)


def rastrigin_gradient(x, A=10):
    return [2 * xi + 2 * math.pi * A * math.sin(2 * math.pi * xi) for xi in x]


def sphere(x, y=None):
    """Sphere function: f(x) = sum(xi^2). Simple convex, minimum at 0."""
    if y is not None:
        return x * x + y * y
    return sum(xi * xi for xi in x)


def sphere_gradient(x):
    return [2 * xi for xi in x]


def ackley(x, y=None):
    """Ackley function: many local minima, global minimum at 0.

    Highly multimodal.
    """
    if y is not None:
        return _ackley_nd([x, y])
    return _ackley_nd(x)


def _ackley_nd(x):
    n = len(x)
    sum1 = sum(xi * xi for xi in x)
    sum2 = sum(math.cos(2 * math.pi * xi) for xi in x)
    return (
        -20 * math.exp(-0.2 * math.sqrt(sum1 / n))
        - math.exp(sum2 / n)
        + 20
        + math.e
    )


def beale(x, y):
    """Beale function: several local minima, global minimum at (3, 0.5)=0."""
    term1 = (1.5 - x + x * y) ** 2
    term2 = (2.25 - x + x * y * y) ** 2
    term3 = (2.625 - x + x * y * y * y) ** 2
    return term1 + term2 + term3


def himmelblau(x, y):
    """Himmelblau's function: four equal minima.

    f(3,2)=f(-2.805,3.131)=f(-3.779,-3.283)=f(3.584,-1.848)=0
    """
    return (x * x + y - 11) ** 2 + (x + y * y - 7) ** 2


FUNCTIONS_2D = {
    "Rosenbrock": (lambda x, y: rosenbrock(x, y), "f = (a-x)² + b(y-x²)², min at (1,1)=0"),
    "Rastrigin": (rastrigin, "f = 20 + x²-10cos(2πx) + y²-10cos(2πy), min at (0,0)=0"),
    "Sphere": (sphere, "f = x² + y², min at (0,0)=0"),
    "Ackley": (ackley, "Highly multimodal, min at (0,0)=0"),
    "Beale": (beale, "Several valleys, min at (3,0.5)=0"),
    "Himmelblau": (himmelblau, "Four equal minima, f=0"),
}

FUNCTIONS_ND = {
    "Rosenbrock": (rosenbrock, rosenbrock_gradient),
    "Rastrigin": (rastrigin, rastrigin_gradient),
    "Sphere": (sphere, sphere_gradient),
}


def make_problem_2d(name):
    if name in FUNCTIONS_2D:
        return FUNCTIONS_2D[name][0]
    return sphere


def make_problem_nd(name, n=5):
    if name == "Rosenbrock":
        return lambda x: rosenbrock(x, a=1, b=100)
    elif name == "Rastrigin":
        return lambda x: rastrigin(x, A=10)
    elif name == "Sphere":
        return lambda x: sphere(x)
    return lambda x: sphere(x)


def make_gradient_nd(name, n=5):
    if name == "Rosenbrock":
        return rosenbrock_gradient
    elif name == "Rastrigin":
        return rastrigin_gradient
    elif name == "Sphere":
        return sphere_gradient
    return sphere_gradient
