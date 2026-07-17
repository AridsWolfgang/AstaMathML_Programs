import sys
import os

sys.path.insert(0, os.path.dirname(__file__))

from data.datasets import linear_data, polynomial_data, multi_linear_data, classification_data, moons_data
from preprocessing import train_test_split, StandardScaler, add_bias
from linear_regression import LinearRegression
from logistic_regression import LogisticRegression
from decision_tree import DecisionTree
from metrics import (
    mean_squared_error, root_mean_squared_error, mean_absolute_error,
    r2_score, accuracy, precision_recall_f1, confusion_matrix,
)

HEADER = """
  +--------------------------------------------+
  |       PREDICTIVE MODEL  v1.0               |
  |  Linear Regression + Logistic Regression   |
  |  + Decision Trees - built from scratch     |
  +--------------------------------------------+
"""


def show_menu():
    print("\n  --- Predictive Model ---")
    print("  1) Linear Regression (OLS normal equation)")
    print("  2) Linear Regression (SGD mini-batch)")
    print("  3) Logistic Regression (binary)")
    print("  4) Decision Tree (classification)")
    print("  5) Decision Tree (regression)")
    print("  6) Run all comparisons")
    print("  Q) Quit")
    print("  Choice: ", end="")


def demo_linear_ols():
    print("\n  --- Linear Regression (OLS) ---")
    print("  Dataset: y = 3.5x + 7.2 + noise")
    n = 100
    X, y = linear_data(n, noise=2.0)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

    scaler = StandardScaler()
    X_train_s = scaler.fit_transform(X_train)
    X_test_s = scaler.transform(X_test)

    model = LinearRegression()
    model.fit_ols(X_train_s, y_train)
    pred = model.predict(X_test_s)

    print(f"\n  Learned: {model}")
    print(f"\n  Test set metrics:")
    print(f"    RMSE: {root_mean_squared_error(y_test, pred):.4f}")
    print(f"    MAE:  {mean_absolute_error(y_test, pred):.4f}")
    print(f"    R2:   {r2_score(y_test, pred):.4f}")


def demo_linear_sgd():
    print("\n  --- Linear Regression (SGD) ---")
    print("  Dataset: y = 2.0*x1 - 1.5*x2 + 0.5*x3 + 5.0")
    n = 200
    X, y = multi_linear_data(n, noise=1.0)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

    scaler = StandardScaler()
    X_train_s = scaler.fit_transform(X_train)
    X_test_s = scaler.transform(X_test)

    model = LinearRegression()
    model.fit_sgd(X_train_s, y_train, lr=0.01, epochs=500, batch_size=16)
    pred = model.predict(X_test_s)

    print(f"\n  Learned: {model}")
    print(f"\n  Test set metrics:")
    print(f"    RMSE: {root_mean_squared_error(y_test, pred):.4f}")
    print(f"    MAE:  {mean_absolute_error(y_test, pred):.4f}")
    print(f"    R2:   {r2_score(y_test, pred):.4f}")


def demo_logistic():
    print("\n  --- Logistic Regression ---")
    print("  Dataset: two Gaussian blobs (2D)")
    n = 200
    X, y = classification_data(n)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

    scaler = StandardScaler()
    X_train_s = scaler.fit_transform(X_train)
    X_test_s = scaler.transform(X_test)

    model = LogisticRegression(lr=0.1, epochs=1000)
    model.fit(X_train_s, y_train)
    pred = model.predict(X_test_s)
    proba = model.predict_proba(X_test_s)

    acc = accuracy(y_test, pred)
    prec, rec, f1 = precision_recall_f1(y_test, pred)
    cm, labels = confusion_matrix(y_test, pred)

    print(f"\n  Learned: {model}")
    print(f"\n  Test set metrics:")
    print(f"    Accuracy:  {acc:.4f}")
    print(f"    Precision: {prec:.4f}")
    print(f"    Recall:    {rec:.4f}")
    print(f"    F1:        {f1:.4f}")
    print(f"  Confusion matrix ({labels}):")
    for row in cm:
        print(f"    {row}")

    print(f"\n  Sample probabilities:")
    for i in range(min(5, len(X_test))):
        true_vals = "yes" if y_test[i] == 1 else "no"
        print(f"    P(y=1)={proba[i]:.4f}  predicted={'yes' if pred[i]==1 else 'no':>3s}  actual={true_vals}")


