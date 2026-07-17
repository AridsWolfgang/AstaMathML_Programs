# Recommendation Engine

> Three recommendation algorithms implemented from scratch in Python — **user‑based collaborative filtering**, **item‑based collaborative filtering**, and **FunkSVD matrix factorization** — with a head‑to‑head comparison of accuracy and speed.

## Architecture

```
recommendation-engine/
├── main.py                  # Interactive REPL with comparisons
├── collaborative.py         # User-based and item-based CF
├── matrix_factorization.py  # FunkSVD (gradient descent)
├── similarity.py            # Cosine, Pearson, Jaccard
├── metrics.py               # RMSE, MAE, precision@k, recall@k
├── data/
│   └── ratings.py           # 8 users x 12 movies rating matrix
├── build/
└── README.md
```

## Dataset

A hand‑crafted 8‑user × 12‑movie rating matrix (64 known ratings, 32 held out as predictions).

| User | Sci‑Fi fan? | Drama fan? | Animation fan? |
|------|-------------|------------|----------------|
| Alice | Yes (Matrix 5, Interstellar 5) | Yes (Shawshank 5) | No (Frozen 2) |
| Bob | Yes (Inception 5, Matrix 4) | Yes (Godfather 5) | — |
| Charlie | Mixed | No | Yes (Toy Story 5, Nemo 5) |
| Diana | No | Yes (Pulp 5, Godfather 5) | No |
| Eve | Yes (Matrix 5, Inception 5) | Mixed | — |
| Frank | No | No | Yes (Toy Story 5, Frozen 5) |
| Grace | Yes | Mixed | Yes |
| Hank | No | Yes (Godfather 5, Shawshank 5) | — |

## Algorithms

### 1. User‑Based Collaborative Filtering

For each user **u** and unrated item **i**:

1. Find other users who rated **i** and are similar to **u**.
2. Compute similarity using **Pearson correlation** on items both users rated.
3. Weight neighbours' ratings by similarity, adjusting for each user's mean rating.

```
pred(u,i) = μ_u + [ Σ sim(u,v) · (r(v,i) − μ_v) ] / Σ |sim(u,v)|
```

**Strengths**: Simple, interpretable. **Weaknesses**: Doesn't scale to millions of users; cold‑start for new users.

### 2. Item‑Based Collaborative Filtering

Same idea, transposed: find items similar to **i** that **u** already rated.

```
pred(u,i) = [ Σ sim(i,j) · r(u,j) ] / Σ |sim(i,j)|
```

Uses **cosine similarity** between item rating vectors. Typically more stable than user‑based CF because items are simpler than people.

### 3. FunkSVD (Matrix Factorization)

Factor the rating matrix into two low‑rank matrices **P** (user‑factors) and **Q** (item‑factors):

```
R ≈ P · Q^T
```

Plus biases: `pred(u,i) = μ + b_u + b_i + P_u · Q_i`

Learned via stochastic gradient descent:

```
e = r(u,i) − pred(u,i)
b_u ← b_u + α(e − λ·b_u)
b_i ← b_i + α(e − λ·b_i)
P_u ← P_u + α(e·Q_i − λ·P_u)
Q_i ← Q_i + α(e·P_u − λ·Q_i)
```

**Used by Netflix Prize winner (2009).** Captures latent features like "sci‑fi affinity" or "prefers dramatic plots" automatically.

## Running

```bash
cd recommendation-engine
python main.py
```

Menu options:
1. View the rating matrix
2. User‑based CF prediction
3. Item‑based CF prediction
4. Run FunkSVD and view latent factors
5. Compare all three methods (RMSE, MAE, training time)
6. Top‑N recommendations for a user

## Results (on this dataset)

| Method | RMSE | MAE | Time |
|--------|------|-----|------|
| User‑based CF | ~1.2 | ~0.9 | <0.1s |
| Item‑based CF | ~1.1 | ~0.8 | <0.1s |
| FunkSVD (10 factors, 100 epochs) | ~1.4 | ~1.0 | ~2s |

FunkSVD slightly underperforms on this tiny dataset (overfitting risk) but scales linearly with data size — the CF methods do not.

## Lessons

1. **Similarity metric selection matters** — Pearson correlation normalizes user bias (some rate 3‑5, others 1‑4). Cosine on raw ratings conflates scale with preference.
2. **Sparsity kills CF** — if users rate only 5 items, finding commonality is hard. Matrix factorization generalizes better from sparse data.
3. **Bias terms capture a lot** — `b_u` alone (how generous a rater is) often explains 30%+ of variance.
4. **Regularisation prevents overfitting** — with only 64 datapoints, un‑regularized SVD can memorise the training set.

## License

Educational use.
