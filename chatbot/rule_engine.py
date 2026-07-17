import re
import random
from datetime import datetime


class RuleEngine:
    def __init__(self):
        self.rules = [
            (re.compile(r"(?:hi|hello|hey|howdy|greetings)\b", re.IGNORECASE),
             [f"Hello{'!' if random.random() < 0.3 else ''}" for _ in range(1)] + [
                 "Hi there!",
                 "Hey, how's it going?",
                 "Greetings! What's on your mind?",
             ]),

            (re.compile(r"(?:good\s*(?:morning|afternoon|evening|day)|gm|gn)\b", re.IGNORECASE),
             ["Good day to you too!", "And a good day to you!"]),

            (re.compile(r"(?:bye|goodbye|see\s*you|farewell|cya|later)\b", re.IGNORECASE),
             ["Goodbye! Come back anytime.",
              "See you later, alligator!",
              "Farewell, my friend.",
              "Take care!"]),

            (re.compile(r"(?:how\s*are\s*you|how(?:'s| is) it going|what['']s up|sup)\b", re.IGNORECASE),
             ["I'm doing great, thanks for asking!",
              "All systems operational. How about you?",
              "Feeling chatty! What can I do for you?",
              "I'm good! Always happy to talk."]),

            (re.compile(r"(?:what(?:'s| is) (?:your|ur) name|who are you|introduce yourself)\b", re.IGNORECASE),
             ["I'm AstaBot, your friendly conversational AI!",
              "My name is AstaBot. I'm here to chat and help.",
              "I'm AstaBot -- a chatbot built from scratch in Python."]),

            (re.compile(r"my name is (\w+)", re.IGNORECASE),
             [lambda m: f"Nice to meet you, {m.group(1)}!",
              lambda m: f"Hello {m.group(1)}, pleasure to meet you!"]),

            (re.compile(r"(?:i(?:'?m| am) )?(?:feeling|feel) (\w+)", re.IGNORECASE),
             [lambda m: f"You're feeling {m.group(1)}, huh? Tell me more about that.",
              lambda m: f"Why are you feeling {m.group(1)}?",
              lambda m: f"I hear you're feeling {m.group(1)}. That's interesting."]),

            (re.compile(r"(?:tell\s*(?:me\s*)?a\s*joke|joke|funny|make me laugh)\b", re.IGNORECASE),
             self._jokes()),

            (re.compile(r"(?:what(?:'s| is) (?:the )?time|what time is it|current time|time now)\b", re.IGNORECASE),
             [lambda _: f"The current time is {datetime.now():%H:%M}.",
              lambda _: f"It's {datetime.now():%I:%M %p} right now."]),

            (re.compile(r"(?:what(?:'s| is) (?:the )?date|today(?:'s)? date|what day)(?! of week)\b", re.IGNORECASE),
             [lambda _: f"Today is {datetime.now():%A, %B %d, %Y}.",
              lambda _: f"It's {datetime.now():%B %d, %Y}."]),

            (re.compile(r"(?:thank|thanks|ty|thx)\b", re.IGNORECASE),
             ["You're welcome!", "My pleasure.", "Anytime!", "Happy to help!"]),

            (re.compile(r"(?:sorry|my bad|apologize)\b", re.IGNORECASE),
             ["No worries at all.", "It's okay!", "Don't worry about it.",
              "No need to apologize."]),

            (re.compile(r"(?:help|what can you do|capabilities|features|commands)\b", re.IGNORECASE),
             ["I can chat with you, tell jokes, answer questions, and remember things about you! Try asking me about the time, date, or just have a conversation.",
              "I'm a conversational chatbot! You can greet me, ask questions, tell me how you feel, or ask for jokes. Just type naturally!"]),

            (re.compile(r"(?:how old are you|your age|when were you created)\b", re.IGNORECASE),
             ["I was born when you started this program! So I'm as old as this session.",
              "I'm timeless -- created fresh every time you run me."]),

            (re.compile(r"(?:where do you live|where are you from|your home)\b", re.IGNORECASE),
             ["I live right here in your computer! All I need is Python.",
              "My home is this terminal. I don't need much."]),

            (re.compile(r"(?:do you (?:have|eat|like|love|hate) (\w+))", re.IGNORECASE),
             [lambda m: f"I don't know much about {m.group(1)}, but I'd love to learn!",
              lambda m: f"{m.group(1)}? That's an interesting topic!"]),

            (re.compile(r"(?:yes|yeah|yep|sure|absolutely|correct)\b", re.IGNORECASE),
             ["Great!", "Awesome!", "Excellent!", "Good to hear!",
              "Wonderful! Tell me more."]),

            (re.compile(r"(?:no|nope|nah|not really|never)\b", re.IGNORECASE),
             ["Okay, no problem.", "Alright then!", "Fair enough.",
              "Got it. Let me know if you change your mind."]),

            (re.compile(r"(?:i (?:love|like|enjoy|adore) (\w+(?:\s\w+)?))", re.IGNORECASE),
             [lambda m: f"That's awesome! I'm glad you like {m.group(1)}.",
              lambda m: f"{m.group(1)} sounds cool! Tell me more about it."]),

            (re.compile(r"(?:i (?:hate|dislike|don't like|cannot stand) (\w+(?:\s\w+)?))", re.IGNORECASE),
             [lambda m: f"Oh, you don't like {m.group(1)}? Why not?",
               lambda m: f"That's too bad. What don't you like about {m.group(1)}?"]),

            (re.compile(r"(?:what is|what's|define|explain) (\w+(?:\s\w+)?)", re.IGNORECASE),
             [lambda m: f"I don't have a definition for '{m.group(1)}' yet. Want to tell me what it means?",
               lambda m: f"Good question! I wish I had a good answer about {m.group(1)}."]),
        ]

        self.fallbacks = [
            "That's interesting! Tell me more.",
            "I see. What else is on your mind?",
            "Hmm, I'm not sure what to say about that.",
            "Could you elaborate on that?",
            "Interesting! How does that make you feel?",
            "I'd love to hear more about that.",
            "Tell me something else!",
            "I'm still learning. Can you rephrase that?",
            "Got it. What else?",
        ]

        self.repeat_guards = {}

    @staticmethod
    def _jokes():
        return [
            "Why don't scientists trust atoms? Because they make up everything!",
            "What do you call a fake noodle? An impasta!",
            "Why did the scarecrow win an award? Because he was outstanding in his field!",
            "Why don't skeletons fight each other? They don't have the guts.",
            "What do you call a bear with no teeth? A gummy bear!",
            "Why did the math book look so sad? Because it had too many problems.",
            "What's orange and sounds like a parrot? A carrot!",
            "Why was the computer cold? It left its Windows open!",
        ]

    def respond(self, text, memory=None):
        for pattern, responses in self.rules:
            m = pattern.search(text)
            if m:
                response = random.choice(responses)
                if callable(response):
                    response = response(m)
                self.repeat_guards[pattern] = text
                return response

        if memory and memory.has_talked_about("joke") and random.random() < 0.3:
            return "Want to hear another joke? Just ask!"

        if memory and memory.turn_count % 5 == 0:
            return random.choice([
                "By the way, is there anything specific you'd like to talk about?",
                "I'm enjoying our conversation! What do you want to discuss?",
                "Do you have any questions for me?",
            ])

        return random.choice(self.fallbacks)

    def respond_with_context(self, text, memory):
        base = self.respond(text, memory)
        if memory and memory.user_name and random.random() < 0.2:
            prefix = random.choice([f"{memory.user_name}, ", f"Hey {memory.user_name}, ", ""])
            if prefix:
                base = prefix + base[0].lower() + base[1:]
        return base
