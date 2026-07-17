import math
from ast_nodes import *


def evaluate(node, env=None):
    if env is None:
        env = {}

    if isinstance(node, Number):
        return node.value
    elif isinstance(node, Variable):
        if node.name in env:
            return env[node.name]
        if node.name == "e":
            return math.e
        if node.name == "pi":
            return math.pi
        raise NameError(f"Unknown variable: {node.name}")
    elif isinstance(node, Add):
        return evaluate(node.left, env) + evaluate(node.right, env)
    elif isinstance(node, Sub):
        return evaluate(node.left, env) - evaluate(node.right, env)
    elif isinstance(node, Mul):
        return evaluate(node.left, env) * evaluate(node.right, env)
    elif isinstance(node, Div):
        left = evaluate(node.left, env)
        right = evaluate(node.right, env)
        if right == 0:
            raise ZeroDivisionError("Division by zero")
        return left / right
    elif isinstance(node, Pow):
        base = evaluate(node.base, env)
        exp = evaluate(node.exp, env)
        return base ** exp
    elif isinstance(node, Neg):
        return -evaluate(node.operand, env)
    elif isinstance(node, Sin):
        return math.sin(evaluate(node.arg, env))
    elif isinstance(node, Cos):
        return math.cos(evaluate(node.arg, env))
    elif isinstance(node, Log):
        return math.log(evaluate(node.arg, env))
    elif isinstance(node, Exp):
        return math.exp(evaluate(node.arg, env))
    else:
        raise TypeError(f"Cannot evaluate node type: {type(node)}")
