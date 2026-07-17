import math


def rmse(predictions, actuals, mask):
    n = 0
    sq_err = 0.0
    for u in range(len(predictions)):
        for i in range(len(predictions[0])):
            if mask[u][i]:
                sq_err += (predictions[u][i] - actuals[u][i]) ** 2
                n += 1
    return math.sqrt(sq_err / n) if n > 0 else 0.0


def mae(predictions, actuals, mask):
    n = 0
    abs_err = 0.0
    for u in range(len(predictions)):
        for i in range(len(predictions[0])):
            if mask[u][i]:
                abs_err += abs(predictions[u][i] - actuals[u][i])
                n += 1
    return abs_err / n if n > 0 else 0.0


def precision_at_k(recommended, relevant, k):
    top_k = recommended[:k]
    if not top_k:
        return 0.0
    hits = sum(1 for item in top_k if item in relevant)
    return hits / k


def recall_at_k(recommended, relevant, k):
    top_k = recommended[:k]
    if not relevant:
        return 0.0
    hits = sum(1 for item in top_k if item in relevant)
    return hits / len(relevant)
