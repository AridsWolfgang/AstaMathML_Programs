import math
import random


class FunkSVD:
    def __init__(self, n_factors=10, lr=0.005, reg=0.02, epochs=100):
        self.n_factors = n_factors
        self.lr = lr
        self.reg = reg
        self.epochs = epochs
        self.user_factors = None
        self.item_factors = None
        self.user_bias = None
        self.item_bias = None
        self.global_mean = 0.0

    def fit(self, ratings, mask, n_users, n_items):
        self.global_mean = sum(
            ratings[u][i]
            for u in range(n_users) for i in range(n_items) if mask[u][i]
        ) / sum(sum(mask[u]) for u in range(n_users))

        self.user_factors = [
            [random.gauss(0, 0.1) for _ in range(self.n_factors)]
            for _ in range(n_users)
        ]
        self.item_factors = [
            [random.gauss(0, 0.1) for _ in range(self.n_factors)]
            for _ in range(n_items)
        ]
        self.user_bias = [0.0] * n_users
        self.item_bias = [0.0] * n_items

        training_examples = [(u, i, ratings[u][i])
                             for u in range(n_users) for i in range(n_items) if mask[u][i]]

        for epoch in range(self.epochs):
            random.shuffle(training_examples)
            total_loss = 0.0
            n = 0
            for u, i, r in training_examples:
                pred = self._predict(u, i)
                err = r - pred
                total_loss += err * err
                n += 1

                bu = self.user_bias[u]
                bi = self.item_bias[i]
                uf = self.user_factors[u]
                itf = self.item_factors[i]

                self.user_bias[u] += self.lr * (err - self.reg * bu)
                self.item_bias[i] += self.lr * (err - self.reg * bi)

                for f in range(self.n_factors):
                    u_old = uf[f]
                    i_old = itf[f]
                    self.user_factors[u][f] += self.lr * (err * i_old - self.reg * u_old)
                    self.item_factors[i][f] += self.lr * (err * u_old - self.reg * i_old)

        return self

    def _predict(self, u, i):
        pred = self.global_mean + self.user_bias[u] + self.item_bias[i]
        for f in range(self.n_factors):
            pred += self.user_factors[u][f] * self.item_factors[i][f]
        return max(0.5, min(5.0, pred))

    def predict_all(self, mask, n_users, n_items):
        preds = [[0.0] * n_items for _ in range(n_users)]
        for u in range(n_users):
            for i in range(n_items):
                if mask[u][i]:
                    preds[u][i] = 0.0
                else:
                    preds[u][i] = self._predict(u, i)
        return preds
