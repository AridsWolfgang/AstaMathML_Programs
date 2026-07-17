import math


class DecisionTree:
    def __init__(self, max_depth=5, min_samples_split=2, task="regression"):
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.task = task
        self.tree = None

    def fit(self, X, y):
        self.tree = self._build(X, y, depth=0)

    def _build(self, X, y, depth):
        n = len(y)
        if depth >= self.max_depth or n < self.min_samples_split or len(set(y)) == 1:
            return self._leaf(y)

        best_gain = -1
        best_feat = None
        best_thresh = None

        n_features = len(X[0])
        for f in range(n_features):
            values = sorted(set(X[i][f] for i in range(n)))
            for i in range(len(values) - 1):
                thresh = (values[i] + values[i + 1]) / 2
                gain = self._gain(y, [X[i][f] for i in range(n)], thresh)
                if gain > best_gain:
                    best_gain = gain
                    best_feat = f
                    best_thresh = thresh

        if best_gain < 1e-12 or best_feat is None:
            return self._leaf(y)

        left_idx = [i for i in range(n) if X[i][best_feat] <= best_thresh]
        right_idx = [i for i in range(n) if X[i][best_feat] > best_thresh]

        if not left_idx or not right_idx:
            return self._leaf(y)

        left = self._build([X[i] for i in left_idx], [y[i] for i in left_idx], depth + 1)
        right = self._build([X[i] for i in right_idx], [y[i] for i in right_idx], depth + 1)

        return {"feat": best_feat, "thresh": best_thresh, "left": left, "right": right}

    def _leaf(self, y):
        if self.task == "regression":
            return {"value": sum(y) / len(y)}
        else:
            return {"value": max(set(y), key=y.count)}

    def _gain(self, y, feature_vals, thresh):
        n = len(y)
        parent = self._impurity(y)
        left_y = [y[i] for i in range(n) if feature_vals[i] <= thresh]
        right_y = [y[i] for i in range(n) if feature_vals[i] > thresh]
        if not left_y or not right_y:
            return 0
        child = (len(left_y) / n) * self._impurity(left_y) + \
                (len(right_y) / n) * self._impurity(right_y)
        return parent - child

    def _impurity(self, y):
        if self.task == "regression":
            mu = sum(y) / len(y)
            return sum((yi - mu)**2 for yi in y) / len(y)
        else:
            total = len(y)
            counts = {}
            for v in y:
                counts[v] = counts.get(v, 0) + 1
            impurity = 1.0
            for c in counts.values():
                p = c / total
                impurity -= p * p
            return max(impurity, 0)

    def predict(self, X):
        return [self._predict_one(row) for row in X]

    def _predict_one(self, row):
        node = self.tree
        while "feat" in node:
            if row[node["feat"]] <= node["thresh"]:
                node = node["left"]
            else:
                node = node["right"]
        return node["value"]

    def __str__(self):
        return self._str_tree(self.tree, 0)

    def _str_tree(self, node, depth):
        if "feat" not in node:
            val = node["value"]
            if isinstance(val, float):
                return f"  " * depth + f"-> {val:.4f}\n"
            return f"  " * depth + f"-> {val}\n"
        s = f"  " * depth + f"[x{node['feat']} <= {node['thresh']:.3f}]\n"
        s += self._str_tree(node["left"], depth + 1)
        s += self._str_tree(node["right"], depth + 1)
        return s
