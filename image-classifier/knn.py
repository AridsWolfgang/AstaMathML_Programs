import math
from collections import Counter


class KNN:
    def __init__(self, k=3):
        self.k = k
        self.X_train = None
        self.y_train = None

    def fit(self, X, y):
        self.X_train = X
        self.y_train = y

    def _distance(self, a, b):
        return math.sqrt(sum((a[i] - b[i])**2 for i in range(len(a))))

    def predict(self, X):
        results = []
        for x in X:
            dists = [(self._distance(x, x_train), y_train)
                     for x_train, y_train in zip(self.X_train, self.y_train)]
            dists.sort(key=lambda d: d[0])
            neighbors = dists[:self.k]
            votes = Counter(label for _, label in neighbors)
            results.append(votes.most_common(1)[0][0])
        return results

    def predict_proba(self, X):
        results = []
        for x in X:
            dists = [(self._distance(x, x_train), y_train)
                     for x_train, y_train in zip(self.X_train, self.y_train)]
            dists.sort(key=lambda d: d[0])
            neighbors = dists[:self.k]
            votes = Counter(label for _, label in neighbors)
            total = sum(votes.values())
            results.append({label: count / total for label, count in votes.items()})
        return results
