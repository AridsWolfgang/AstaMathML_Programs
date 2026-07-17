/*
 * ===================================================================
 *  MODULE 1: MATHEMATICAL CONSTANTS
 * ===================================================================
 *
 *  We don't just #define M_PI. We COMPUTE pi from scratch.
 *  This is how you truly understand where these numbers come from.
 *
 *  CONSTANTS COVERED:
 *    Pi (π)   — Leibniz series, Machin's formula, Monte Carlo
 *    E        — Taylor series
 *    Phi (φ)  — Golden ratio via continued fraction
 *    sqrt(2)  — via Newton's method (preview)
 *
 *  REAL-WORLD USE:
 *    - π:  Every circle, wave, oscillation in physics
 *    - e:  Population growth, compound interest, entropy
 *    - φ:  Art, architecture, optimization (golden-section search)
 *
 *  KEY INSIGHT:
 *    Constants are not magical numbers. They are the limits of
 *    infinite series and geometric relationships. You can compute
 *    them to any precision with enough terms.
 * ===================================================================
 */

#include <stdio.h>

void run_constants_module(void);
extern void clear_input(void);

/* ==================================================================
 *  PI (π) — Leibniz / Gregory Series
 * ==================================================================
 *
 *  π/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...
 *
 *  This is the SLOWEST converging series for pi.
 *  After 1,000,000 terms, you get only ~5 correct digits.
 *  We include it for its SIMPLICITY, not its speed.
 *
 *  Visual:
 *    π = 4 × (1 - 1/3 + 1/5 - 1/7 + 1/9 - ...)
 *           ↑    ↑     ↑     ↑     ↑     ↑
 *          n=0  n=1   n=2   n=3   n=4   n=5
 *
 *  REAL-WORLD: Not used for serious computation, but the
 *  alternating series pattern appears everywhere in math.
 * ================================================================== */
static double pi_leibniz(int terms)
{
    double sum = 0.0;
    for (int n = 0; n < terms; ++n) {
        double term = 1.0 / (2.0 * n + 1.0);
        if (n % 2 == 0)
            sum += term;   /* Even n: positive */
        else
            sum -= term;   /* Odd n: negative */
    }
    return 4.0 * sum;
}

/* ==================================================================
 *  PI (π) — Machin's Formula (fast convergence)
 * ==================================================================
 *
 *  π/4 = 4·arctan(1/5) - arctan(1/239)
 *
 *  Each term adds ~1.4 decimal digits. 10 terms → 14 correct digits.
 *  This was used for centuries to compute pi by hand.
 *
 *  REAL-WORLD: This is how pi was computed to hundreds of digits
 *  before computers. John Machin used it in 1706.
 * ================================================================== */

/* arctan(x) via Taylor series: arctan(x) = x - x³/3 + x⁵/5 - ... */
static double arctan_taylor(double x, int terms)
{
    double sum = 0.0;
    double x_pow = x;  /* x^1 */

    for (int n = 0; n < terms; ++n) {
        double term = x_pow / (2.0 * n + 1.0);
        if (n % 2 == 0)
            sum += term;
        else
            sum -= term;
        x_pow *= x * x;  /* Multiply by x² each iteration */
    }

    return sum;
}

static double pi_machin(int terms)
{
    /* π/4 = 4·arctan(1/5) - arctan(1/239) */
    double a = 4.0 * arctan_taylor(1.0 / 5.0, terms);
    double b = arctan_taylor(1.0 / 239.0, terms);
    return 4.0 * (a - b);
}

/* ==================================================================
 *  E (Euler's Number) — Taylor Series
 * ==================================================================
 *
 *  e = 1 + 1/1! + 1/2! + 1/3! + 1/4! + ...
 *
 *  This converges VERY fast — 15 terms gives full double precision.
 *
 *  Visual:
 *    e = 1 + 1 + 1/2 + 1/6 + 1/24 + 1/120 + ...
 *          ↑    ↑     ↑     ↑      ↑       ↑
 *        n=0  n=1   n=2   n=3    n=4     n=5
 *
 *  REAL-WORLD: Compound interest (A = P·e^rt),
 *  population growth, normal distribution, entropy.
 * ================================================================== */
static double e_taylor(int terms)
{
    double sum = 1.0;   /* n = 0 term */
    double fact = 1.0;  /* 0! = 1 */

    for (int n = 1; n <= terms; ++n) {
        fact *= n;      /* n! = (n-1)! * n */
        sum += 1.0 / fact;
    }

    return sum;
}

