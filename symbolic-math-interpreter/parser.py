import re
from ast_nodes import *


TOKEN_SPEC = [
    ("NUMBER", r"\d+(\.\d+)?"),
    ("VARIABLE", r"[a-zA-Z_][a-zA-Z0-9_]*"),
    ("PLUS", r"\+"),
    ("MINUS", r"-"),
    ("TIMES", r"\*"),
    ("DIVIDE", r"/"),
    ("POW", r"\^"),
    ("LPAREN", r"\("),
    ("RPAREN", r"\)"),
    ("COMMA", r","),
    ("WS", r"\s+"),
]


def tokenize(text):
    tokens = []
    pos = 0
    while pos < len(text):
        match = None
        for tok_type, pattern in TOKEN_SPEC:
            m = re.match(pattern, text[pos:])
            if m:
                match = m
                if tok_type != "WS":
                    tokens.append((tok_type, m.group()))
                pos += m.end()
                break
        if not match:
            raise SyntaxError(f"Unexpected character {text[pos]!r} at position {pos}")
    return tokens


class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def peek(self):
        if self.pos < len(self.tokens):
            return self.tokens[self.pos]
        return ("EOF", "")

    def consume(self, expected=None):
        tok = self.peek()
        if expected is not None and tok[0] != expected:
            raise SyntaxError(f"Expected {expected}, got {tok[0]} ({tok[1]!r})")
        self.pos += 1
        return tok

    def parse(self):
        return self.parse_expression()

    def parse_expression(self):
        left = self.parse_term()
        while self.peek()[0] in ("PLUS", "MINUS"):
            op = self.consume()[1]
            right = self.parse_term()
            if op == "+":
                left = Add(left, right)
            else:
                left = Sub(left, right)
        return left

    def parse_term(self):
        left = self.parse_power()
        while self.peek()[0] in ("TIMES", "DIVIDE"):
            op = self.consume()[1]
            right = self.parse_power()
            if op == "*":
                left = Mul(left, right)
            else:
                left = Div(left, right)
        return left

    def parse_power(self):
        left = self.parse_unary()
        if self.peek()[0] == "POW":
            self.consume()
            right = self.parse_power()
            left = Pow(left, right)
        return left

    def parse_unary(self):
        if self.peek()[0] == "MINUS":
            self.consume()
            operand = self.parse_unary()
            return Neg(operand)
        return self.parse_primary()

    def parse_primary(self):
        tok = self.peek()
        if tok[0] == "NUMBER":
            self.consume()
            val = tok[1]
            return Number(float(val) if "." in val else int(val))
        elif tok[0] == "VARIABLE":
            self.consume()
            name = tok[1]
            if self.peek()[0] == "LPAREN":
                self.consume("LPAREN")
                arg = self.parse_expression()
                self.consume("RPAREN")
                if name == "sin":
                    return Sin(arg)
                elif name == "cos":
                    return Cos(arg)
                elif name == "log":
                    return Log(arg)
                elif name == "ln":
                    return Log(arg)
                elif name == "exp":
                    return Exp(arg)
                elif name == "sqrt":
                    return Pow(arg, Number(0.5))
                else:
                    raise SyntaxError(f"Unknown function: {name}")
            return Variable(name)
        elif tok[0] == "LPAREN":
            self.consume("LPAREN")
            expr = self.parse_expression()
            self.consume("RPAREN")
            return expr
        raise SyntaxError(f"Unexpected token: {tok}")


def parse(text):
    tokens = tokenize(text)
    parser = Parser(tokens)
    return parser.parse()
