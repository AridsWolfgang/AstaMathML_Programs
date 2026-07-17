import math
import random
from stats import mean, std


def monte_carlo_pi(num_samples=100000):
    inside = 0
    for _ in range(num_samples):
        x = random.random() * 2 - 1
        y = random.random() * 2 - 1
        if x * x + y * y <= 1:
            inside += 1
    return 4.0 * inside / num_samples


def monte_carlo_integral(func, a, b, num_samples=100000):
    total = 0.0
    for _ in range(num_samples):
        x = a + (b - a) * random.random()
        total += func(x)
    return (b - a) * total / num_samples


def dice_roll_simulation(num_rolls=100000, num_dice=2, sides=6):
    counts = {}
    for _ in range(num_rolls):
        total = sum(random.randint(1, sides) for _ in range(num_dice))
        counts[total] = counts.get(total, 0) + 1
    return {k: v / num_rolls for k, v in sorted(counts.items())}


def bootstrap_ci(data, statistic_func, num_resamples=1000, ci=0.95):
    n = len(data)
    stats = []
    for _ in range(num_resamples):
        sample = [random.choice(data) for _ in range(n)]
        stats.append(statistic_func(sample))
    stats.sort()
    lower_idx = int((1 - ci) / 2 * num_resamples)
    upper_idx = int((1 + ci) / 2 * num_resamples) - 1
    return stats[lower_idx], stats[upper_idx]


def bootstrap_mean_ci(data, num_resamples=1000, ci=0.95):
    return bootstrap_ci(data, mean, num_resamples, ci)


def bootstrap_std_ci(data, num_resamples=1000, ci=0.95):
    return bootstrap_ci(data, std, num_resamples, ci)
