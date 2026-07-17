import math
import random
from stats import mean, variance, std


def t_test_independent(sample1, sample2, equal_var=True):
    n1, n2 = len(sample1), len(sample2)
    if n1 < 2 or n2 < 2:
        return None, None
    m1, m2 = mean(sample1), mean(sample2)
    v1, v2 = variance(sample1, ddof=1), variance(sample2, ddof=1)
    if equal_var:
        sp = ((n1 - 1) * v1 + (n2 - 1) * v2) / (n1 + n2 - 2)
        se = math.sqrt(sp * (1.0 / n1 + 1.0 / n2))
        df = n1 + n2 - 2
    else:
        se = math.sqrt(v1 / n1 + v2 / n2)
        num = (v1 / n1 + v2 / n2) ** 2
        denom = ((v1 / n1) ** 2 / (n1 - 1)) + ((v2 / n2) ** 2 / (n2 - 1))
        df = num / denom if denom > 0 else 1
    t_stat = (m1 - m2) / se if se != 0 else 0.0
    p_val = 2.0 * _t_cdf(-abs(t_stat), df)
    return t_stat, p_val


def _t_cdf(x, df):
    """Compute P(T <= x) for Student's t distribution using incomplete beta."""
    if df <= 0:
        return 0.5
    if x == 0:
        return 0.5
    x2 = df / (df + x * x)
    ib = _betainc(df / 2.0, 0.5, x2)
    if x > 0:
        return 1.0 - 0.5 * ib
    return 0.5 * ib


def _betainc(a, b, x, n=100):
    """Compute regularized incomplete beta function I_x(a,b) using continued fraction."""
    if x < 0 or x > 1:
        return 0.0
    if x == 0 or x == 1:
        return 0.0 if x == 0 else 1.0
    bt = (
        math.exp(
            math.lgamma(a + b)
            - math.lgamma(a)
            - math.lgamma(b)
            + a * math.log(x)
            + b * math.log(1 - x)
        )
    )
    if x < (a + 1) / (a + b + 2):
        return bt * _betacf(a, b, x, n) / a
    return 1.0 - bt * _betacf(b, a, 1 - x, n) / b


def _betacf(a, b, x, iters=100):
    """Continued fraction for incomplete beta."""
    qab = a + b
    qap = a + 1.0
    qam = a - 1.0
    c = 1.0
    d = 1.0 - qab * x / qap
    if abs(d) < 1e-20:
        d = 1e-20
    d = 1.0 / d
    h = d
    for m in range(1, iters + 1):
        m2 = 2 * m
        aa = m * (b - m) * x / ((qam + m2) * (a + m2))
        d = 1.0 + aa * d
        if abs(d) < 1e-20:
            d = 1e-20
        c = 1.0 + aa / c
        if abs(c) < 1e-20:
            c = 1e-20
        d = 1.0 / d
        h *= d * c
        aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2))
        d = 1.0 + aa * d
        if abs(d) < 1e-20:
            d = 1e-20
        c = 1.0 + aa / c
        if abs(c) < 1e-20:
            c = 1e-20
        d = 1.0 / d
        delta = d * c
        h *= delta
        if abs(delta - 1.0) < 1e-10:
            break
    return h


def chi_square_test(observed, expected=None):
    """Pearson's chi-square goodness-of-fit test.

    observed: list of observed frequencies.
    expected: list of expected frequencies (uniform if None).
    """
    k = len(observed)
    if expected is None:
        n = sum(observed)
        expected = [n / k] * k
    if any(e <= 0 for e in expected):
        return None, None
    chi2 = sum((o - e) ** 2 / e for o, e in zip(observed, expected))
    df = k - 1
    p_val = 1.0 - _chi2_cdf(chi2, df)
    return chi2, p_val


def _chi2_cdf(x, k):
    """CDF of chi-square distribution using regularized gamma."""
    if x <= 0:
        return 0.0
    return _gammainc(k / 2.0, x / 2.0)


def _gammainc(a, x, iters=200):
    """Regularized lower incomplete gamma P(a,x) using continued fraction."""
    if x <= 0 or a <= 0:
        return 0.0
    gln = math.lgamma(a)
    b = x + 1.0 - a
    c = 1.0 / 1e-30
    d = 1.0 / b if abs(b) < 1e-30 else 1.0 / b
    h = d
    for i in range(1, iters + 1):
        an = -i * (i - a)
        b += 2.0
        d = an * d + b
        if abs(d) < 1e-30:
            d = 1e-30
        c = b + an / c
        if abs(c) < 1e-30:
            c = 1e-30
        d = 1.0 / d
        delta = d * c
        h *= delta
        if abs(delta - 1.0) < 3e-7:
            break
    return math.exp(a * math.log(x) - x - gln) * h


def one_sample_t_test(data, mu0=0):
    n = len(data)
    if n < 2:
        return None, None
    m = mean(data)
    s = std(data, ddof=1)
    if s == 0:
        return 0.0, 1.0
    t_stat = (m - mu0) / (s / math.sqrt(n))
    df = n - 1
    p_val = 2.0 * _t_cdf(-abs(t_stat), df)
    return t_stat, p_val
