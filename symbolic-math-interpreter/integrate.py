from ast_nodes import *
from simplify import simplify


def integrate(node, var="x"):
    if isinstance(node, Number):
        if node.value == 0:
            return Number(0)
        return Mul(node, Variable(var))
    elif isinstance(node, Variable):
        if node.name == var:
            return Div(Pow(Variable(var), Number(2)), Number(2))
        return Mul(node, Variable(var))
    elif isinstance(node, Add):
        return Add(integrate(node.left, var), integrate(node.right, var))
    elif isinstance(node, Sub):
        return Sub(integrate(node.left, var), integrate(node.right, var))
    elif isinstance(node, Mul):
        if isinstance(node.left, Number):
            return Mul(node.left, integrate(node.right, var))
        if isinstance(node.right, Number):
            return Mul(node.right, integrate(node.left, var))
        return integrate(expand_power_mul(node), var)
    elif isinstance(node, Pow):
        base = node.base
        exp = node.exp
        if isinstance(base, Variable) and base.name == var and isinstance(exp, Number):
            if exp.value == -1:
                return Log(Variable(var))
            return Div(Pow(Variable(var), Number(exp.value + 1)), Number(exp.value + 1))
        if isinstance(base, Variable) and base.name == var and isinstance(exp, Variable):
            return integrate(Exp(Mul(exp, Log(base))), var)
        return integrate(expand_power_mul(node), var)
    elif isinstance(node, Div):
        if isinstance(node.left, Number) and isinstance(node.right, Variable):
            return Mul(node.left, Log(Variable(var)))
        if isinstance(node.right, Variable) and isinstance(node.right, Variable):
            return Mul(node.left, Log(Variable(var)))
        if isinstance(node.left, Number) and node.left.value == 1 and isinstance(node.right, Variable):
            return Log(Variable(var))
        return integrate(expand_power_mul(node), var)
    elif isinstance(node, Sin):
        if isinstance(node.arg, Variable) and node.arg.name == var:
            return Neg(Cos(Variable(var)))
        return Mul(Neg(Cos(node.arg)), differentiate(node.arg, var, integrate_mode=True))
    elif isinstance(node, Cos):
        if isinstance(node.arg, Variable) and node.arg.name == var:
            return Sin(Variable(var))
        return integrate(node, var)
    elif isinstance(node, Exp):
        if isinstance(node.arg, Variable) and node.arg.name == var:
            return Exp(Variable(var))
        if isinstance(node.arg, Mul) and isinstance(node.arg.left, Number):
            return Div(Exp(node.arg), node.arg.left)
        return integrate(node, var)
    elif isinstance(node, Neg):
        return Neg(integrate(node.operand, var))
    else:
        raise TypeError(f"Cannot integrate node type: {type(node)}")


def expand_power_mul(node):
    return node


def integrate_expr(expr_text, var="x"):
    from parser import parse as parse_expr
    node = parse_expr(expr_text)
    result = integrate(node, var)
    return simplify(result)
