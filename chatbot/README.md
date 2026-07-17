# Chatbot

> A conversational agent built from scratch in Python — no frameworks, no API calls, just pure Python and a few standard library modules. Combines a **rule‑based ELIZA‑style engine** with a **Naive Bayes intent classifier** to produce sensible, context‑aware responses.

## Architecture

```
chatbot/
├── main.py              # REPL entry point (type-out effect, main loop)
├── rule_engine.py       # ELIZA-inspired regex pattern matcher
├── classifier.py        # From-scratch Naive Bayes intent classifier
├── tokenizer.py         # Word tokenization, stop‑word removal, normalization
├── memory.py            # Conversation context and JSON persistence
├── entities.py          # Entity extraction (names, dates, times, numbers)
├── data/                # Persistent conversation state (JSON)
├── build/
└── README.md
```

### 1. Rule Engine (`rule_engine.py`)

Inspired by [ELIZA (Weizenbaum, 1966)](https://en.wikipedia.org/wiki/ELIZA) — the first chatbot.

- **Pattern‑response pairs** — each compiled regex maps to a list of possible responses (chosen randomly).
- **Callable responses** — some responses are `lambda` functions that interpolate matched groups (e.g., `My name is {name}`).
- **Fallback chain** — when no pattern matches, a random generic prompt is returned ("Tell me more about that").
- **Context injection** — the bot can prepend the user's remembered name to any response.

Patterns cover:
- Greetings & farewells
- Time / date queries
- Name introduction & recall
- Feeling / mood statements
- Jokes, thanks, apologies
- Affirmation / negation
- Likes & dislikes
- Help & capabilities
- Identity questions

### 2. Naive Bayes Classifier (`classifier.py`)

A multinomial Naive Bayes model trained on ~70 hand‑labeled examples across 16 intent classes.

**How it works:**
1. **Training**: Count word frequencies per intent using `P(word | intent) = (count + α) / (total + α·|V|)` (Laplace smoothing).
2. **Prediction**: Compute `log P(intent | text) ∝ log P(intent) + Σ log P(word | intent)` and pick the argmax.
3. **Confidence**: Exponentiate log‑probs to get a proper probability distribution.

| Intent | Example trigger |
|--------|----------------|
| `greeting` | "hello", "hi", "good morning" |
| `farewell` | "bye", "see you later" |
| `time` | "what time is it" |
| `date` | "what's the date" |
| `joke` | "tell me a joke" |
| `thanks` | "thank you" |
| `feeling` | "i feel sad" |
| `introduce` | "my name is bob" |

The classifier is used as a **secondary path** — when confidence exceeds 60% the bot uses an intent‑specific response; otherwise it falls back to the rule engine.

### 3. Tokenizer (`tokenizer.py`)

A minimal NLP pipeline built from scratch:
- **Tokenization**: `re.findall(r"[a-zA-Z]+(?:'[a-zA-Z]+)?", text)` — handles contractions like `don't`.
- **Stop‑word removal**: ~100 common English words filtered out to improve classifier signal.
- **Normalization**: Lower‑casing, punctuation stripping.

### 4. Conversation Memory (`memory.py`)

- Maintains a rolling window of the last 10 `(role, message)` turns.
- Tracks user name, mood, topic, and entity mentions.
- Persists `user_name` and `user_mood` to `data/conversation.json` across sessions.
- Detects first interaction (to prompt for name) and recurring topics.

### 5. Entity Extraction (`entities.py`)

Simple regex‑based extraction of:
- **Names** — patterns like "my name is X", "call me X", "I'm X"
- **Dates** — weekday names, `MM/DD/YYYY`, "today/tomorrow/yesterday"
- **Times** — `HH:MM AM/PM`
- **Numbers** — integer sequences

## Running

```bash
cd chatbot
python main.py
```

Type naturally. Try: greetings, name introduction, time/date queries, jokes, thanks, or just chat.

## Lessons

1. **Rule‑based vs. ML** — Rules give precise, predictable responses but don't scale. ML handles variety but needs data. The hybrid combines both.
2. **Naive Bayes is surprisingly effective** — with only 70 training examples, it can disambiguate common intents. Laplace smoothing (`α=0.5`) prevents zero‑probability blow‑up.
3. **Tokenization matters** — simple whitespace splitting breaks on punctuation. A regex tokenizer that preserves contractions (`don't` → `don't`, not `don` `t`) improves downstream accuracy.
4. **State makes a chatbot** — without memory, every turn is a blank slate. Remembering the user's name and referencing prior topics gives the illusion of understanding.

## License

Educational use.
