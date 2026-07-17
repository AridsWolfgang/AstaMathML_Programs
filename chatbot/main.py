import sys
import time
import random

sys.path.insert(0, ".")

from tokenizer import normalize
from rule_engine import RuleEngine
from classifier import build_classifier, get_response_for_intent, get_mood_for_intent
from memory import ConversationMemory
from entities import extract_entities

BANNER = """
  +----------------------------------------+
  |        >>  AstaBot  v1.0  <<           |
  |  A Chatbot Built From Scratch          |
  |  Type 'quit' or 'exit' to stop.        |
  +----------------------------------------+
"""


def type_out(text, delay=0.015):
    for char in text:
        print(char, end="", flush=True)
        time.sleep(delay if char not in " \n" else 0.001)
    print()


def main():
    print(BANNER)

    engine = RuleEngine()
    classifier = build_classifier()
    memory = ConversationMemory()

    if memory.user_name:
        print(f"  (Welcome back, {memory.user_name}!)")

    while True:
        try:
            user_input = input("\n  You: ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\n  Goodbye!")
            break

        if not user_input:
            continue

        lowered = user_input.lower().strip()
        if lowered in ("quit", "exit", "q"):
            type_out("  AstaBot: Goodbye! Come back anytime.")
            memory.save()
            break

        entities = extract_entities(user_input)

        if "name" in entities and not memory.user_name:
            memory.user_name = entities["name"]
            memory.save()

        intent = classifier.predict(user_input)
        conf_scores = classifier.predict_proba(user_input)
        conf = conf_scores.get(intent, 0)
        mood = get_mood_for_intent(intent)
        memory.user_mood = mood

        memory.remember("user", user_input, intent, entities)

        if conf > 0.6:
            response = get_response_for_intent(intent, conf)
            if intent == "time":
                from datetime import datetime
                response = f"The current time is {datetime.now():%H:%M}."
            elif intent == "date":
                from datetime import datetime
                response = f"Today is {datetime.now():%A, %B %d, %Y}."
            elif intent == "introduce" and memory.user_name:
                response = f"Nice to meet you, {memory.user_name}!"
            elif intent == "joke":
                jokes = engine._jokes()
                response = random.choice(jokes)
            response = engine.respond_with_context(response, memory)
        else:
            response = engine.respond_with_context(user_input, memory)

        memory.remember("bot", response)

        if memory.is_first_interaction() and not memory.user_name:
            probe = random.choice([
                " By the way, what's your name?",
                " I don't think I caught your name?",
                " What should I call you?",
            ])
            response += probe

        type_out("  AstaBot: " + response)

    print()


if __name__ == "__main__":
    main()
