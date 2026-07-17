import re

STOP_WORDS = frozenset({
    "a","an","the","is","are","was","were","be","been","being",
    "i","you","he","she","it","we","they","my","your","his","her",
    "its","our","their","me","him","us","them",
    "and","or","but","if","because","so","than","that","this",
    "these","those","of","in","on","at","to","for","with","by",
    "about","as","into","through","during","before","after",
    "do","does","did","doing","have","has","had","having",
    "will","would","can","could","shall","should","may","might",
    "no","nor","not","very","just","then","also","well","there",
    "here","when","where","why","how","all","each","every",
    "some","any","both","few","more","most","other","such",
    "only","own","same","too","up","down","out","off","over",
    "what","which","who","whom","am","please","ok","okay",
    "yes","no","hi","hello","hey","oh","ah","well","now",
})

def tokenize(text):
    return re.findall(r"[a-zA-Z]+(?:\'[a-zA-Z]+)?", text.lower())

def remove_stopwords(tokens):
    return [t for t in tokens if t not in STOP_WORDS]

def normalize(text):
    text = text.lower().strip()
    text = re.sub(r"[^a-zA-Z0-9\s'?.!,]", "", text)
    return text
