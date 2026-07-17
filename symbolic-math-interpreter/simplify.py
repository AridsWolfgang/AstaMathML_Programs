from ast_nodes import *


def simplify(node):
    if isinstance(node, Number):
        return node
    elif isinstance(node, Variable):
        return node
    elif isinstance(node, Neg):
        operand = simplify(node.operand)
        if isinstance(operand, Number):
            return Number(-operand.value)
        if isinstance(operand, Neg):
            return operand.operand
        return Neg(operand)
    elif isinstance(node, Add):
        left = simplify(node.left)
        right = simplify(node.right)
        if isinstance(left, Number) and isinstance(right, Number):
            return Number(left.value + right.value)
        if isinstance(left, Number) and left.value == 0:
            return right
        if isinstance(right, Number) and right.value == 0:
            return left
        return collect_like_terms(Add(left, right))
    elif isinstance(node, Sub):
        left = simplify(node.left)
        right = simplify(node.right)
        if isinstance(left, Number) and isinstance(right, Number):
            return Number(left.value - right.value)
        if isinstance(right, Number) and right.value == 0:
            return left
        if isinstance(left, Number) and left.value == 0:
            return Neg(right)
        return Sub(left, right)
    elif isinstance(node, Mul):
        left = simplify(node.left)
        right = simplify(node.right)
        if isinstance(left, Number) and isinstance(right, Number):
            return Number(left.value * right.value)
        if isinstance(left, Number):
            if left.value == 0:
                return Number(0)
            if left.value == 1:
                return right
        if isinstance(right, Number):
            if right.value == 0:
                return Number(0)
            if right.value == 1:
                return left
        merged = merge_mul_constants(left, right)
        if merged is not None:
            return merged
        return collect_like_terms(Mul(left, right))
    elif isinstance(node, Div):
        left = simplify(node.left)
        right = simplify(node.right)
        if isinstance(left, Number) and isinstance(right, Number):
            if right.value == 0:
                raise ZeroDivisionError("Division by zero in simplify")
            return Number(left.value / right.value)
        if isinstance(right, Number):
            if right.value == 1:
                return left
            if right.value == 0:
                raise ZeroDivisionError("Division by zero in simplify")
        return Div(left, right)
    elif isinstance(node, Pow):
        base = simplify(node.base)
        exp = simplify(node.exp)
        if isinstance(base, Number) and isinstance(exp, Number):
            return Number(base.value ** exp.value)
        if isinstance(exp, Number):
            if exp.value == 0:
                return Number(1)
            if exp.value == 1:
                return base
        if isinstance(base, Number) and base.value == 0:
            return Number(0)
        if isinstance(base, Number) and base.value == 1:
            return Number(1)
        return Pow(base, exp)
    elif isinstance(node, Sin):
        return Sin(simplify(node.arg))
    elif isinstance(node, Cos):
        return Cos(simplify(node.arg))
    elif isinstance(node, Log):
        return Log(simplify(node.arg))
    elif isinstance(node, Exp):
        return Exp(simplify(node.arg))
    return node


def merge_mul_constants(left, right):
    left_const = _get_constant(left)
    right_const = _get_constant(right)
    if left_const is not None and right_const is not None:
        c = left_const * right_const
        rest = _remove_constant(left) or _remove_constant(right)
        if rest is None:
            return Number(c)
        if c == 0:
            return Number(0)
        if c == 1:
            return rest
        return Mul(Number(c), rest)
    return None


def _get_constant(node):
    if isinstance(node, Number):
        return node.value
    if isinstance(node, Mul) and isinstance(node.left, Number):
        return node.left.value
    return None


def _remove_constant(node):
    if isinstance(node, Number):
        return None
    if isinstance(node, Mul) and isinstance(node.left, Number):
        return node.right
    return node


def collect_like_terms(node):
    if isinstance(node, Add):
        left = collect_like_terms(node.left)
        right = collect_like_terms(node.right)
        terms = extract_terms(left) + extract_terms(right)
        return combine_terms(terms)
    return node


def extract_terms(node):
    if isinstance(node, Add):
        return extract_terms(node.left) + extract_terms(node.right)
    return [node]


def split_term(node):
    if isinstance(node, Number):
        return node.value, None
    elif isinstance(node, Mul):
        if isinstance(node.left, Number):
            return node.left.value, node.right
        if isinstance(node.right, Number):
            return node.right.value, node.left
        return 1, node
    else:
        return 1, node


def term_key(rest):
    if rest is None:
        return None
    if isinstance(rest, Variable):
        return ("var", rest.name)
    if isinstance(rest, Pow):
        return ("pow", term_key(rest.base), term_key(rest.exp))
    if isinstance(rest, Sin):
        return ("sin", term_key(rest.arg))
    if isinstance(rest, Cos):
        return ("cos", term_key(rest.arg))
    if isinstance(rest, Log):
        return ("log", term_key(rest.arg))
    if isinstance(rest, Exp):
        return ("exp", term_key(rest.arg))
    if isinstance(rest, Neg):
        return ("neg", term_key(rest.operand))
    return ("other", repr(rest))


def combine_terms(terms):
    coeff_map = {}
    rest_map = {}
    for term in terms:
        coeff, rest = split_term(term)
        k = term_key(rest)
        coeff_map[k] = coeff_map.get(k, 0) + coeff
        if k not in rest_map:
            rest_map[k] = rest

    result = None
    for k, coeff in coeff_map.items():
        if coeff == 0:
            continue
        rest = rest_map[k]
        if rest is None:
            term = Number(coeff)
        elif coeff == 1:
            term = rest
        else:
            term = Mul(Number(coeff), rest)
        if result is None:
            result = term
        else:
            result = Add(result, term)

    return result if result is not None else Number(0)
