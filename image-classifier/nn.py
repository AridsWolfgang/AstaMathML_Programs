import math
import random


def sigmoid(x):
    if x > 0:
        return 1.0 / (1.0 + math.exp(-x))
    ez = math.exp(x)
    return ez / (1.0 + ez)


def sigmoid_deriv(s):
    return s * (1.0 - s)


class NeuralNetwork:
    def __init__(self, input_size, hidden_size=16, output_size=4, lr=0.5):
        self.lr = lr
        self.W1 = [[random.gauss(0, 1.0 / math.sqrt(input_size)) for _ in range(hidden_size)]
                   for _ in range(input_size)]
        self.b1 = [0.0] * hidden_size
        self.W2 = [[random.gauss(0, 1.0 / math.sqrt(hidden_size)) for _ in range(output_size)]
                   for _ in range(hidden_size)]
        self.b2 = [0.0] * output_size

        self.label_to_idx = {}
        self.idx_to_label = {}
        self.output_size = output_size

    def fit(self, X, y, epochs=200):
        labels = sorted(set(y))
        self.label_to_idx = {l: i for i, l in enumerate(labels)}
        self.idx_to_label = {i: l for i, l in enumerate(labels)}
        self.output_size = len(labels)

        n = len(X)
        for epoch in range(epochs):
            indices = list(range(n))
            random.shuffle(indices)
            total_loss = 0.0
            for idx in indices:
                target = [0.0] * self.output_size
                target[self.label_to_idx[y[idx]]] = 1.0
                loss = self._train_sample(X[idx], target)
                total_loss += loss
            if epoch % 50 == 49:
                avg_loss = total_loss / n
                if avg_loss < 0.01:
                    break

    def _train_sample(self, x, target):
        h_in = [sum(x[i] * self.W1[i][j] for i in range(len(x))) + self.b1[j]
                for j in range(len(self.b1))]
        h_out = [sigmoid(v) for v in h_in]
        o_in = [sum(h_out[j] * self.W2[j][k] for j in range(len(h_out))) + self.b2[k]
                for k in range(len(self.b2))]
        o_out = [sigmoid(v) for v in o_in]

        loss = sum(-target[k] * math.log(max(o_out[k], 1e-15))
                   - (1 - target[k]) * math.log(max(1 - o_out[k], 1e-15))
                   for k in range(len(o_out)))

        delta_o = [(o_out[k] - target[k]) for k in range(len(o_out))]
        delta_h = [
            sum(delta_o[k] * self.W2[j][k] for k in range(len(delta_o))) * sigmoid_deriv(h_out[j])
            for j in range(len(h_out))
        ]

        for j in range(len(self.W2)):
            for k in range(len(self.W2[0])):
                self.W2[j][k] -= self.lr * delta_o[k] * h_out[j]
        for k in range(len(self.b2)):
            self.b2[k] -= self.lr * delta_o[k]
        for i in range(len(self.W1)):
            for j in range(len(self.W1[0])):
                self.W1[i][j] -= self.lr * delta_h[j] * x[i]
        for j in range(len(self.b1)):
            self.b1[j] -= self.lr * delta_h[j]

        return loss

    def _forward(self, x):
        h_in = [sum(x[i] * self.W1[i][j] for i in range(len(x))) + self.b1[j]
                for j in range(len(self.b1))]
        h_out = [sigmoid(v) for v in h_in]
        o_in = [sum(h_out[j] * self.W2[j][k] for j in range(len(h_out))) + self.b2[k]
                for k in range(len(self.b2))]
        o_out = [sigmoid(v) for v in o_in]
        return o_out, h_out

    def predict(self, X):
        return [self.idx_to_label[max(range(self.output_size), key=lambda k: self._forward(x)[0][k])]
                for x in X]

    def predict_proba(self, X):
        results = []
        for x in X:
            o_out, _ = self._forward(x)
            total = sum(o_out)
            proba = {self.idx_to_label[k]: v / max(total, 1e-15) for k, v in enumerate(o_out)}
            results.append(proba)
        return results
