# Optimization Engine

An interactive Python suite for numerical optimization — gradient descent methods, linear programming via the simplex method, genetic algorithms, and constrained optimization using penalty methods — built entirely from scratch using only the Python standard library.

---

## Table of Contents

1. [Unconstrained Optimization](#1-unconstrained-optimization)
   - [Gradient Descent (SGD)](#gradient-descent-sgd)
   - [Momentum](#momentum)
   - [Adam Optimizer](#adam-optimizer)
   - [Convergence Criteria](#convergence-criteria)
2. [Linear Programming](#2-linear-programming)
   - [Simplex Method](#simplex-method)
3. [Evolutionary Algorithms](#3-evolutionary-algorithms)
   - [Genetic Algorithm](#genetic-algorithm)
4. [Constrained Optimization](#4-constrained-optimization)
   - [Penalty Method](#penalty-method)
5. [Benchmark Functions](#5-benchmark-functions)
6. [Comparison & Analysis](#6-comparison--analysis)
7. [File Reference](#7-file-reference)

---

## 1. Unconstrained Optimization

We seek to minimize $f: \mathbb{R}^n \to \mathbb{R}$ without constraints.

### Gradient Descent (SGD)

The simplest first-order optimization method.

$$ x_{k+1} = x_k - \alpha \nabla f(x_k) $$

where α is the **learning rate** (step size).

**Intuition:** The gradient points in the direction of steepest ascent; move opposite it.

**Challenges:**
- Choosing α: too small = slow, too large = divergence
- Can oscillate in narrow valleys
- Gets stuck in local minima for non-convex functions

**Convergence:** For convex functions with Lipschitz gradient, SGD converges at rate O(1/k).

### Momentum

Accumulates a velocity vector to smooth updates and accelerate convergence.

$$ v_{k+1} = \beta v_k + \alpha \nabla f(x_k) $$
$$ x_{k+1} = x_k - v_{k+1} $$

where β ∈ [0, 1) is the **momentum coefficient** (typically 0.9).

**Intuition:** Like a ball rolling downhill — momentum carries it through flat regions and dampens oscillations.

**Effect:**
- Speeds up convergence in consistent gradient directions
- Reduces oscillation in narrow valleys
- Can overshoot minima

### Adam Optimizer

**Adam** (Adaptive Moment Estimation) combines momentum with per-parameter adaptive learning rates.

$$ m_k = \beta_1 m_{k-1} + (1-\beta_1) g_k $$
$$ v_k = \beta_2 v_{k-1} + (1-\beta_2) g_k^2 $$
$$ \hat{m}_k = \frac{m_k}{1-\beta_1^k}, \quad \hat{v}_k = \frac{v_k}{1-\beta_2^k} $$
$$ x_{k+1} = x_k - \alpha \frac{\hat{m}_k}{\sqrt{\hat{v}_k} + \epsilon} $$

Where:
- m = first moment (mean of gradients) ≈ momentum
- v = second moment (uncentered variance) ≈ adaptive step size
- β₁ = 0.9, β₂ = 0.999, ε = 10⁻⁸ (defaults)

**Advantages:**
- Handles sparse gradients well
- Works well with noisy or non-stationary objectives
- Requires little hyperparameter tuning
- Bias correction for early iterations

**Convergence:** Adam has been shown to converge in convex settings and works very well in practice for deep learning.

### Convergence Criteria

Common stopping conditions:
- $\|\nabla f(x_k)\| < \text{tol}$ — gradient magnitude below threshold
- $|f(x_{k+1}) - f(x_k)| < \text{tol}$ — function value stabilizes
- Maximum iterations reached

---

## 2. Linear Programming

**Linear Program (LP):**

$$ \text{maximize } c^T x \quad \text{subject to } Ax \leq b, \; x \geq 0 $$

- c = objective coefficients (n×1)
- A = constraint matrix (m×n)
- b = right-hand side (m×1)

### Simplex Method

The simplex algorithm moves along edges of the feasible polytope to find the optimal vertex.

**Key concepts:**

1. **Standard form:** Add slack variables to convert inequalities to equalities
2. **Basis:** A set of m variables (basic) that form a vertex. The remaining n variables are non-basic (= 0)
3. **Tableau:** Matrix representation of constraints + objective
4. **Pivot:** Move to an adjacent vertex by swapping a basic variable with a non-basic one

**Algorithm:**
1. Start with a feasible basis (slack variables)
2. **Entering variable:** Column with most negative reduced cost (objective coefficient in tableau)
3. **Leaving variable:** Minimum ratio test $b_i / a_{ij}$ for positive $a_{ij}$
4. **Pivot:** Perform row operations to make entering column a unit vector
5. Repeat until all reduced costs ≥ 0 (optimal)

**Degeneracy:** When the minimum ratio test gives a zero ratio, the pivot doesn't improve the objective. Cycling can occur but is rare.

**Two-phase method:** Phase I finds a feasible starting basis; Phase II optimizes.

**Time Complexity:** Exponential worst-case (Klee-Minty cube), but polynomial average-case. Interior point methods (not implemented here) are polynomial.

---

## 3. Evolutionary Algorithms

### Genetic Algorithm (GA)

A population-based metaheuristic inspired by natural selection.

**Components:**

1. **Population:** Set of candidate solutions, each encoded as a vector of real numbers
2. **Selection:** Choose parents based on fitness (tournament selection in this implementation)
3. **Crossover (SBX):** Simulated Binary Crossover creates offspring by mixing parent genes
4. **Mutation:** Gaussian noise added to genes to maintain diversity
5. **Elitism:** Best individuals survive unchanged to next generation

**Algorithm:**
1. Initialize random population of size N
2. Evaluate fitness of each individual
3. While not converged:
   a. Select parents via tournament
   b. Apply crossover with probability $p_c$
   c. Apply mutation with probability $p_m$
   d. Evaluate offspring fitness
   e. Replace population (with elitism)

**Hyperparameters:**
- Population size: 50–200
- Crossover rate: 0.7–0.9
- Mutation rate: 0.01–0.2
- Tournament size: 2–5

**Advantages:**
- No gradient required
- Handles non-differentiable, discontinuous, multimodal functions
- Good global search capability

**Disadvantages:**
- Slow convergence near optimum
- No convergence guarantees
- Many hyperparameters to tune

---

## 4. Constrained Optimization

### Penalty Method

Converts a constrained problem into an unconstrained one by adding a penalty for constraint violations.

**Problem:**
$$ \min f(x) \quad \text{s.t.} \quad g_i(x) \leq 0, \; h_j(x) = 0 $$

**Penalty function:**
$$ P(x, \mu) = f(x) + \frac{\mu}{2} \left( \sum_j h_j(x)^2 + \sum_i \max(0, g_i(x))^2 \right) $$

**Algorithm:**
1. Start with small penalty μ
2. Minimize P(x, μ) using unconstrained optimizer
3. Multiply μ by a factor (e.g., 10)
4. Repeat until constraints are satisfied

**Convergence:** As μ → ∞, the minimizer of P approaches the constrained optimum. However, large μ makes the problem ill-conditioned.

**Trade-off:**
- Small μ: easy to minimize, but constraints may be violated
- Large μ: accurate solution, but harder to optimize

---

## 5. Benchmark Functions

### Sphere Function
$$ f(x) = \sum_{i=1}^n x_i^2 $$

- Global minimum: f(0, ..., 0) = 0
- Simple, convex, single minimum
- Easy for all optimizers

### Rosenbrock Function
$$ f(x, y) = (1 - x)^2 + 100(y - x^2)^2 $$

N-dimensional: $f(x) = \sum_{i=1}^{n-1} \left[ 100(x_{i+1} - x_i^2)^2 + (1 - x_i)^2 \right]$

- Global minimum: f(1, ..., 1) = 0
- Narrow, curved valley makes gradient descent difficult
- Known as the "banana function"

### Rastrigin Function
$$ f(x) = 10n + \sum_{i=1}^n \left( x_i^2 - 10\cos(2\pi x_i) \right) $$

- Global minimum: f(0, ..., 0) = 0
- Highly multimodal (many local minima)
- Tests ability to escape local optima

### Ackley Function
$$ f(x) = -20\exp\left(-0.2\sqrt{\frac{1}{n}\sum x_i^2}\right) - \exp\left(\frac{1}{n}\sum \cos(2\pi x_i)\right) + 20 + e $$

- Global minimum: f(0, ..., 0) = 0
- Many local minima around a central basin
- Challenging for local search methods

### Beale Function
$$ f(x, y) = (1.5 - x + xy)^2 + (2.25 - x + xy^2)^2 + (2.625 - x + xy^3)^2 $$

- Global minimum: f(3, 0.5) = 0
- Several valleys, one global minimum

### Himmelblau Function
$$ f(x, y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2 $$

- Four equal global minima (f = 0)
- Tests ability to find multiple optima

---

## 6. Comparison & Analysis

| Method | Gradient Required | Global Search | Convergence Rate | Suitable For |
|--------|-----------------|---------------|-----------------|-------------|
| SGD | Yes | No | O(1/k) | Smooth convex |
| Momentum | Yes | No | Faster than SGD | Ill-conditioned |
| Adam | Yes | No | Fast | General, noisy |
| Simplex | No (LP) | Yes (LP) | Exponential worst-case | Linear constraints |
| Genetic Alg | No | Yes | Slow near optimum | Multimodal, discrete |
| Penalty Method | Yes (inner) | No | Depends on inner | Constrained smooth |

**General advice:**
- For smooth convex problems: Adam or Momentum
- For non-differentiable/multimodal: Genetic Algorithm
- For linearly constrained: Simplex method
- For general constrained: Penalty method + Adam

---

## 7. File Reference

| File | Contents |
|------|----------|
| `main.py` | Interactive menu system |
| `gradient_descent.py` | SGD, Momentum, Adam from scratch |
| `linear_programming.py` | Simplex method (maximization/minimization) |
| `evolutionary.py` | Genetic algorithm with SBX crossover |
| `constrained.py` | Penalty method for constrained problems |
| `data/problems.py` | Benchmark functions (Sphere, Rosenbrock, Rastrigin, Ackley, Beale, Himmelblau) |
| `README.md` | This lecture document |

---

*All code uses only the Python standard library.*
