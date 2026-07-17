import math
from collections import Counter, defaultdict


class MultinomialNB:
    def __init__(self, alpha=1.0):
        self.alpha = alpha
        self.class_log_prior = {}
        self.feature_log_prob = defaultdict(lambda: {})
        self.classes = []
        self.n_features = 0

    def fit(self, X, y):
        self.classes = sorted(set(y))
        n = len(y)
        class_counts = Counter(y)
        feature_counts = defaultdict(lambda: [0.0] * len(X[0]))

        for i in range(n):
            cls = y[i]
            for j in range(len(X[i])):
                feature_counts[cls][j] += X[i][j]

        for cls in self.classes:
            self.class_log_prior[cls] = math.log(class_counts[cls] / n)

        self.n_features = len(X[0])
        for cls in self.classes:
            total = sum(feature_counts[cls])
            for j in range(self.n_features):
                num = feature_counts[cls][j] + self.alpha
                den = total + self.alpha * self.n_features
                self.feature_log_prob[cls][j] = math.log(num / den)

    def predict_log_proba(self, X):
        results = []
        for row in X:
            scores = {}
            for cls in self.classes:
                score = self.class_log_prior[cls]
                for j in range(len(row)):
                    if row[j] > 0:
                        score += row[j] * self.feature_log_prob[cls][j]
                scores[cls] = score
            results.append(scores)
        return results

    def predict(self, X):
        log_probas = self.predict_log_proba(X)
        return [max(scores, key=scores.get) for scores in log_probas]

    def predict_proba(self, X):
        log_probas = self.predict_log_proba(X)
        results = []
        for scores in log_probas:
            max_log = max(scores.values())
            exp_scores = {c: math.exp(s - max_log) for c, s in scores.items()}
            total = sum(exp_scores.values())
            results.append({c: v / total for c, v in exp_scores.items()})
        return results
