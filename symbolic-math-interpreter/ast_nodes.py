class Node:
    def __repr__(self):
        return f"{self.__class__.__name__}({', '.join(f'{k}={v!r}' for k, v in self.__dict__.items())})"

    def __eq__(self, other):
        if type(self) is not type(other):
            return False
        return self.__dict__ == other.__dict__

    def __hash__(self):
        return hash((type(self), *((k, v) if not isinstance(v, Node) else (k, v.__hash__()) for k, v in self.__dict__.items())))


class Number(Node):
    def __init__(self, value):
        self.value = value

    def __hash__(self):
        return hash(("Number", self.value))


class Variable(Node):
    def __init__(self, name):
        self.name = name

    def __hash__(self):
        return hash(("Variable", self.name))


class Add(Node):
    def __init__(self, left, right):
        self.left = left
        self.right = right


class Sub(Node):
    def __init__(self, left, right):
        self.left = left
        self.right = right


class Mul(Node):
    def __init__(self, left, right):
        self.left = left
        self.right = right


class Div(Node):
    def __init__(self, left, right):
        self.left = left
        self.right = right


class Pow(Node):
    def __init__(self, base, exp):
        self.base = base
        self.exp = exp


class Neg(Node):
    def __init__(self, operand):
        self.operand = operand


class Sin(Node):
    def __init__(self, arg):
        self.arg = arg


class Cos(Node):
    def __init__(self, arg):
        self.arg = arg


class Log(Node):
    def __init__(self, arg):
        self.arg = arg


class Exp(Node):
    def __init__(self, arg):
        self.arg = arg
