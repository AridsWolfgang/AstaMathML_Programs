import math


def accuracy(y_true, y_pred):
    return sum(1 for t, p in zip(y_true, y_pred) if t == p) / max(len(y_true), 1)


def confusion_matrix(y_true, y_pred, labels=None):
    if labels is None:
        labels = sorted(set(y_true + y_pred))
    idx = {l: i for i, l in enumerate(labels)}
    n = len(labels)
    mat = [[0] * n for _ in range(n)]
    for t, p in zip(y_true, y_pred):
        mat[idx[t]][idx[p]] += 1
    return mat, labels


def precision_recall_f1(y_true, y_pred, positive_class="spam"):
    tp = sum(1 for t, p in zip(y_true, y_pred) if t == positive_class and p == positive_class)
    fp = sum(1 for t, p in zip(y_true, y_pred) if t != positive_class and p == positive_class)
    fn = sum(1 for t, p in zip(y_true, y_pred) if t == positive_class and p != positive_class)
    prec = tp / (tp + fp) if (tp + fp) > 0 else 0.0
    rec = tp / (tp + fn) if (tp + fn) > 0 else 0.0
    f1 = 2 * prec * rec / (prec + rec) if (prec + rec) > 0 else 0.0
    return prec, rec, f1


def report(y_true, y_pred, labels=None):
    if labels is None:
        labels = sorted(set(y_true + y_pred))
    cm, _ = confusion_matrix(y_true, y_pred, labels)
    print(f"\n  Accuracy: {accuracy(y_true, y_pred):.4f}")
    print(f"  Confusion matrix:")
    header = " " * 14 + "".join(f"{l:>10s}" for l in labels)
    print(header)
    for i, l in enumerate(labels):
        row = "".join(f"{cm[i][j]:10d}" for j in range(len(labels)))
        print(f"  {l:12s} {row}")
    for cls in labels:
        prec, rec, f1 = precision_recall_f1(y_true, y_pred, cls)
        if prec > 0 or rec > 0:
            print(f"  {cls:10s}: prec={prec:.3f}  rec={rec:.3f}  f1={f1:.3f}")
