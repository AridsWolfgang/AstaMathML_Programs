from ast_nodes import *
from simplify import simplify


def differentiate(node, var="x"):
    if isinstance(node, Number):
        return Number(0)
    elif isinstance(node, Variable):
        if node.name == var:
            return Number(1)
        return Number(0)
    elif isinstance(node, Add):
        return Add(differentiate(node.left, var), differentiate(node.right, var))
    elif isinstance(node, Sub):
        return Sub(differentiate(node.left, var), differentiate(node.right, var))
    elif isinstance(node, Mul):
        left = node.left
        right = node.right
        dleft = differentiate(left, var)
        dright = differentiate(right, var)
        return Add(Mul(dleft, right), Mul(left, dright))
    elif isinstance(node, Div):
        left = node.left
        right = node.right
        dleft = differentiate(left, var)
        dright = differentiate(right, var)
        numerator = Sub(Mul(dleft, right), Mul(left, dright))
        denominator = Pow(right, Number(2))
        return Div(numerator, denominator)
    elif isinstance(node, Pow):
        base = node.base
        exp = node.exp
        if isinstance(exp, Number):
            if isinstance(base, Variable):
                return Mul(exp, Pow(base, Number(exp.value - 1)))
            else:
                inner = differentiate(base, var)
                if isinstance(inner, Number) and inner.value == 0:
                    return Number(0)
                return Mul(exp, Mul(Pow(base, Number(exp.value - 1)), inner))
        if isinstance(base, Variable) and base.name == var:
            return Mul(exp, Mul(Pow(Variable(var), Sub(exp, Number(1))), differentiate(exp, var)))
        return differentiate(Exp(Mul(exp, Log(base))), var)
    elif isinstance(node, Neg):
        return Neg(differentiate(node.operand, var))
    elif isinstance(node, Sin):
        return Mul(Cos(node.arg), differentiate(node.arg, var))
    elif isinstance(node, Cos):
        return Neg(Mul(Sin(node.arg), differentiate(node.arg, var)))
    elif isinstance(node, Log):
        return Div(differentiate(node.arg, var), node.arg)
    elif isinstance(node, Exp):
        return Mul(Exp(node.arg), differentiate(node.arg, var))
    else:
        raise TypeError(f"Cannot differentiate node type: {type(node)}")


def diff(expr_text, var="x"):
    from parser import parse as parse_expr
    node = parse_expr(expr_text)
    result = differentiate(node, var)
    return simplify(result)
