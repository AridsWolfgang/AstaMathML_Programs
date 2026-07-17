import math
import random


class LinearRegression:
    def __init__(self, fit_intercept=True):
        self.coef_ = None
        self.intercept_ = 0.0
        self.fit_intercept = fit_intercept

    def fit_ols(self, X, y):
        n = len(X)
        k = len(X[0])
        if self.fit_intercept:
            X = [[1.0] + row for row in X]
            k += 1
        Xt = list(zip(*X))
        XtX = [[sum(Xt[r][i] * Xt[c][i] for i in range(n)) for c in range(k)] for r in range(k)]
        Xty = [sum(Xt[r][i] * y[i] for i in range(n)) for r in range(k)]
        beta = self._solve(XtX, Xty)
        if self.fit_intercept:
            self.intercept_ = beta[0]
            self.coef_ = beta[1:]
        else:
            self.coef_ = beta

    def _solve(self, A, b):
        n = len(A)
        aug = [row[:] + [b[i]] for i, row in enumerate(A)]
        for col in range(n):
            pivot = max(range(col, n), key=lambda r: abs(aug[r][col]))
            if abs(aug[pivot][col]) < 1e-12:
                continue
            aug[col], aug[pivot] = aug[pivot], aug[col]
            piv_val = aug[col][col]
            for j in range(col, n + 1):
                aug[col][j] /= piv_val
            for r in range(n):
                if r != col:
                    factor = aug[r][col]
                    for j in range(col, n + 1):
                        aug[r][j] -= factor * aug[col][j]
        return [aug[i][n] for i in range(n)]

    def fit_sgd(self, X, y, lr=0.01, epochs=100, batch_size=None):
        n = len(X)
        k = len(X[0])
        if self.fit_intercept:
            X = [[1.0] + row for row in X]
            k += 1
        self.coef_ = [0.0] * (k - 1) if self.fit_intercept else [0.0] * k
        self.intercept_ = 0.0
        weights = [0.0] * k

        if batch_size is None:
            batch_size = n

        for epoch in range(epochs):
            indices = list(range(n))
            random.shuffle(indices)
            for start in range(0, n, batch_size):
                batch = indices[start:start + batch_size]
                grad = [0.0] * k
                for i in batch:
                    pred = sum(weights[j] * X[i][j] for j in range(k))
                    err = pred - y[i]
                    for j in range(k):
                        grad[j] += err * X[i][j]
                for j in range(k):
                    weights[j] -= lr * grad[j] / len(batch)

        if self.fit_intercept:
            self.intercept_ = weights[0]
            self.coef_ = weights[1:]
        else:
            self.coef_ = weights

    def predict(self, X):
        return [self.intercept_ + sum(self.coef_[j] * row[j] for j in range(len(self.coef_))) for row in X]

    def __str__(self):
        terms = [f"{c:+.4f}*x{i}" for i, c in enumerate(self.coef_)]
        return f"y = {self.intercept_:.4f} " + " ".join(terms)
