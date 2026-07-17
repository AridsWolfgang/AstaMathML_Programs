import math
import random


def linear_data(n=100, noise=2.0, seed=42):
    random.seed(seed)
    xs = [random.uniform(-10, 10) for _ in range(n)]
    ys = [3.5 * x + 7.2 + random.gauss(0, noise) for x in xs]
    return [[x] for x in xs], ys


def polynomial_data(n=100, degree=3, noise=5.0, seed=42):
    random.seed(seed)
    xs = [random.uniform(-5, 5) for _ in range(n)]
    ys = []
    for x in xs:
        y = 2 * x**3 - 3 * x**2 + 4 * x + 1 + random.gauss(0, noise)
        ys.append(y)
    return [[x] for x in xs], ys


def multi_linear_data(n=100, noise=1.0, seed=42):
    random.seed(seed)
    xs = [[random.uniform(-3, 3) for _ in range(3)] for _ in range(n)]
    ys = [2.0 * x[0] - 1.5 * x[1] + 0.5 * x[2] + 5.0 + random.gauss(0, noise) for x in xs]
    return xs, ys


def classification_data(n=100, seed=42):
    random.seed(seed)
    xs, ys = [], []
    for _ in range(n // 2):
        xs.append([random.gauss(-1.5, 1.0), random.gauss(-1.5, 1.0)])
        ys.append(0)
    for _ in range(n // 2):
        xs.append([random.gauss(1.5, 1.0), random.gauss(1.5, 1.0)])
        ys.append(1)
    combined = list(zip(xs, ys))
    random.shuffle(combined)
    xs, ys = zip(*combined)
    return list(xs), list(ys)


def moons_data(n=100, noise=0.15, seed=42):
    random.seed(seed)
    xs, ys = [], []
    for i in range(n // 2):
        t = random.uniform(0, math.pi)
        xs.append([math.cos(t) + random.gauss(0, noise), math.sin(t) + random.gauss(0, noise)])
        ys.append(0)
    for i in range(n // 2):
        t = random.uniform(0, math.pi)
        xs.append([1 - math.cos(t) + random.gauss(0, noise), -math.sin(t) + random.gauss(0, noise)])
        ys.append(1)
    combined = list(zip(xs, ys))
    random.shuffle(combined)
    xs, ys = zip(*combined)
    return list(xs), list(ys)
