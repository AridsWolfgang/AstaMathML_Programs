import re

STOP_WORDS = frozenset({
    "a","an","the","and","or","but","if","because","so","than","that","this","these","those",
    "of","in","on","at","to","for","with","by","about","as","into","through","during","before","after",
    "is","are","was","were","be","been","being","have","has","had","having",
    "do","does","did","doing","will","would","can","could","shall","should","may","might",
    "i","you","he","she","it","we","they","me","him","us","them",
    "my","your","his","her","its","our","their","myself","yourself","himself","herself",
    "no","nor","not","very","just","then","also","well","there","here",
    "when","where","why","how","all","each","every","some","any","both","few","more","most","other",
    "only","own","same","too","up","down","out","off","over","under","again","further",
    "what","which","who","whom","this","am","please","ok","okay","yes","hi","hello","hey","oh",
    "get","got","gotten","make","made","said","going","come","came","take","took","know","see","think",
})

STEM_SUFFIXES = [
    ("ies", "y"), ("ied", "y"), ("ying", "y"),
    ("ingly", ""), ("ingly", "ing"),
    ("ation", "ate"), ("ness", ""), ("ment", ""),
    ("able", ""), ("ible", ""),
    ("est", ""), ("er", ""), ("ed", ""), ("ing", ""), ("ly", ""), ("s", ""),
]


def tokenize(text):
    return re.findall(r"[a-zA-Z]+(?:'[a-zA-Z]+)?", text.lower())


def stem(word):
    if len(word) <= 3:
        return word
    for suffix, replacement in STEM_SUFFIXES:
        if word.endswith(suffix):
            base = word[: -len(suffix)] + replacement
            if len(base) >= 3:
                return base
    return word


def remove_stopwords(tokens):
    return [t for t in tokens if t not in STOP_WORDS]


def ngrams(tokens, n=2):
    return [" ".join(tokens[i:i+n]) for i in range(len(tokens)-n+1)]


def preprocess(text, use_stemming=True, use_ngrams=False):
    tokens = tokenize(text)
    tokens = remove_stopwords(tokens)
    if use_stemming:
        tokens = [stem(t) for t in tokens]
    result = list(tokens)
    if use_ngrams:
        for n in range(2, 4):
            result.extend(ngrams(tokens, n))
    return result
