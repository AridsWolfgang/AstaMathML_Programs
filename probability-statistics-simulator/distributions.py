import math
import random


class Normal:
    def __init__(self, mean=0, std=1):
        self.mean = mean
        self.std = std

    def pdf(self, x):
        return (1 / (self.std * math.sqrt(2 * math.pi))) * math.exp(
            -0.5 * ((x - self.mean) / self.std) ** 2
        )

    def cdf(self, x):
        return 0.5 * (1 + math.erf((x - self.mean) / (self.std * math.sqrt(2))))

    def sample(self, n=1):
        samples = []
        for _ in range(n):
            u1 = random.random()
            u2 = random.random()
            z = math.sqrt(-2 * math.log(u1)) * math.cos(2 * math.pi * u2)
            samples.append(self.mean + self.std * z)
        return samples if n > 1 else samples[0]


class Binomial:
    def __init__(self, n=10, p=0.5):
        self.n = n
        self.p = p

    def pmf(self, k):
        if k < 0 or k > self.n:
            return 0.0
        return self._comb(self.n, k) * (self.p ** k) * ((1 - self.p) ** (self.n - k))

    def cdf(self, k):
        return sum(self.pmf(i) for i in range(int(k) + 1))

    def _comb(self, n, k):
        if k < 0 or k > n:
            return 0
        k = min(k, n - k)
        result = 1
        for i in range(1, k + 1):
            result = result * (n - k + i) // i
        return result

    def sample(self, n=1):
        samples = []
        for _ in range(n):
            count = sum(1 for _ in range(self.n) if random.random() < self.p)
            samples.append(count)
        return samples if n > 1 else samples[0]


class Poisson:
    def __init__(self, lam=4):
        self.lam = lam

    def pmf(self, k):
        if k < 0:
            return 0.0
        return (math.exp(-self.lam) * (self.lam ** k)) / math.factorial(k)

    def cdf(self, k):
        return sum(self.pmf(i) for i in range(int(k) + 1))

    def sample(self, n=1):
        samples = []
        for _ in range(n):
            L = math.exp(-self.lam)
            k = 0
            p = 1.0
            while p > L:
                k += 1
                p *= random.random()
            samples.append(k - 1)
        return samples if n > 1 else samples[0]


class Uniform:
    def __init__(self, a=0, b=10):
        self.a = a
        self.b = b

    def pdf(self, x):
        if x < self.a or x > self.b:
            return 0.0
        return 1.0 / (self.b - self.a)

    def cdf(self, x):
        if x < self.a:
            return 0.0
        if x > self.b:
            return 1.0
        return (x - self.a) / (self.b - self.a)

    def sample(self, n=1):
        samples = [self.a + (self.b - self.a) * random.random() for _ in range(n)]
        return samples if n > 1 else samples[0]


def ascii_histogram(data, bins=20, width=60, title="Distribution"):
    """Render an ASCII histogram of the data."""
    if not data:
        return "[Empty data]"
    mn = min(data)
    mx = max(data)
    if mx == mn:
        return f"[All values = {mn}]"
    bin_width = (mx - mn) / bins
    counts = [0] * bins
    for v in data:
        idx = min(int((v - mn) / bin_width), bins - 1)
        counts[idx] += 1
    max_count = max(counts) if max(counts) > 0 else 1
    lines = [f"--- {title} ---"]
    lines.append(f"Range: [{mn:.4f}, {mx:.4f}]  N={len(data)}")
    lines.append("")
    for i in range(bins):
        lo = mn + i * bin_width
        hi = lo + bin_width
        bar_len = int((counts[i] / max_count) * width)
        bar = "#" * bar_len
        label = f"[{lo:7.2f} - {hi:7.2f}]"
        lines.append(f"{label} |{bar} {counts[i]}")
    return "\n".join(lines)
