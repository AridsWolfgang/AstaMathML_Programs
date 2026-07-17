import re

NAME_PATTERN = re.compile(
    r"(?:my name(?:'s| is) |call me |i(?:')?m |name['']s )([A-Z][a-z]+)",
    re.IGNORECASE,
)
NUMBER_PATTERN = re.compile(r"\b(\d+(?:\.\d+)?)\b")

DATE_PATTERNS = [
    (re.compile(r"(today|tomorrow|yesterday)"), lambda m: m.group(1).lower()),
    (re.compile(r"(monday|tuesday|wednesday|thursday|friday|saturday|sunday)"), lambda m: m.group(1).lower()),
    (re.compile(r"(\d{1,2})[/-](\d{1,2})(?:[/-](\d{2,4}))?"), lambda m: f"{m.group(1)}/{m.group(2)}"),
]

TIME_PATTERN = re.compile(r"\b(\d{1,2})(?::(\d{2}))?\s*(am|pm)?\b", re.IGNORECASE)


def extract_entities(text):
    entities = {}

    m = NAME_PATTERN.search(text)
    if m:
        entities["name"] = m.group(1).strip()

    for pat, formatter in DATE_PATTERNS:
        m = pat.search(text)
        if m:
            entities["date"] = formatter(m)
            break

    m = TIME_PATTERN.search(text)
    if m:
        hour = int(m.group(1))
        minute = m.group(2) or "00"
        ampm = m.group(3)
        if ampm:
            entities["time"] = f"{hour}:{minute} {ampm}"
        else:
            entities["time"] = f"{hour}:{minute}"

    numbers = [int(n) for n in NUMBER_PATTERN.findall(text) if len(n) < 10]
    if numbers:
        entities["numbers"] = numbers

    return entities
