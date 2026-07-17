import math
import random


def normal_population(mean=0, std=1, size=10000):
    data = []
    for _ in range(size):
        u1 = random.random()
        u2 = random.random()
        z = math.sqrt(-2 * math.log(u1)) * math.cos(2 * math.pi * u2)
        data.append(mean + std * z)
    return data


def binomial_population(n=20, p=0.5, size=10000):
    data = []
    for _ in range(size):
        count = sum(1 for _ in range(n) if random.random() < p)
        data.append(count)
    return data


def poisson_population(lam=4, size=10000):
    data = []
    for _ in range(size):
        L = math.exp(-lam)
        k = 0
        p = 1.0
        while p > L:
            k += 1
            p *= random.random()
        data.append(k - 1)
    return data


def uniform_population(a=0, b=10, size=10000):
    return [a + (b - a) * random.random() for _ in range(size)]


def custom_mixture(size=10000):
    data = []
    for _ in range(size):
        if random.random() < 0.5:
            u1 = random.random()
            u2 = random.random()
            z = math.sqrt(-2 * math.log(u1)) * math.cos(2 * math.pi * u2)
            data.append(0 + 1 * z)
        else:
            u1 = random.random()
            u2 = random.random()
            z = math.sqrt(-2 * math.log(u1)) * math.cos(2 * math.pi * u2)
            data.append(5 + 1.5 * z)
    return data
