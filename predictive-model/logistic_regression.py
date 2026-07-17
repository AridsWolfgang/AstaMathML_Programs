import math
import random


def sigmoid(z):
    if z > 0:
        return 1.0 / (1.0 + math.exp(-z))
    else:
        ez = math.exp(z)
        return ez / (1.0 + ez)


class LogisticRegression:
    def __init__(self, lr=0.1, epochs=1000, fit_intercept=True):
        self.lr = lr
        self.epochs = epochs
        self.fit_intercept = fit_intercept
        self.coef_ = None
        self.intercept_ = 0.0

    def fit(self, X, y):
        n = len(X)
        k = len(X[0])
        if self.fit_intercept:
            Xe = [[1.0] + row for row in X]
        else:
            Xe = [row[:] for row in X]
        dim = k + 1 if self.fit_intercept else k
        weights = [0.0] * dim

        for epoch in range(self.epochs):
            indices = list(range(n))
            random.shuffle(indices)
            for i in indices:
                z = sum(weights[j] * Xe[i][j] for j in range(dim))
                p = sigmoid(z)
                err = p - y[i]
                for j in range(dim):
                    weights[j] -= self.lr * err * Xe[i][j]

        if self.fit_intercept:
            self.intercept_ = weights[0]
            self.coef_ = weights[1:]
        else:
            self.coef_ = weights

    def predict_proba(self, X):
        if self.fit_intercept:
            Xe = [[1.0] + row for row in X]
        else:
            Xe = [row[:] for row in X]
        dim = len(self.coef_) + 1 if self.fit_intercept else len(self.coef_)
        result = []
        for i in range(len(X)):
            z = (self.intercept_ if self.fit_intercept else 0.0) * Xe[i][0]
            offset = 1 if self.fit_intercept else 0
            for j in range(len(self.coef_)):
                z += self.coef_[j] * Xe[i][j + offset]
            result.append(sigmoid(z))
        return result

    def predict(self, X):
        return [1 if p >= 0.5 else 0 for p in self.predict_proba(X)]

    def __str__(self):
        terms = [f"{c:+.4f}*x{i}" for i, c in enumerate(self.coef_)]
        return f"σ(y) = σ({self.intercept_:.4f} " + " ".join(terms) + ")"
