"""Common mathematical functions for the symbolic math interpreter.

This module defines metadata about supported functions for reference.
"""

FUNCTIONS = {
    "sin": {
        "description": "Sine function",
        "domain": "All real numbers",
        "range": "[-1, 1]",
        "derivative": "cos(x)",
        "integral": "-cos(x) + C",
    },
    "cos": {
        "description": "Cosine function",
        "domain": "All real numbers",
        "range": "[-1, 1]",
        "derivative": "-sin(x)",
        "integral": "sin(x) + C",
    },
    "log": {
        "description": "Natural logarithm",
        "domain": "x > 0",
        "range": "All real numbers",
        "derivative": "1/x",
        "integral": "x*log(x) - x + C",
    },
    "ln": {
        "description": "Natural logarithm (alias for log)",
        "domain": "x > 0",
        "range": "All real numbers",
        "derivative": "1/x",
        "integral": "x*ln(x) - x + C",
    },
    "exp": {
        "description": "Exponential function (e^x)",
        "domain": "All real numbers",
        "range": "x > 0",
        "derivative": "exp(x)",
        "integral": "exp(x) + C",
    },
    "sqrt": {
        "description": "Square root",
        "domain": "x >= 0",
        "range": "y >= 0",
        "derivative": "1/(2*sqrt(x))",
        "integral": "(2/3)*x^(3/2) + C",
    },
}
