import sys
import os

sys.path.insert(0, os.path.dirname(__file__))

from tokenizer import preprocess
from vectorizer import TfidfVectorizer
from naive_bayes import MultinomialNB
from data.spam import SPAM_HAM_DATA
from data.sentiment import SENTIMENT_DATA
from metrics import report

HEADER = """
  +--------------------------------------------+
  |     SPAM & SENTIMENT CLASSIFIER v1.0       |
  |  Naive Bayes + TF-IDF from scratch         |
  +--------------------------------------------+
"""


def train_spam_classifier():
    texts, labels = zip(*SPAM_HAM_DATA)
    tokens = [preprocess(t, use_stemming=True, use_ngrams=False) for t in texts]
    vec = TfidfVectorizer(max_features=500)
    X = vec.fit_transform(tokens)
    clf = MultinomialNB(alpha=0.5)
    clf.fit(X, list(labels))
    return vec, clf


def train_sentiment_classifier():
    texts, labels = zip(*SENTIMENT_DATA)
    tokens = [preprocess(t, use_stemming=True, use_ngrams=True) for t in texts]
    vec = TfidfVectorizer(max_features=1000)
    X = vec.fit_transform(tokens)
    clf = MultinomialNB(alpha=0.5)
    clf.fit(X, list(labels))
    return vec, clf


def evaluate_spam():
    print("\n  --- Spam Classifier Evaluation ---")
    print(f"  Dataset: {len(SPAM_HAM_DATA)} messages")
    texts, labels = zip(*SPAM_HAM_DATA)
    tokens = [preprocess(t, use_stemming=True) for t in texts]
    vec = TfidfVectorizer(max_features=500)
    X = vec.fit_transform(tokens)
    clf = MultinomialNB(alpha=0.5)
    clf.fit(X, list(labels))
    preds = clf.predict(X)
    report(list(labels), preds, labels=["ham", "spam"])

    print(f"\n  Top spam indicators:")
    spam_idx = 1
    features = sorted(vec.vocab, key=lambda t: clf.feature_log_prob["spam"][vec.vocab[t]] -
                      clf.feature_log_prob["ham"][vec.vocab[t]], reverse=True)[:10]
    for i, feat in enumerate(features):
        spam_prob = math.exp(clf.feature_log_prob["spam"][vec.vocab[feat]])
        ham_prob = math.exp(clf.feature_log_prob["ham"][vec.vocab[feat]])
        print(f"    {i+1}. '{feat}' (spam={spam_prob:.4f}, ham={ham_prob:.4f})")


def evaluate_sentiment():
    print("\n  --- Sentiment Classifier Evaluation ---")
    print(f"  Dataset: {len(SENTIMENT_DATA)} reviews")
    texts, labels = zip(*SENTIMENT_DATA)
    tokens = [preprocess(t, use_stemming=True, use_ngrams=True) for t in texts]
    vec = TfidfVectorizer(max_features=1000)
    X = vec.fit_transform(tokens)
    clf = MultinomialNB(alpha=0.5)
    clf.fit(X, list(labels))
    preds = clf.predict(X)
    report(list(labels), preds, labels=["neg", "pos"])

    print(f"\n  Top positive indicators:")
    features = sorted(vec.vocab, key=lambda t: clf.feature_log_prob["pos"][vec.vocab[t]] -
                      clf.feature_log_prob["neg"][vec.vocab[t]], reverse=True)[:10]
    for i, feat in enumerate(features):
        pos_prob = math.exp(clf.feature_log_prob["pos"][vec.vocab[feat]])
        neg_prob = math.exp(clf.feature_log_prob["neg"][vec.vocab[feat]])
        print(f"    {i+1}. '{feat}' (pos={pos_prob:.4f}, neg={neg_prob:.4f})")


def classify_text(text, vec, clf, label_a, label_b):
    tokens = preprocess(text, use_stemming=True)
    X = vec.transform([tokens])
    proba = clf.predict_proba(X)[0]
    pred = label_a if proba.get(label_a, 0) > proba.get(label_b, 0) else label_b
    conf = max(proba.values())
    return pred, conf, proba


import math


def main():
    print(HEADER)

    print("  Training classifiers...")
    spam_vec, spam_clf = train_spam_classifier()
    sent_vec, sent_clf = train_sentiment_classifier()
    print("  Done.\n")

    while True:
        print("\n  --- Spam & Sentiment Classifier ---")
        print("  1) Evaluate spam classifier (on training set)")
        print("  2) Evaluate sentiment classifier (on training set)")
        print("  3) Classify a custom message (spam + sentiment)")
        print("  Q) Quit")
        print("  Choice: ", end="")
        choice = input().strip().lower()

        if choice == "1":
            evaluate_spam()
        elif choice == "2":
            evaluate_sentiment()
        elif choice == "3":
            print()
            text = input("  Enter text: ").strip()
            if not text:
                print("  Empty input.")
                continue

            spam_pred, spam_conf, spam_proba = classify_text(
                text, spam_vec, spam_clf, "ham", "spam")
            sent_pred, sent_conf, sent_proba = classify_text(
                text, sent_vec, sent_clf, "neg", "pos")

            print(f"\n  Results for: \"{text[:60]}{'...' if len(text) > 60 else ''}\"")
            print(f"  {'-' * 50}")
            print(f"  Spam classifier:     {spam_pred.upper():>4s} (confidence: {spam_conf:.3f})")
            print(f"    P(ham)  = {spam_proba.get('ham', 0):.4f}")
            print(f"    P(spam) = {spam_proba.get('spam', 0):.4f}")
            print(f"  Sentiment:           {'POSITIVE' if sent_pred == 'pos' else 'NEGATIVE'} (confidence: {sent_conf:.3f})")
            print(f"    P(neg)  = {sent_proba.get('neg', 0):.4f}")
            print(f"    P(pos)  = {sent_proba.get('pos', 0):.4f}")
        elif choice == "q":
            print("\n  Goodbye!")
            break
        else:
            print("  Invalid choice.")


if __name__ == "__main__":
    main()
