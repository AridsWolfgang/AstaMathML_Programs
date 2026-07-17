# Probability & Statistics Simulator

An interactive Python simulator for exploring probability distributions, Monte Carlo methods, the Central Limit Theorem, and hypothesis testing — built entirely from scratch using only the Python standard library.

---

## Table of Contents

1. [Probability Distributions](#1-probability-distributions)
   - [Normal Distribution](#normal-distribution)
   - [Binomial Distribution](#binomial-distribution)
   - [Poisson Distribution](#poisson-distribution)
   - [Uniform Distribution](#uniform-distribution)
2. [Sampling & Monte Carlo](#2-sampling--monte-carlo)
   - [Monte Carlo Method](#monte-carlo-method)
   - [Bootstrapping](#bootstrapping)
3. [Descriptive Statistics](#3-descriptive-statistics)
4. [Central Limit Theorem](#4-central-limit-theorem)
5. [Hypothesis Testing](#5-hypothesis-testing)
   - [t-Test](#t-test)
   - [Chi-Square Test](#chi-square-test)
6. [File Reference](#6-file-reference)

---

## 1. Probability Distributions

A **probability distribution** describes how probabilities are assigned to possible outcomes of a random variable.

### Normal Distribution

The normal (Gaussian) distribution is a continuous probability distribution symmetric about its mean. It is the cornerstone of statistical inference.

**Probability Density Function (PDF):**

$$ f(x \mid \mu, \sigma) = \frac{1}{\sigma \sqrt{2\pi}} \exp\left(-\frac{(x - \mu)^2}{2\sigma^2}\right) $$

Where:
- μ = mean (center)
- σ = standard deviation (spread)
- σ² = variance

**Properties:**
- Symmetric about μ (skewness = 0)
- Kurtosis = 3 (excess kurtosis = 0)
- ~68% of data within μ ± σ, ~95% within μ ± 2σ, ~99.7% within μ ± 3σ

**Generation (Box-Muller Transform):**
Two uniform random variables U₁, U₂ are transformed:
$$ Z = \sqrt{-2\ln U_1} \cdot \cos(2\pi U_2) $$
Then: $X = \mu + \sigma Z$

### Binomial Distribution

Models the number of successes in n independent Bernoulli trials, each with success probability p.

**Probability Mass Function (PMF):**

$$ P(X = k) = \binom{n}{k} p^k (1-p)^{n-k}, \quad k = 0, 1, \ldots, n $$

Where:
- $\binom{n}{k} = \frac{n!}{k!(n-k)!}$ is the binomial coefficient
- n = number of trials
- p = probability of success

**Properties:**
- Mean: μ = np
- Variance: σ² = np(1-p)
- Skewness: $\frac{1-2p}{\sqrt{np(1-p)}}$
- As n → ∞, Binomial(n, p) approaches Normal(np, np(1-p))

### Poisson Distribution

Models the number of events occurring in a fixed interval when events happen at a constant average rate λ.

**Probability Mass Function (PMF):**

$$ P(X = k) = \frac{e^{-\lambda} \lambda^k}{k!}, \quad k = 0, 1, 2, \ldots $$

**Properties:**
- Mean = Variance = λ
- Skewness = $1/\sqrt{\lambda}$
- Used for rare events, queueing theory, radioactive decay
- Approximation to Binomial when n is large and p is small

**Generation (Knuth's algorithm):**
Generate uniform random numbers until product ≤ e^(-λ); count - 1 is the Poisson sample.

### Uniform Distribution

All outcomes in an interval [a, b] are equally likely.

**Probability Density Function (PDF):**

$$ f(x) = \begin{cases} \frac{1}{b-a} & a \leq x \leq b \\ 0 & \text{otherwise} \end{cases} $$

**Properties:**
- Mean: (a + b) / 2
- Variance: (b - a)² / 12
- Skewness = 0, Kurtosis = -1.2 (excess)
- Foundation for random number generation

---

## 2. Sampling & Monte Carlo

### Monte Carlo Method

Monte Carlo methods use random sampling to estimate deterministic quantities.

**Estimating π:**
1. Generate random points (x, y) in the square [-1, 1] × [-1, 1]
2. Count points inside the unit circle: x² + y² ≤ 1
3. π ≈ 4 × (points inside) / (total points)

By the law of large numbers, the estimate converges to π as sample size grows.

**Monte Carlo Integration:**
$$ \int_a^b f(x)\,dx \approx \frac{b-a}{N} \sum_{i=1}^N f(x_i) $$
where xᵢ are uniform random samples in [a, b]. The error decreases as O(1/√N).

**Dice Roll Simulation:**
The sum of dice follows a discrete convolution. For two fair 6-sided dice, the distribution is triangular with the most likely sum being 7 (probability 1/6).

### Bootstrapping

Bootstrapping is a resampling technique that estimates the sampling distribution of a statistic by repeatedly sampling with replacement from the observed data.

**Procedure:**
1. Given data of size n
2. Draw B resamples of size n with replacement
3. Compute the statistic for each resample
4. Use the empirical distribution to estimate confidence intervals

A 95% bootstrap confidence interval is given by the 2.5th and 97.5th percentiles of the bootstrap distribution.

---

## 3. Descriptive Statistics

For a dataset {x₁, x₂, ..., xₙ}:

| Statistic | Formula | Purpose |
|-----------|---------|---------|
| **Mean** | $\bar{x} = \frac{1}{n}\sum_{i=1}^n x_i$ | Center |
| **Variance** | $s^2 = \frac{1}{n-1}\sum (x_i - \bar{x})^2$ | Spread |
| **Std Dev** | $s = \sqrt{s^2}$ | Spread (same units) |
| **Skewness** | $\frac{1}{n}\sum \left(\frac{x_i - \bar{x}}{s}\right)^3$ | Asymmetry |
| **Kurtosis** | $\frac{1}{n}\sum \left(\frac{x_i - \bar{x}}{s}\right)^4 - 3$ | Tail weight |
| **Covariance** | $\frac{1}{n-1}\sum (x_i - \bar{x})(y_i - \bar{y})$ | Joint variation |
| **Correlation** | $r = \frac{\text{Cov}(X,Y)}{s_X s_Y}$ | Linear association [-1, 1] |

- Skewness > 0: right tail heavier; < 0: left tail heavier
- Excess kurtosis > 0: heavy tails (leptokurtic); < 0: light tails (platykurtic)

---

## 4. Central Limit Theorem (CLT)

**The CLT states:** For any population with mean μ and finite variance σ², the sampling distribution of the sample mean approaches a normal distribution as the sample size n increases, regardless of the population's shape.

$$ \bar{X}_n \xrightarrow{d} \mathcal{N}\left(\mu, \frac{\sigma^2}{n}\right) $$

**Key insights:**
- The mean of sample means = population mean μ
- The standard deviation of sample means = σ/√n (standard error)
- For n ≥ 30, the approximation is usually good even for skewed populations
- This explains why the normal distribution appears so frequently in nature

**Demo in the simulator:**
1. Start with a non-normal population (uniform, exponential, binomial)
2. Draw many samples of various sizes
3. Observe the histogram of sample means becoming bell-shaped as n increases

---

## 5. Hypothesis Testing

Hypothesis testing is a framework for making decisions using data. It involves:
- **H₀ (null hypothesis):** Default assumption (e.g., no difference)
- **H₁ (alternative hypothesis):** What we want to prove
- **p-value:** Probability of observing data at least as extreme as ours, assuming H₀ is true
- **Significance level α:** Threshold (typically 0.05)
- If p < α, we **reject H₀** (statistically significant result)

### t-Test

**One-sample t-test:** Tests if the population mean equals a specific value μ₀.

$$ t = \frac{\bar{x} - \mu_0}{s / \sqrt{n}} $$

Under H₀, t follows a Student's t-distribution with n-1 degrees of freedom.

**Two-sample independent t-test:** Tests if two populations have the same mean.

$$ t = \frac{\bar{x}_1 - \bar{x}_2}{\sqrt{\frac{s_1^2}{n_1} + \frac{s_2^2}{n_2}}} $$

Degrees of freedom are approximated by the Welch-Satterthwaite equation.

**Student's t-Distribution:**
- Symmetric, bell-shaped, heavier tails than normal
- As df → ∞, t-distribution → standard normal
- Used when population standard deviation is unknown

### Chi-Square Test

**Goodness-of-fit test:** Tests if observed frequencies match expected frequencies.

$$ \chi^2 = \sum_{i=1}^{k} \frac{(O_i - E_i)^2}{E_i} $$

Under H₀, χ² follows a chi-square distribution with k-1 degrees of freedom.

The chi-square distribution:
- Positively skewed
- Mean = df, Variance = 2·df
- As df → ∞, approaches a normal distribution

---

## 6. File Reference

| File | Contents |
|------|----------|
| `main.py` | Interactive menu system |
| `distributions.py` | Normal, Binomial, Poisson, Uniform classes with ASCII histogram |
| `sampling.py` | Monte Carlo π, integration, dice roll, bootstrap |
| `stats.py` | Mean, variance, skewness, kurtosis, covariance, correlation |
| `tests.py` | One-sample t, two-sample t, chi-square tests |
| `data/datasets.py` | Synthetic population generators |
| `README.md` | This lecture document |

---

*All code uses only the Python standard library — no numpy, scipy, or matplotlib.*
