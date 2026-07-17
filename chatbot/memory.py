import json
import os
from datetime import datetime

MEMORY_FILE = os.path.join(os.path.dirname(__file__), "data", "conversation.json")


class ConversationMemory:
    def __init__(self):
        self.turns = []
        self.user_name = None
        self.user_mood = None
        self.topic = None
        self.turn_count = 0
        self.mentioned_entities = {}
        self.last_intent = None
        self._load()

    def remember(self, role, message, intent=None, entities=None):
        self.turns.append({
            "role": role,
            "message": message,
            "intent": intent,
            "entities": entities or {},
            "timestamp": datetime.now().isoformat(),
        })
        self.turn_count += 1
        self.last_intent = intent
        if entities:
            for k, v in entities.items():
                self.mentioned_entities.setdefault(k, set()).add(v)

    def get_recent(self, n=3):
        return self.turns[-n:]

    def get_context_history(self):
        return [(t["role"], t["message"]) for t in self.turns[-10:]]

    def is_first_interaction(self):
        return self.turn_count <= 1

    def has_talked_about(self, topic):
        return any(topic in t["message"].lower() for t in self.turns)

    def _load(self):
        try:
            with open(MEMORY_FILE) as f:
                data = json.load(f)
                self.user_name = data.get("user_name")
                self.user_mood = data.get("user_mood")
        except (FileNotFoundError, json.JSONDecodeError):
            pass

    def save(self):
        try:
            os.makedirs(os.path.dirname(MEMORY_FILE), exist_ok=True)
            with open(MEMORY_FILE, "w") as f:
                json.dump({
                    "user_name": self.user_name,
                    "user_mood": self.user_mood,
                    "turn_count": self.turn_count,
                    "last_session": datetime.now().isoformat(),
                }, f, indent=2)
        except OSError:
            pass
