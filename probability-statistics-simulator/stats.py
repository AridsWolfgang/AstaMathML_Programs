import math


def mean(data):
    if not data:
        return 0.0
    return sum(data) / len(data)


def variance(data, ddof=0):
    if len(data) < 2:
        return 0.0
    mu = mean(data)
    return sum((x - mu) ** 2 for x in data) / (len(data) - ddof)


def std(data, ddof=0):
    return math.sqrt(variance(data, ddof))


def moment(data, k):
    mu = mean(data)
    return sum((x - mu) ** k for x in data) / len(data)


def skewness(data):
    if len(data) < 3:
        return 0.0
    s = std(data, ddof=0)
    if s == 0:
        return 0.0
    return moment(data, 3) / (s ** 3)


def kurtosis(data, excess=True):
    if len(data) < 4:
        return 0.0
    s = std(data, ddof=0)
    if s == 0:
        return 0.0
    kurt = moment(data, 4) / (s ** 4)
    if excess:
        kurt -= 3.0
    return kurt


def covariance(x, y):
    if len(x) != len(y) or len(x) < 2:
        return 0.0
    mx = mean(x)
    my = mean(y)
    return sum((xi - mx) * (yi - my) for xi, yi in zip(x, y)) / (len(x) - 1)


def correlation(x, y):
    cov = covariance(x, y)
    sx = std(x, ddof=1)
    sy = std(y, ddof=1)
    if sx == 0 or sy == 0:
        return 0.0
    return cov / (sx * sy)
