import sys
import os
import math
import random

sys.path.insert(0, os.path.dirname(__file__))

from data.dataset import generate_dataset, flatten, print_image, IMG_SIZE, SHAPE_LABELS, SHAPE_FNS
from features import extract_all, extract_pixels
from knn import KNN
from nn import NeuralNetwork
from metrics import accuracy, confusion_matrix, precision_recall_f1

HEADER = """
  +--------------------------------------------+
  |        IMAGE CLASSIFIER  v1.0              |
  |  KNN + Neural Network from scratch         |
  |  8x8 synthetic shape images                |
  +--------------------------------------------+
"""


def show_menu():
    print("\n  --- Image Classifier ---")
    print("  1) View sample images from dataset")
    print("  2) Train & evaluate KNN")
    print("  3) Train & evaluate Neural Network")
    print("  4) Compare KNN vs Neural Network")
    print("  5) Classify a generated image")
    print("  Q) Quit")
    print("  Choice: ", end="")


def view_samples():
    print("\n  Sample images from each class:")
    for label, shape_fn in SHAPE_FNS.items():
        img = shape_fn()
        print(f"\n  {label}:")
        print_image(img)
    print()


def evaluate_knn():
    print("\n  --- K-Nearest Neighbors ---")
    print("  Generating dataset...")
    X_img, y = generate_dataset(samples_per_class=60, noise=0.15)
    X = flatten(X_img)

    split = int(len(X) * 0.7)
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]

    for k in [1, 3, 5, 7]:
        knn = KNN(k=k)
        knn.fit(X_train, y_train)
        preds = knn.predict(X_test)
        acc = accuracy(y_test, preds)
        print(f"  K={k}: accuracy = {acc:.4f}")

    best_k = 3
    knn = KNN(k=best_k)
    knn.fit(X_train, y_train)
    preds = knn.predict(X_test)
    print(f"\n  Best K={best_k} details:")
    print(f"    Accuracy: {accuracy(y_test, preds):.4f}")
    for label in SHAPE_LABELS:
        prec, rec, f1 = precision_recall_f1(y_test, preds, label)
        print(f"    {label:10s}: prec={prec:.3f}  rec={rec:.3f}  f1={f1:.3f}")


def evaluate_nn():
    print("\n  --- Neural Network ---")
    print("  Generating dataset...")
    X_img, y = generate_dataset(samples_per_class=80, noise=0.15)
    X = flatten(X_img)
    split = int(len(X) * 0.7)
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]

    input_size = len(X[0])
    nn = NeuralNetwork(input_size=input_size, hidden_size=16, output_size=4, lr=0.5)
    print(f"  Input size: {input_size}, Hidden: 16, Output: 4")
    print(f"  Training for up to 200 epochs...")
    nn.fit(X_train, y_train, epochs=200)
    preds = nn.predict(X_test)
    print(f"\n  Results:")
    print(f"    Accuracy: {accuracy(y_test, preds):.4f}")
    for label in SHAPE_LABELS:
        prec, rec, f1 = precision_recall_f1(y_test, preds, label)
        print(f"    {label:10s}: prec={prec:.3f}  rec={rec:.3f}  f1={f1:.3f}")

    probas = nn.predict_proba(X_test[:5])
    print(f"\n  Sample predictions:")
    for i in range(min(5, len(X_test))):
        actual = y_test[i]
        pred = preds[i]
        conf = max(probas[i].values())
        print(f"    Actual={actual:8s} Pred={pred:8s} Conf={conf:.3f}")


def compare_models():
    print("\n  --- KNN vs Neural Network ---")
    print("  Generating dataset...")
    X_img, y = generate_dataset(samples_per_class=80, noise=0.15)
    X = flatten(X_img)
    split = int(len(X) * 0.7)
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = y[:split], y[split:]

    import time

    t0 = time.time()
    knn = KNN(k=3)
    knn.fit(X_train, y_train)
    knn_preds = knn.predict(X_test)
    knn_time = time.time() - t0
    knn_acc = accuracy(y_test, knn_preds)

    t0 = time.time()
    nn = NeuralNetwork(input_size=len(X[0]), hidden_size=16, output_size=4, lr=0.5)
    nn.fit(X_train, y_train, epochs=200)
    nn_preds = nn.predict(X_test)
    nn_time = time.time() - t0
    nn_acc = accuracy(y_test, nn_preds)

    print(f"\n  {'Method':20s} {'Accuracy':>10s} {'Time':>10s}")
    print(f"  {'-'*20} {'-'*10} {'-'*10}")
    print(f"  {'KNN (k=3)':20s} {knn_acc:10.4f} {knn_time:9.3f}s")
    print(f"  {'Neural Network':20s} {nn_acc:10.4f} {nn_time:9.3f}s")
    print()


def classify_generated():
    print("\n  --- Classify a Generated Image ---")
    print(f"  Shapes: {', '.join(SHAPE_LABELS)}")
    shape = input("  Enter shape name: ").strip().lower()
    if shape not in SHAPE_FNS:
        print(f"  Unknown shape. Choose from: {', '.join(SHAPE_LABELS)}")
        return

    try:
        noise = float(input("  Noise level (0-0.5, default 0.15): ") or "0.15")
    except ValueError:
        noise = 0.15

    X_img, _ = generate_dataset(samples_per_class=80, noise=0.15)
    X = flatten(X_img)
    labels = [s for s, _ in zip(SHAPE_LABELS * 80, range(320))]
    split = int(len(X) * 0.7)
    X_train, X_test = X[:split], X[split:]
    y_train, y_test = labels[:split], labels[split:]

    knn = KNN(k=3)
    knn.fit(X_train, y_train)

    nn = NeuralNetwork(input_size=len(X[0]), hidden_size=16, output_size=4, lr=0.5)
    nn.fit(X_train, y_train, epochs=200)

    test_img = SHAPE_FNS[shape]()
    noisy_img = add_noise(test_img, noise)
    flat = flatten([noisy_img])[0]

    print(f"\n  Generated {shape} with noise={noise}:")
    print_image(noisy_img)

    knn_pred = knn.predict([flat])[0]
    knn_proba = knn.predict_proba([flat])[0]

    nn_preds = nn.predict([flat])
    nn_proba = nn.predict_proba([flat])[0]

    print(f"\n  KNN prediction:  {knn_pred:>8s} (conf={max(knn_proba.values()):.3f})")
    print(f"  NN  prediction:  {nn_preds[0]:>8s} (conf={max(nn_proba.values()):.3f})")
    print()

    for label in SHAPE_LABELS:
        print(f"    P({label:8s})  KNN={knn_proba.get(label, 0):.3f}  NN={nn_proba.get(label, 0):.3f}")


def add_noise(img, intensity=0.15):
    return [
        [min(1.0, max(0.0, v + random.gauss(0, intensity))) for v in row]
        for row in img
    ]


from metrics import accuracy, confusion_matrix, precision_recall_f1
from nn import NeuralNetwork


def main():
    print(HEADER)

    while True:
        show_menu()
        choice = input().strip().lower()
        if choice == "1":
            view_samples()
        elif choice == "2":
            evaluate_knn()
        elif choice == "3":
            evaluate_nn()
        elif choice == "4":
            compare_models()
        elif choice == "5":
            classify_generated()
        elif choice == "q":
            print("\n  Goodbye!")
            break
        else:
            print("  Invalid choice.")


if __name__ == "__main__":
    main()
