import math


class CountVectorizer:
    def __init__(self, max_features=None):
        self.max_features = max_features
        self.vocab = {}
        self.vocab_size = 0

    def fit(self, tokenized_docs):
        freq = {}
        for doc in tokenized_docs:
            seen = set()
            for token in doc:
                if token not in seen:
                    freq[token] = freq.get(token, 0) + 1
                    seen.add(token)
        sorted_vocab = sorted(freq.items(), key=lambda x: -x[1])
        if self.max_features:
            sorted_vocab = sorted_vocab[:self.max_features]
        self.vocab = {token: i for i, (token, _) in enumerate(sorted_vocab)}
        self.vocab_size = len(self.vocab)

    def transform(self, tokenized_docs):
        mat = []
        for doc in tokenized_docs:
            row = [0.0] * self.vocab_size
            for token in doc:
                if token in self.vocab:
                    row[self.vocab[token]] += 1.0
            mat.append(row)
        return mat

    def fit_transform(self, tokenized_docs):
        self.fit(tokenized_docs)
        return self.transform(tokenized_docs)


class TfidfVectorizer:
    def __init__(self, max_features=None):
        self.max_features = max_features
        self.vocab = {}
        self.vocab_size = 0
        self.idf = {}

    def fit(self, tokenized_docs):
        n_docs = len(tokenized_docs)
        freq = {}
        doc_freq = {}
        for i, doc in enumerate(tokenized_docs):
            seen = set()
            for token in doc:
                if token not in seen:
                    doc_freq[token] = doc_freq.get(token, 0) + 1
                    seen.add(token)
                freq[token] = freq.get(token, 0) + 1
        sorted_vocab = sorted(freq.items(), key=lambda x: -x[1])
        if self.max_features:
            sorted_vocab = sorted_vocab[:self.max_features]
        self.vocab = {token: i for i, (token, _) in enumerate(sorted_vocab)}
        self.vocab_size = len(self.vocab)
        self.idf = {}
        for token in self.vocab:
            df = doc_freq.get(token, 1)
            self.idf[token] = math.log((n_docs + 1) / (df + 1)) + 1.0

    def transform(self, tokenized_docs):
        mat = []
        for doc in tokenized_docs:
            tf = {}
            for token in doc:
                if token in self.vocab:
                    tf[token] = tf.get(token, 0) + 1.0
            row = [0.0] * self.vocab_size
            max_tf = max(tf.values()) if tf else 1.0
            for token, count in tf.items():
                if token in self.vocab:
                    tf_weight = count / max_tf
                    row[self.vocab[token]] = tf_weight * self.idf[token]
            mat.append(row)
        return mat

    def fit_transform(self, tokenized_docs):
        self.fit(tokenized_docs)
        return self.transform(tokenized_docs)
