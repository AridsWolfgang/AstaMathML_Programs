# Custom Math Library

> A pedagogical reimplementation of common math functions from scratch — no `libm` beyond the absolute minimum. Teaches **how** calculators compute `sin`, `sqrt`, `ln`, `π`, `e`, and more.

## Project Structure

```
custom-math-library/
├── c/          # C99 implementation (standalone, no libm)
│   ├── src/
│   │   ├── main.c           # Menu-driven demo
│   │   ├── 01_constants.c   # π (Leibniz / Machin), e, φ
│   │   ├── 02_trig.c        # sin, cos, tan via Taylor
│   │   ├── 03_exp_log.c     # exp, ln, log10, pow
│   │   ├── 04_roots.c       # sqrt (Newton), cbrt (Halley), hypot
│   │   ├── 05_linalg.c      # Vectors, matrices, 3D rotation
│   │   ├── 06_stats.c       # Mean, median, variance, Pearson's r
│   │   └── 07_special.c     # Factorial, Fibonacci, GCD, sigmoid, ReLU
│   └── build/               # Compiled executable (gitignored)
├── cpp/        # C++17 port (same modules, std:: functional style)
│   ├── src/    # Same file layout
│   └── build/
├── python/     # Python 3 port (same modules)
│   ├── constants.py
│   ├── trig.py
│   ├── exp_log.py
│   ├── roots.py
│   ├── linalg.py
│   ├── stats.py
│   ├── special.py
│   └── main.py
└── README.md   # This file
```

## What Each Module Teaches

### 1. Constants
| Constant | Method | Key Insight |
|----------|--------|-------------|
| **π** | Leibniz series (`π/4 = 1 - 1/3 + 1/5 - ...`) | Simple but terribly slow (~100k terms for 4 digits) |
| **π** | Machin's formula (`π/4 = 4·arctan(1/5) - arctan(1/239)`) | 10 terms → full double precision |
| **e** | Taylor series (`e = Σ 1/n!`) | ~15 terms → full precision |
| **φ** | Continued fraction (`φ = 1 + 1/(1 + 1/(1 + ...))`) | 10 iterations → ~1e-15 error |

### 2. Trigonometry
All functions use **Taylor–Maclaurin series**:
- `sin(x) = x - x³/3! + x⁵/5! - ...`
- `cos(x) = sin(x + π/2)` (phase shift identity)
- Range reduction first maps any angle to `[-π/2, π/2]` where series converge fast
- 10 terms sufficient for ~1e-12 accuracy

### 3. Exponentials & Logarithms
- **`exp(x)`**: Taylor series with range reduction (`exp(x) = exp(x/2)²` for `x > 1`, and `exp(-x) = 1/exp(x)`)
- **`ln(x)`**: Newton's method on `f(y) = eʸ - x`, iterating `y ← y - (eʸ - x) / eʸ`
- **`log10(x)`**: `ln(x) / ln(10)`
- **`pow(x, y)`**: `exp(y · ln(x))`

### 4. Roots
- **`sqrt(S)`**: Newton–Raphson `x ← (x + S/x) / 2` — quadratic convergence, ~5 iterations to machine epsilon
- **`cbrt(S)`**: Halley's method `x ← (2x + S/x²) / 3` — cubic convergence
- **`hypot(a, b)`**: `sqrt(a² + b²)`

### 5. Linear Algebra
- Vector addition, dot product, magnitude
- Matrix-vector multiplication
- 3D rotation matrix (rotation around Z-axis by 45°)

### 6. Statistics
- **Mean**: arithmetic average
- **Median**: middle of sorted data (handles even/odd)
- **Variance / StdDev**: `σ² = Σ(xᵢ - μ)² / n`
- **Pearson's r**: `cov(X,Y) / (σₓ · σᵧ)` — measures linear correlation

### 7. Special Functions
- **Factorial**: iterative product (handles 0! = 1)
- **Fibonacci**: iterative O(n) sequence generation
- **GCD**: Euclidean algorithm `gcd(a,b) = gcd(b, a mod b)`
- **Prime sieve**: trial division up to `√n`
- **Sigmoid**: `σ(x) = 1 / (1 + e⁻ˣ)` — standard ML activation
- **ReLU**: `max(0, x)` — modern neural network activation

## Building & Running

### C (gcc / MSVC)
```bash
cd custom-math-library/c
gcc -std=c99 -Wall -Wextra -O2 src/*.c -o build\custom_math.exe
.\build\custom_math.exe
```

### C++ (g++ / MSVC)
```bash
cd custom-math-library/cpp
g++ -std=c++17 -Wall -Wextra -O2 src/*.cpp -o build\custom_math.exe
.\build\custom_math.exe
```

### Python
```bash
cd custom-math-library/python
python main.py
```

## The Big Picture

Every function here implements the same algorithm in three languages. The real educational value is:

1. **Numerical methods matter** — naive series don't converge for large inputs; range reduction, argument reduction, and Newton iteration are essential tools.
2. **No magic in `libm`** — your calculator's `sin` button runs something very close to this code.
3. **Cross-language comparison** — same logic, different syntax. Compare the C pointer-based linalg to the Python list-comprehension version.

## License

Educational use. Do whatever you want with it.
