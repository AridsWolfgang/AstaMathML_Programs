/*
 * ===================================================================
 *  MODULE 3: EXPONENTIALS & LOGARITHMS
 * ===================================================================
 *
 *  REAL-WORLD USE:
 *    - Finance: Compound interest (A = P·e^rt)
 *    - Physics: Radioactive decay (N = N₀·e^{-λt})
 *    - ML: Softmax, sigmoid, cross-entropy loss
 *    - Audio: Decibels (log scale), equalizers
 *    - Biology: Population growth, bacterial cultures
 *    - CS: Information theory (log₂ for entropy)
 *
 *  FUNCTIONS:
 *    exp(x)  — e^x via Taylor series
 *    ln(x)   — Natural log via series + Newton's method
 *    log10(x) — Base-10 log via change of base
 *    pow(x,y) — x^y = exp(y · ln(x))
 * ===================================================================
 */

#include <stdio.h>

void run_exp_log_module(void);
extern void clear_input(void);

/* ==================================================================
 *  EXP(x) — Taylor Series
 * ==================================================================
 *
 *  e^x = 1 + x + x²/2! + x³/3! + x⁴/4! + ...
 *
 *  This converges for ALL x, but converges fastest near x=0.
 *  For large x, we use: e^x = (e^{x/2})²  (argument reduction)
 *
 *  Visual:
 *    e^x = Σ x^n / n!   for n = 0 to ∞
 *
 *    At x=2:  1 + 2 + 4/2 + 8/6 + 16/24 + ...
 *           = 1 + 2 + 2 + 1.333 + 0.667 + ...
 *           = 7.389...  (e² is actually ~7.389)
 *
 *  REAL-WORLD: Every growth/decay process in nature.
 * ================================================================== */
static double exp_taylor(double x, int terms)
{
    double sum = 1.0;  /* n=0 term: x⁰/0! = 1 */
    double term = 1.0; /* x^n / n! */

    for (int n = 1; n <= terms; ++n) {
        term *= x / (double)n;  /* x^n / n! = x^{n-1}/(n-1)! * x/n */
        sum += term;
    }

    return sum;
}

/*
 * Handle large positive/negative x by splitting:
 *   e^x = (e^{x/N})^N  where N is a power of 2
 * This keeps the Taylor argument small for fast convergence.
 */
double exp_custom(double x, int terms)
{
    if (x < 0)
        return 1.0 / exp_custom(-x, terms);  /* e^{-x} = 1/e^x */

    if (x > 1.0) {
        /* Reduce: e^x = (e^{x/2})² */
        double half = exp_custom(x / 2.0, terms);
        return half * half;
    }

    return exp_taylor(x, terms);
}

/* ==================================================================
 *  LN(x) — Natural Logarithm
 * ==================================================================
 *
 *  Method: Solve e^y = x for y using Newton's method.
 *    f(y) = e^y - x = 0
 *    f'(y) = e^y
 *    y_{n+1} = y_n - (e^{y_n} - x) / e^{y_n}
 *            = y_n - 1 + x / e^{y_n}
 *            = y_n + x / e^{y_n} - 1
 *
 *  Visual:
 *    ln(x) asks: "What power of e gives me x?"
 *    ln(100) = 4.605... because e^4.605... = 100
 *
 *  REAL-WORLD: Richter scale (log₁₀), pH (log₁₀),
 *  decibels (log₁₀), information entropy (log₂).
 * ================================================================== */
static double ln_custom(double x, int newton_iters)
{
    if (x <= 0) {
        printf("    ln(x) undefined for x <= 0\n");
        return 0;
    }

    /* For numbers not near 1, scale them:
     * ln(x) = ln(m · 2^k) = ln(m) + k · ln(2)
     * This keeps the Newton iteration stable. */
    double y = x / 2.0;  /* Initial guess */

    for (int i = 0; i < newton_iters; ++i) {
        double e_y = exp_custom(y, 15);
        y = y - (e_y - x) / e_y;  /* Newton update */
    }

    return y;
}

/* ==================================================================
 *  LOG10(x) — Change of Base
 * ==================================================================
 *
 *  log₁₀(x) = ln(x) / ln(10)
 *
 *  REAL-WORLD: pH scale, Richter scale, decibels.
 * ================================================================== */
