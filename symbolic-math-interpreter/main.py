import sys
import traceback
from parser import parse
from simplify import simplify
from differentiate import diff
from integrate import integrate_expr
from evaluate import evaluate
from ast_nodes import *


def to_string(node):
    if isinstance(node, Number):
        v = node.value
        if v == int(v):
            return str(int(v))
        return str(v)
    elif isinstance(node, Variable):
        return node.name
    elif isinstance(node, Neg):
        inner = to_string(node.operand)
        if isinstance(node.operand, (Add, Sub)):
            return f"-({inner})"
        return f"-{inner}"
    elif isinstance(node, Add):
        return f"{to_string(node.left)} + {to_string(node.right)}"
    elif isinstance(node, Sub):
        return f"{to_string(node.left)} - {to_string(node.right)}"
    elif isinstance(node, Mul):
        left = to_string(node.left)
        right = to_string(node.right)
        if isinstance(node.left, (Add, Sub)):
            left = f"({left})"
        if isinstance(node.right, (Add, Sub)):
            right = f"({right})"
        return f"{left}*{right}"
    elif isinstance(node, Div):
        return f"({to_string(node.left)})/({to_string(node.right)})"
    elif isinstance(node, Pow):
        base = to_string(node.base)
        if isinstance(node.base, (Add, Sub, Mul, Div)):
            base = f"({base})"
        return f"{base}^{to_string(node.exp)}"
    elif isinstance(node, Sin):
        return f"sin({to_string(node.arg)})"
    elif isinstance(node, Cos):
        return f"cos({to_string(node.arg)})"
    elif isinstance(node, Log):
        return f"log({to_string(node.arg)})"
    elif isinstance(node, Exp):
        return f"exp({to_string(node.arg)})"
    return repr(node)


def _split_expr_var(args):
    parts = args.split()
    if len(parts) >= 2 and len(parts[-1]) == 1 and parts[-1].isalpha():
        var = parts[-1]
        expr = " ".join(parts[:-1])
    else:
        var = "x"
        expr = args
    return expr, var


def cmd_diff(args):
    expr, var = _split_expr_var(args)
    try:
        result = diff(expr, var)
        print(f"d/d{var}({expr}) = {to_string(result)}")
    except Exception as e:
        print(f"Error: {e}")


def cmd_integrate(args):
    expr, var = _split_expr_var(args)
    try:
        result = integrate_expr(expr, var)
        print(f"Int({expr}) d{var} = {to_string(result)} + C")
    except Exception as e:
        print(f"Error: {e}")


def cmd_simplify(args):
    try:
        node = parse(args)
        result = simplify(node)
        print(f"simplify({args}) = {to_string(result)}")
    except Exception as e:
        print(f"Error: {e}")


def cmd_eval(args):
    try:
        parts = args.split()
        expr_parts = []
        env = {}
        for token in parts:
            if "=" in token:
                var_name, val_str = token.split("=", 1)
                env[var_name.strip()] = float(val_str)
            else:
                expr_parts.append(token)
        expr_text = " ".join(expr_parts)
        node = parse(expr_text)
        result = evaluate(node, env)
        env_str = ", ".join(f"{k}={v}" for k, v in env.items())
        print(f"eval({expr_text}) at {env_str} = {result}")
    except Exception as e:
        print(f"Error: {e}")


def cmd_expand(args):
    try:
        node = parse(args)
        result = simplify(node)
        print(f"expand({args}) = {to_string(result)}")
    except Exception as e:
        print(f"Error: {e}")


def print_help():
    print("Commands:")
    print("  diff <expr> [var]       - Differentiate expression with respect to var (default x)")
    print("  integrate <expr> [var]  - Integrate expression with respect to var (default x)")
    print("  simplify <expr>         - Simplify an algebraic expression")
    print("  eval <expr> var=val...  - Evaluate expression with variable assignments")
    print("  expand <expr>           - Expand/simplify an expression")
    print("  quit                    - Exit the REPL")
    print("Examples:")
    print("  diff x^3 + 2*x^2")
    print("  integrate x^2")
    print("  simplify x + x")
    print("  eval x^2 + 1 x=3")
    print("  expand (x + 1)^2")
    print()


COMMANDS = {
    "diff": cmd_diff,
    "integrate": cmd_integrate,
    "simplify": cmd_simplify,
    "eval": cmd_eval,
    "expand": cmd_expand,
    "help": lambda _: print_help(),
}


def repl():
    print("Symbolic Math Interpreter")
    print("Type 'help' for commands, 'quit' to exit.")
    print()
    while True:
        try:
            line = input(">>> ").strip()
            if not line:
                continue
            if line == "quit":
                break
            cmd = line.split()[0]
            args = line[len(cmd):].strip()
            if cmd in COMMANDS:
                COMMANDS[cmd](args)
            else:
                try:
                    node = parse(line)
                    result = simplify(node)
                    print(to_string(result))
                except Exception as e:
                    print(f"Error: {e}")
        except (EOFError, KeyboardInterrupt):
            print()
            break
        except Exception as e:
            print(f"Unexpected error: {e}")
            traceback.print_exc()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        expr = " ".join(sys.argv[1:])
        try:
            node = parse(expr)
            print(to_string(simplify(node)))
        except Exception as e:
            print(f"Error: {e}")
    else:
        repl()
