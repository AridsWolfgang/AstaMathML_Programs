# Predictive Model

> Four predictive models implemented from scratch in Python — **Ordinary Least Squares**, **Stochastic Gradient Descent**, **Logistic Regression**, and **Decision Trees (CART)** — trained on synthetic datasets with full metric evaluation.

## Architecture

```
predictive-model/
├── main.py                  # Interactive REPL with per-model demos
├── linear_regression.py     # OLS (normal equation) + SGD (mini-batch)
├── logistic_regression.py   # Binary classification with sigmoid + cross-entropy
├── decision_tree.py         # CART: classification (Gini) + regression (MSE)
├── preprocessing.py         # Train/test split, StandardScaler, bias term
├── metrics.py               # RMSE, MAE, R2, accuracy, precision, recall, F1, confusion matrix
├── data/
│   └── datasets.py          # Synthetic datasets (linear, polynomial, moons, blobs)
├── build/
└── README.md
```

## Algorithms

### 1. Linear Regression — OLS (`linear_regression.py:fit_ols`)

Closed-form solution using the normal equation:

```
β = (XᵀX)⁻¹ Xᵀy
```

Implemented with **Gaussian elimination** (row reduction) instead of a generic matrix inversion — numerically stable for small-to-medium problems.

- **Dataset**: `y = 3.5x + 7.2 + N(0, 2)`
- **Expected**: R² > 0.99

### 2. Linear Regression — SGD (`linear_regression.py:fit_sgd`)

Mini-batch gradient descent for scalability:

```
β ← β − α · ∇L(β)
∇L = (2/n) · Xᵀ(Xβ − y)
```

- Supports mini-batch size control
- Shuffles data each epoch to avoid cyclical gradients
- **Dataset**: `y = 2x₁ − 1.5x₂ + 0.5x₃ + 5 + N(0, 1)` (3 features)

### 3. Logistic Regression (`logistic_regression.py`)

Binary classification via sigmoid + cross-entropy:

```
P(y=1|x) = σ(βᵀx) = 1/(1 + e^{-βᵀx})
β ← β − α · (σ(βᵀx) − y) · x
```

- Full-batch SGD with per-sample updates
- **Dataset**: Two Gaussian blobs (separable)
- **Expected**: 100% accuracy, F1 = 1.0

### 4. Decision Tree — CART (`decision_tree.py`)

Recursive binary splitting:

| Task | Impurity measure | Leaf value |
|------|------------------|------------|
| Classification | Gini impurity `1 − Σ p²` | Majority class |
| Regression | MSE `(1/n) Σ(y − μ)²` | Mean of samples |

- Supports `max_depth` and `min_samples_split` for pruning
- **Classification dataset**: Two moons (non-linear boundary)
- **Regression dataset**: `y = 2x³ − 3x² + 4x + 1 + N(0, 5)`

## Running

```bash
cd predictive-model
python main.py
```

Menu:
- Options 1-5: Individual model demos with dataset description, learned parameters, and test metrics
- Option 6: Run all models sequentially for comparison

## Metrics Implemented (`metrics.py`)

| Metric | Formula | Used for |
|--------|---------|----------|
| RMSE | `√((1/n) Σ(y − ŷ)²)` | Regression |
| MAE | `(1/n) Σ | y − ŷ |` | Regression |
| R² | `1 − SS_res / SS_tot` | Regression |
| Accuracy | `correct / total` | Classification |
| Precision | `TP / (TP + FP)` | Classification |
| Recall | `TP / (TP + FN)` | Classification |
| F1 | `2 · P · R / (P + R)` | Classification |
| Confusion matrix | `M[i][j]` = count of class i predicted as j | Classification |

## Lessons

1. **OLS vs SGD** — OLS gives exact solution in one step; SGD scales to large data. On small data they should converge to the same coefficients.
2. **Logistic regression is a linear classifier** — it works perfectly on linearly separable data but cannot learn XOR or moons without feature engineering.
3. **Decision trees handle non-linearity** — the moons dataset requires splits to model the curved boundary. Gini impurity picks the split that maximises class purity.
4. **Standardization matters** — without scaling, gradient descent oscillates or diverges when features have different units.
5. **Overfitting is visible** — a deep tree (depth 10+) on 50 samples will memorise every point; pruning with `min_samples_split` forces generalisation.

## License

Educational use.
