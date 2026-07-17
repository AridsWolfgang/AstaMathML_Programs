import math
import random


def train_test_split(X, y, test_size=0.2, seed=42):
    random.seed(seed)
    combined = list(zip(X, y))
    random.shuffle(combined)
    split = int(len(combined) * (1 - test_size))
    train = combined[:split]
    test = combined[split:]
    X_train = [x for x, _ in train]
    y_train = [y for _, y in train]
    X_test = [x for x, _ in test]
    y_test = [y for _, y in test]
    return X_train, X_test, y_train, y_test


class StandardScaler:
    def __init__(self):
        self.means = []
        self.stds = []

    def fit(self, X):
        n_features = len(X[0])
        n = len(X)
        self.means = [sum(row[f] for row in X) / n for f in range(n_features)]
        self.stds = [
            math.sqrt(sum((row[f] - self.means[f])**2 for row in X) / max(n - 1, 1))
            for f in range(n_features)
        ]
        return self

    def transform(self, X):
        return [
            [(row[f] - self.means[f]) / max(self.stds[f], 1e-12) for f in range(len(self.means))]
            for row in X
        ]

    def fit_transform(self, X):
        self.fit(X)
        return self.transform(X)


def add_bias(X):
    return [[1.0] + row for row in X]
