import math


def cosine_similarity(a, b):
    dot = sum(ai * bi for ai, bi in zip(a, b))
    na = math.sqrt(sum(x*x for x in a))
    nb = math.sqrt(sum(x*x for x in b))
    if na == 0 or nb == 0:
        return 0.0
    return dot / (na * nb)


def pearson_correlation(a, b):
    n = len(a)
    mu_a = sum(a) / n
    mu_b = sum(b) / n
    diff_a = [x - mu_a for x in a]
    diff_b = [x - mu_b for x in b]
    num = sum(diff_a[i] * diff_b[i] for i in range(n))
    den_a = math.sqrt(sum(x*x for x in diff_a))
    den_b = math.sqrt(sum(x*x for x in diff_b))
    if den_a == 0 or den_b == 0:
        return 0.0
    return num / (den_a * den_b)


def jaccard_similarity(set_a, set_b):
    if not set_a or not set_b:
        return 0.0
    inter = len(set_a & set_b)
    union = len(set_a | set_b)
    return inter / union if union > 0 else 0.0