static double log10_custom(double x, int iters)
{
    return ln_custom(x, iters) / ln_custom(10.0, iters);
}

/* ==================================================================
 *  POW(x, y) — Power Function
 * ==================================================================
 *
 *  x^y = e^{y · ln(x)}
 *
 *  This works for ALL positive x and any real y.
 *
 *  REAL-WORLD: Compound interest, physics formulas,
 *  machine learning (exponential activations).
 * ================================================================== */
static double pow_custom(double x, double y, int iters)
{
    if (x <= 0) {
        printf("    pow(x,y) defined for positive x only\n");
        return 0;
    }
    return exp_custom(y * ln_custom(x, iters), 15);
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_exp_log_module(void)
{
    int choice;

    printf("\n  --- Exponentials & Logarithms ---\n");
    printf("  1) exp(x) — e^x\n");
    printf("  2) ln(x) — Natural log\n");
    printf("  3) log10(x) & pow(x, y)\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 4) {
        printf("  >> EXP(x) — Taylor series (15 terms)\n");
        printf("     e^x = 1 + x + x²/2! + x³/3! + ...\n\n");
        printf("     x     |  our_exp    |  exact      |  error\n");
        printf("     ------+-------------+-------------+-------------\n");
        double xs[] = {-2, -1, 0, 0.5, 1, 2, 3, 5};
        double ex[] = {0.1353352832366127, 0.3678794411714423,
                       1.0, 1.6487212707001282, 2.718281828459045,
                       7.38905609893065, 20.085536923187668,
                       148.4131591025766};
        int n = 8;
        for (int i = 0; i < n; ++i) {
            double our = exp_custom(xs[i], 15);
            printf("     %-4.1f  |  %-11.8f  |  %-11.8f  |  %+.2e\n",
                   xs[i], our, ex[i], our - ex[i]);
        }
        printf("\n     Lesson: e^x grows FAST (e^10 ~ 22,026).\n");
        printf("     Our method: reduce x, apply Taylor, square back.\n\n");
    }

    if (choice == 2 || choice == 4) {
        printf("  >> LN(x) — Newton's method\n");
        printf("     Solve e^y = x for y using y = y - (e^y - x)/e^y\n\n");
        printf("     x     |  our_ln     |  exact      |  error\n");
        printf("     ------+-------------+-------------+-------------\n");
        double xs[] = {0.5, 1, 2, 3, 10, 100};
        double ex[] = {-0.6931471805599453, 0.0,
                       0.6931471805599453, 1.0986122886681098,
                       2.302585092994046, 4.605170185988092};
        int n = 6;
        for (int i = 0; i < n; ++i) {
            double our = ln_custom(xs[i], 10);
            printf("     %-4.1f  |  %-11.8f  |  %-11.8f  |  %+.2e\n",
                   xs[i], our, ex[i], our - ex[i]);
        }
        printf("\n     Lesson: ln(x) grows slowly (ln(1000) ~ 6.9).\n");
        printf("     Newton's method converges quadratically.\n\n");
    }

    if (choice == 3 || choice == 4) {
        printf("  >> LOG10(x) — Change of base: log₁₀(x) = ln(x)/ln(10)\n");
        printf("  >> POW(x, y) — x^y = e^{y·ln(x)}\n\n");
        printf("     log10(100)   = %.6f  (exact: 2)\n", log10_custom(100, 10));
        printf("     log10(1000)  = %.6f  (exact: 3)\n", log10_custom(1000, 10));
        printf("     pow(2, 10)   = %.6f  (exact: 1024)\n", pow_custom(2, 10, 10));
        printf("     pow(5, 3)    = %.6f  (exact: 125)\n", pow_custom(5, 3, 10));
        printf("     pow(10, -2)  = %.6f  (exact: 0.01)\n", pow_custom(10, -2, 10));
        printf("     pow(2, 0.5)  = %.6f  (exact: ~1.4142)\n", pow_custom(2, 0.5, 10));
        printf("\n     Lesson: x^y = e^{y·ln(x)} works for ANY real y.\n");
        printf("     Square roots are just pow(x, 0.5)!\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
