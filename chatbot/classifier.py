import math
import random
from collections import Counter, defaultdict
from tokenizer import tokenize, remove_stopwords


class NaiveBayesClassifier:
    def __init__(self, alpha=1.0):
        self.alpha = alpha
        self.class_counts = Counter()
        self.feature_counts = defaultdict(lambda: Counter())
        self.vocab = set()
        self.total_docs = 0

    def train(self, texts, labels):
        for text, label in zip(texts, labels):
            tokens = remove_stopwords(tokenize(text))
            self.class_counts[label] += 1
            self.total_docs += 1
            for token in set(tokens):
                self.feature_counts[label][token] += 1
                self.vocab.add(token)

    def predict(self, text):
        tokens = remove_stopwords(tokenize(text))
        scores = {}
        for label in self.class_counts:
            prior = self.class_counts[label] / self.total_docs
            log_prob = math.log(prior)
            for token in tokens:
                count = self.feature_counts[label].get(token, 0)
                prob = (count + self.alpha) / (
                    sum(self.feature_counts[label].values())
                    + self.alpha * len(self.vocab)
                )
                log_prob += math.log(prob)
            scores[label] = log_prob
        return max(scores, key=scores.get)

    def predict_proba(self, text):
        tokens = remove_stopwords(tokenize(text))
        scores = {}
        for label in self.class_counts:
            prior = self.class_counts[label] / self.total_docs
            log_prob = math.log(prior)
            for token in tokens:
                count = self.feature_counts[label].get(token, 0)
                prob = (count + self.alpha) / (
                    sum(self.feature_counts[label].values())
                    + self.alpha * len(self.vocab)
                )
                log_prob += math.log(prob)
            scores[label] = log_prob
        total = sum(math.exp(s) for s in scores.values())
        return {l: math.exp(s) / total for l, s in scores.items()}


TRAIN_TEXTS = [
    "hello", "hi", "hey", "good morning", "good evening", "howdy", "greetings",
    "what's up", "yo", "hi there",
    "bye", "goodbye", "see you", "see you later", "farewell", "later", "cya",
    "gotta go", "talk to you later", "peace out",
    "how are you", "how's it going", "how do you do", "what's up",
    "how are you doing", "you good",
    "i'm fine", "i'm good", "i'm great", "doing well", "feeling good",
    "i'm sad", "i'm happy", "i feel tired", "i'm feeling great",
    "thank you", "thanks", "thanks a lot", "thank you so much", "thx",
    "thanks for your help",
    "sorry", "i'm sorry", "my apologies", "apologies", "my bad",
    "tell me a joke", "joke", "make me laugh", "say something funny",
    "tell me another joke", "funny",
    "what time is it", "time", "current time", "tell me the time",
    "what's the date", "date", "today's date", "what day is it",
    "what's your name", "who are you", "your name", "tell me about yourself",
    "what can you do", "help", "commands", "capabilities", "what do you do",
    "yes", "yeah", "yep", "sure", "correct", "that's right",
    "no", "nope", "nah", "not really", "no way",
    "i love python", "i like programming", "i enjoy reading",
    "i hate traffic", "i don't like bugs", "i dislike spam",
    "my name is alice", "call me bob", "i'm charlie",
    "what is machine learning", "define recursion", "explain gravity",
    "i don't know", "not sure", "maybe", "i guess",
    "good morning", "good night", "have a good day",
]

TRAIN_LABELS = [
    "greeting", "greeting", "greeting", "greeting", "greeting", "greeting", "greeting",
    "greeting", "greeting", "greeting",
    "farewell", "farewell", "farewell", "farewell", "farewell", "farewell",
    "farewell", "farewell", "farewell",
    "how_are_you", "how_are_you", "how_are_you", "how_are_you",
    "how_are_you", "how_are_you",
    "feeling", "feeling", "feeling", "feeling", "feeling",
    "feeling", "feeling", "feeling", "feeling",
    "thanks", "thanks", "thanks", "thanks", "thanks",
    "thanks",
    "apology", "apology", "apology", "apology", "apology",
    "joke", "joke", "joke", "joke",
    "joke", "joke",
    "time", "time", "time", "time",
    "date", "date", "date", "date",
    "name_query", "name_query", "name_query", "name_query",
    "help", "help", "help", "help", "help", "help",
    "affirm", "affirm", "affirm", "affirm", "affirm",
    "deny", "deny", "deny", "deny", "deny",
    "like", "like", "like",
    "dislike", "dislike", "dislike",
    "introduce", "introduce", "introduce",
    "definition", "definition", "definition",
    "uncertain", "uncertain", "uncertain", "uncertain",
    "greeting", "farewell", "farewell",
]


_intent_map = {
    "greeting": ["Hello!", "Hi there!", "Hey!", "Greetings!"],
    "farewell": ["Goodbye!", "See you later!", "Take care!"],
    "how_are_you": ["I'm doing great!", "All good here!", "Feeling chatty!"],
    "feeling": ["Tell me more about that.", "Why do you feel that way?"],
    "thanks": ["You're welcome!", "Anytime!", "My pleasure!"],
    "apology": ["No worries!", "It's okay!", "Don't worry about it."],
    "joke": ["Why don't scientists trust atoms? Because they make up everything!"],
    "time": [f"The current time is ..."],
    "date": [f"Today is ..."],
    "name_query": ["I'm AstaBot!", "My name is AstaBot."],
    "help": ["I can chat, tell jokes, answer questions!"],
    "affirm": ["Great!", "Awesome!", "Excellent!"],
    "deny": ["Okay, no problem.", "Alright then!"],
    "like": ["That's awesome!", "Glad you like that!"],
    "dislike": ["Oh, why don't you like that?", "That's too bad."],
    "introduce": ["Nice to meet you!", "Pleased to meet you!"],
    "definition": ["That's a great question! I'm still learning though."],
    "uncertain": ["Take your time!", "No rush at all."],
}

_intent_mood_map = {
    "greeting": "friendly",
    "farewell": "neutral",
    "how_are_you": "friendly",
    "feeling": "reflective",
    "thanks": "grateful",
    "apology": "forgiving",
    "joke": "playful",
    "affirm": "positive",
    "deny": "neutral",
    "like": "excited",
    "dislike": "sympathetic",
}


def build_classifier():
    clf = NaiveBayesClassifier(alpha=0.5)
    clf.train(TRAIN_TEXTS, TRAIN_LABELS)
    return clf


def get_response_for_intent(intent, conf=None):
    responses = _intent_map.get(intent, ["Interesting! Tell me more."])
    import random
    return random.choice(responses)


def get_mood_for_intent(intent):
    return _intent_mood_map.get(intent, "curious")
