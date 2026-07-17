# Numeric Methods Engine

An interactive C99 command-line toolkit implementing fundamental numerical methods for root finding, integration, ODE solving, and linear systems. Built for educational exploration and benchmarking.

## Building

```bash
cd numeric-methods-engine
gcc -std=c99 -Wall -Wextra -O2 src/*.c -o build/numeric_methods.exe -lm
```

Run: `build\numeric_methods.exe` (Windows) or `./build/numeric_methods` (Linux).

## Root Finding

All methods find `x` such that `f(x) = 0`.

### Bisection Method

- **Concept**: Intermediate Value Theorem — if `f(a) * f(b) < 0` then a root lies in `[a,b]`. Repeatedly halve the interval.
- **Convergence**: Linear, guaranteed for continuous functions.
- **Pros**: Always converges given sign change. No derivative needed.
- **Cons**: Slow. Requires initial bracket `[a,b]` with opposite signs.

**Algorithm**:
1. Compute `c = (a + b) / 2`.
2. If `f(c) == 0` or `(b - a)/2 < ε`, return `c`.
3. If `f(c) * f(a) < 0`, set `b = c`; else `a = c`.
4. Repeat.

### Newton-Raphson Method

- **Concept**: Use tangent line approximation: `x_{n+1} = x_n - f(x_n) / f'(x_n)`.
- **Convergence**: Quadratic near simple roots.
- **Pros**: Very fast convergence.
- **Cons**: Requires derivative. May diverge if initial guess is poor.

**Algorithm**:
1. Guess `x_0`.
2. Iterate `x_{n+1} = x_n - f(x_n) / f'(x_n)` until `|x_{n+1} - x_n| < ε`.

### Secant Method

- **Concept**: Approximate derivative via finite difference: `f'(x_n) ≈ (f(x_n) - f(x_{n-1})) / (x_n - x_{n-1})`.
- **Convergence**: Superlinear (order ≈ 1.618).
- **Pros**: No derivative needed. Faster than bisection.
- **Cons**: May fail if function values are nearly equal.

**Algorithm**:
1. Two initial guesses `x_0`, `x_1`.
2. Compute `x_{n+1} = x_n - f(x_n) * (x_n - x_{n-1}) / (f(x_n) - f(x_{n-1}))`.
3. Repeat until convergence.

## Numerical Integration

Approximate `∫_a^b f(x) dx` using discrete samples.

### Rectangle Rule (Midpoint)

- **Concept**: Approximate area under curve by rectangles centered at each subinterval midpoint.
- **Formula**: `∫ f(x) dx ≈ h Σ f(a + (i+0.5)h)` where `h = (b-a)/n`.
- **Error**: `O(h^2)`.

### Trapezoidal Rule

- **Concept**: Approximate area by trapezoids formed by linear interpolation between endpoints.
- **Formula**: `∫ f(x) dx ≈ h [½f(a) + Σ_{i=1}^{n-1} f(a+ih) + ½f(b)]`.
- **Error**: `O(h^2)`.

### Simpson's Rule

- **Concept**: Approximate each pair of subintervals by a quadratic polynomial (parabola).
- **Formula**: `∫ f(x) dx ≈ h/3 [f(a) + 4f(a+h) + 2f(a+2h) + 4f(a+3h) + ... + f(b)]`.
  Requires `n` even.
- **Error**: `O(h^4)` — significantly more accurate for smooth functions.

## ODE Solving

Solve first-order initial value problem: `dy/dt = f(t,y)`, `y(t₀) = y₀`.

### Euler's Method

- **Concept**: Forward difference approximation: `y_{n+1} = y_n + h f(t_n, y_n)`.
- **Error**: `O(h)` — first order.
- **Pros**: Simplest method.
- **Cons**: Inaccurate for stiff equations; may become unstable with large step size.

### Runge-Kutta 4 (RK4)

- **Concept**: Four-stage explicit method that computes weighted average of slopes.
- **Stages**:
  - `k₁ = h f(t_n, y_n)`
  - `k₂ = h f(t_n + h/2, y_n + k₁/2)`
  - `k₃ = h f(t_n + h/2, y_n + k₂/2)`
  - `k₄ = h f(t_n + h, y_n + k₃)`
  - `y_{n+1} = y_n + (k₁ + 2k₂ + 2k₃ + k₄)/6`
- **Error**: `O(h^4)`.
- **Pros**: Excellent accuracy-to-cost ratio. Workhorse of numerical ODE solving.

## Linear System Solving

Solve `Ax = b` for `x ∈ ℝⁿ`.

### Gauss Elimination (Direct)

- **Concept**: Transform augmented matrix `[A|b]` to row-echelon form via partial pivoting, then back-substitute.
- **Steps**:
  1. **Forward elimination**: For each column, find pivot (largest element), swap rows, eliminate below.
  2. **Back substitution**: Solve from last row upward.
- **Complexity**: `O(n³)`.
- **Pros**: Direct, exact (in exact arithmetic). Handles general systems.
- **Cons**: Sensitive to roundoff; requires pivoting for stability.

### Gauss-Seidel (Iterative)

- **Concept**: Update each variable sequentially using the most recent values: `x_i^{(k+1)} = (b_i - Σ_{j<i} a_{ij} x_j^{(k+1)} - Σ_{j>i} a_{ij} x_j^{(k)}) / a_{ii}`.
- **Convergence**: Guaranteed for strictly diagonally dominant or symmetric positive-definite matrices.
- **Pros**: Simple, memory efficient for sparse systems.
- **Cons**: May not converge; slower than direct methods for dense systems.

## File Structure

```
numeric-methods-engine/
├── README.md
├── header.h              # Common declarations and includes
├── src/
│   ├── main.c            # Menu-driven interactive CLI
│   ├── 01_roots.c        # Bisection, Newton-Raphson, Secant
│   ├── 02_integration.c  # Rectangle, Trapezoidal, Simpson
│   ├── 03_ode.c          # Euler, RK4
│   └── 04_linsys.c       # Gauss elimination, Gauss-Seidel
└── build/
    └── numeric_methods.exe
```

## References

- Burden & Faires, *Numerical Analysis*, 10th ed.
- Press et al., *Numerical Recipes in C*, 2nd ed.