def demo_dt_classification():
    print("\n  --- Decision Tree (Classification) ---")
    print("  Dataset: two moons (non-linear boundary)")
    n = 200
    X, y = moons_data(n, noise=0.2)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

    model = DecisionTree(max_depth=6, min_samples_split=4, task="classification")
    model.fit(X_train, y_train)
    pred = model.predict(X_test)

    acc = accuracy(y_test, pred)
    prec, rec, f1 = precision_recall_f1(y_test, pred)
    cm, labels = confusion_matrix(y_test, pred)

    print(f"\n  Tree depth: {model.max_depth}")
    print(f"\n  Test set metrics:")
    print(f"    Accuracy:  {acc:.4f}")
    print(f"    Precision: {prec:.4f}")
    print(f"    Recall:    {rec:.4f}")
    print(f"    F1:        {f1:.4f}")
    print(f"  Confusion matrix ({labels}):")
    for row in cm:
        print(f"    {row}")
    print(f"\n  Tree structure:")
    print(f"  (root)")
    print(model)


def demo_dt_regression():
    print("\n  --- Decision Tree (Regression) ---")
    print("  Dataset: y = 2x^3 - 3x^2 + 4x + 1 + noise")
    n = 100
    X, y = polynomial_data(n, degree=3, noise=5.0)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

    model = DecisionTree(max_depth=5, min_samples_split=5, task="regression")
    model.fit(X_train, y_train)
    pred = model.predict(X_test)

    print(f"\n  Test set metrics:")
    print(f"    RMSE: {root_mean_squared_error(y_test, pred):.4f}")
    print(f"    MAE:  {mean_absolute_error(y_test, pred):.4f}")
    print(f"    R2:   {r2_score(y_test, pred):.4f}")
    print(f"\n  Tree structure:")
    print(f"  (root)")
    print(model)


def run_all():
    print("\n  ====== ALL MODELS COMPARISON ======\n")

    print("  [1/4] Linear Regression (OLS)...")
    X, y = linear_data(100, noise=2.0)
    Xtr, Xte, ytr, yte = train_test_split(X, y)
    scaler = StandardScaler()
    lr = LinearRegression()
    lr.fit_ols(scaler.fit_transform(Xtr), ytr)
    p = lr.predict(scaler.transform(Xte))
    print(f"    R2={r2_score(yte, p):.4f}  RMSE={root_mean_squared_error(yte, p):.4f}\n")

    print("  [2/4] Linear Regression (SGD)...")
    lr2 = LinearRegression()
    lr2.fit_sgd(scaler.fit_transform(Xtr), ytr, lr=0.01, epochs=500, batch_size=16)
    p2 = lr2.predict(scaler.transform(Xte))
    print(f"    R2={r2_score(yte, p2):.4f}  RMSE={root_mean_squared_error(yte, p2):.4f}\n")

    print("  [3/4] Logistic Regression...")
    Xc, yc = classification_data(200)
    Xctr, Xcte, yctr, ycte = train_test_split(Xc, yc)
    logreg = LogisticRegression(lr=0.1, epochs=1000)
    logreg.fit(Xctr, yctr)
    pc = logreg.predict(Xcte)
    print(f"    acc={accuracy(ycte, pc):.4f}  f1={precision_recall_f1(ycte, pc)[2]:.4f}\n")

    print("  [4/4] Decision Tree...")
    dt = DecisionTree(max_depth=6, task="classification")
    dt.fit(Xctr, yctr)
    pd = dt.predict(Xcte)
    print(f"    acc={accuracy(ycte, pd):.4f}  f1={precision_recall_f1(ycte, pd)[2]:.4f}\n")

    print("  ====== DONE ======")


def main():
    print(HEADER)
    while True:
        show_menu()
        choice = input().strip().lower()
        if choice == "1":
            demo_linear_ols()
        elif choice == "2":
            demo_linear_sgd()
        elif choice == "3":
            demo_logistic()
        elif choice == "4":
            demo_dt_classification()
        elif choice == "5":
            demo_dt_regression()
        elif choice == "6":
            run_all()
        elif choice == "q":
            print("\n  Goodbye!")
            break
        else:
            print("  Invalid choice.")


if __name__ == "__main__":
    main()