/* ==================================================================
 *  PHI (Golden Ratio) — Continued Fraction
 * ==================================================================
 *
 *  φ = (1 + sqrt(5)) / 2 = 1.6180339887...
 *
 *  As a continued fraction:
 *  φ = 1 + 1/(1 + 1/(1 + 1/(1 + ...)))
 *
 *  Every 5 iterations adds ~3 decimal digits of accuracy.
 *
 *  REAL-WORLD: Golden-section search (optimization),
 *  Fibonacci numbers, art/architecture proportions.
 * ================================================================== */
static double phi_continued(int iterations)
{
    double phi = 1.0;
    for (int i = 0; i < iterations; ++i)
        phi = 1.0 + 1.0 / phi;
    return phi;
}

/* ==================================================================
 *  SQRT(2) — Newton's Method (preview from Module 4)
 * ==================================================================
 *
 *  Newton's method: x_{n+1} = x_n - f(x_n)/f'(x_n)
 *  For sqrt(S):  f(x) = x² - S,  f'(x) = 2x
 *  So: x_{n+1} = (x_n + S/x_n) / 2
 *
 *  REAL-WORLD: Distance calculations, normalization.
 * ================================================================== */
static double sqrt_newton(double S)
{
    if (S <= 0) return 0;
    double x = S;  /* Initial guess */
    for (int i = 0; i < 20; ++i)
        x = (x + S / x) / 2.0;
    return x;
}

static double phi_exact(void)
{
    return (1.0 + sqrt_newton(5.0)) / 2.0;
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_constants_module(void)
{
    int choice;

    printf("\n  --- Mathematical Constants ---\n");
    printf("  1) Pi (Leibniz series — slow)\n");
    printf("  2) Pi (Machin's formula — fast)\n");
    printf("  3) Euler's number e\n");
    printf("  4) Golden ratio Phi\n");
    printf("  5) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 5) {
        printf("  >> PI via LEIBNIZ SERIES\n");
        printf("     π = 4 × (1 - 1/3 + 1/5 - 1/7 + ...)\n\n");
        int terms[] = {10, 100, 1000, 10000, 100000};
        for (int i = 0; i < 5; ++i) {
            double p = pi_leibniz(terms[i]);
            printf("     %6d terms: %.15f  (error: %+.10e)\n",
                   terms[i], p, p - 3.141592653589793);
        }
        printf("     Lesson: Leibniz converges VERY slowly.\n");
        printf("     After 100k terms, only ~5 decimal digits correct.\n\n");
    }

    if (choice == 2 || choice == 5) {
        printf("  >> PI via MACHIN'S FORMULA\n");
        printf("     π/4 = 4·arctan(1/5) - arctan(1/239)\n\n");
        for (int t = 2; t <= 12; t += 2) {
            double p = pi_machin(t);
            printf("     %2d terms: %.15f  (error: %+.10e)\n",
                   t, p, p - 3.141592653589793);
        }
        printf("     Lesson: 10 terms gives double-precision pi!\n");
        printf("     This is how pi was computed for 200+ years.\n\n");
    }

    if (choice == 3 || choice == 5) {
        printf("  >> EULER'S NUMBER e\n");
        printf("     e = 1 + 1/1! + 1/2! + 1/3! + ...\n\n");
        for (int t = 2; t <= 20; t += 2) {
            double e = e_taylor(t);
            printf("     %2d terms: %.15f  (error: %+.10e)\n",
                   t, e, e - 2.718281828459045);
        }
        printf("     Lesson: 15 terms gives double-precision e.\n");
        printf("     The factorial grows fast, terms shrink fast.\n\n");
    }

    if (choice == 4 || choice == 5) {
        printf("  >> GOLDEN RATIO Phi\n");
        printf("     φ = (1 + √5) / 2 = 1 + 1/(1 + 1/(1 + ...))\n\n");
        for (int t = 1; t <= 10; ++t) {
            double p = phi_continued(t);
            printf("     %2d iterations: %.15f  (error: %+.10e)\n",
                   t, p, p - 1.618033988749895);
        }
        printf("     Exact (via sqrt(5)): %.15f\n", phi_exact());
        printf("     Lesson: Continued fractions converge fast.\n");
        printf("     φ appears in Fibonacci ratios and nature.\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
