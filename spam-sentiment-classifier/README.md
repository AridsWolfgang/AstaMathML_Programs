# Spam & Sentiment Classifier

> Two pipelines built from scratch in Python — a **spam/ham detector** and a **positive/negative sentiment analyzer** — using Multinomial Naive Bayes with TF-IDF vectorization. No scikit-learn, no NLTK, just pure Python.

## Architecture

```
spam-sentiment-classifier/
├── main.py              # Interactive REPL (evaluate + classify custom text)
├── naive_bayes.py       # Multinomial Naive Bayes (Laplace smoothing)
├── vectorizer.py        # CountVectorizer + TF-IDF from scratch
├── tokenizer.py         # Tokenizer, stemmer, stop words, n-grams
├── metrics.py           # Accuracy, confusion matrix, precision/recall/F1
├── data/
│   ├── spam.py          # 50 spam/ham SMS-style messages
│   └── sentiment.py     # 100 positive/negative movie reviews
├── build/
└── README.md
```

## Pipeline

```
Raw text → Tokenizer → Stemmer → Stop-word removal → N-grams (sentiment only)
    → TF-IDF vectorization → Multinomial Naive Bayes → Prediction
```

### 1. Tokenizer (`tokenizer.py`)

- **Tokenization**: regex `[a-zA-Z]+(?:'[a-zA-Z]+)?` — handles contractions like `don't`.
- **Stemming**: suffix-stripping rules (`-ing`, `-ed`, `-ly`, `-ies`, `-ation`, etc.) — a simple rule-based stemmer inspired by the Porter algorithm.
- **Stop words**: ~100 common English words filtered out.
- **N-grams**: up to 3-grams for sentiment (captures phrases like `not good`).

### 2. TF-IDF Vectorizer (`vectorizer.py`)

Implemented in two stages:

**CountVectorizer**: builds a vocabulary from document frequency, converts each document to a frequency vector.

**TfidfVectorizer**: extends CountVectorizer with TF-IDF weighting:

```
TF(t,d) = raw count / max count in d
IDF(t) = log((N + 1) / (df(t) + 1)) + 1
TF-IDF = TF × IDF
```

- Smooth IDF prevents division by zero for out-of-vocabulary terms.
- `max_features` limits vocabulary to the top-N most frequent terms.

### 3. Multinomial Naive Bayes (`naive_bayes.py`)

The "Bayes" part:

```
P(class | doc) ∝ P(class) × Π P(token | class)^count(token, doc)
```

With Laplace (add-α) smoothing:

```
P(token | class) = (count(token, class) + α) / (total(class) + α × |V|)
```

- `predict_log_proba` computes log-probabilities to avoid floating-point underflow.
- `predict_proba` exponentiates and normalises to a true probability distribution.

### 4. Metrics (`metrics.py`)

| Metric | Formula |
|--------|---------|
| Accuracy | `correct / total` |
| Precision | `TP / (TP + FP)` |
| Recall | `TP / (TP + FN)` |
| F1 | `2 · P · R / (P + R)` |
| Confusion matrix | `M[i][j]` = count of class i predicted as j |

## Datasets

### Spam/Ham (50 messages)
- 25 spam: lottery scams, phishing, loans, free offers
- 25 ham: casual conversation, reminders, plans
- Token overlap is low, so the classifier achieves near-perfect separation.

### Sentiment (100 movie reviews)
- 50 positive: "brilliant", "masterpiece", "fantastic", "loved it"
- 50 negative: "terrible", "boring", "waste of time", "awful"
- Uses n-grams to capture phrases like `not funny` or `worst ever`.

## Running

```bash
cd spam-sentiment-classifier
python main.py
```

Menu:
- Option 1: Evaluate spam classifier with metrics + top spam indicators
- Option 2: Evaluate sentiment classifier with metrics + top positive/negative indicators
- Option 3: Classify custom text through both pipelines simultaneously

## Results

### Spam Classifier
```
Accuracy:  1.0000
Confusion matrix:
                   ham       spam
  ham              25          0
  spam              0         25
  ham       : prec=1.000  rec=1.000  f1=1.000
  spam      : prec=1.000  rec=1.000  f1=1.000
```

Top spam indicators: `free`, `click`, `claim`, `now`, `win`, `urgent`, `congratul`, `offer`, `money`, `apply`.

### Sentiment Classifier
```
Accuracy:  1.0000
Confusion matrix:
                   neg        pos
  neg              50          0
  pos               0         50
  neg       : prec=1.000  rec=1.000  f1=1.000
  pos       : prec=1.000  rec=1.000  f1=1.000
```

Top positive indicators: `brilliant`, `masterpiece`, `fantastic`, `loved`, `beautiful`, `amazing`, `incredible`, `perfect`, `heartwarming`, `stunning`.

Top negative indicators: `terrible`, `boring`, `awful`, `waste`, `worst`, `disappointing`, `predictable`, `embarrassing`, `atrocious`, `cringe`.

## Lessons

1. **Naive Bayes is naive but effective** — the "naive" independence assumption is clearly wrong (words in a sentence are not independent), yet the classifier works surprisingly well for text classification.
2. **TF-IDF beats raw counts** — common words like "movie", "film", "the" are downweighted by IDF, letting discriminative words dominate.
3. **Stemming reduces sparsity** — `running`, `runner`, `ran` all map to the same stem, making the classifier more robust.
4. **N-grams capture negation** — without n-grams, `not good` could be misclassified as positive because `good` is a strong positive indicator. With bigrams, `not good` becomes a separate feature.
5. **Spam is an arms race** — a simple bag-of-words classifier catches obvious spam but would miss cleverly crafted messages. Modern spam filters use many more signals (sender reputation, link analysis, etc.).

## License

Educational use.
