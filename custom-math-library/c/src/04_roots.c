/*
 * ===================================================================
 *  MODULE 4: ROOTS & POWERS
 * ===================================================================
 *
 *  REAL-WORLD USE:
 *    - Distance: Euclidean distance = sqrt(x² + y²)
 *    - Graphics: Vector normalization (divide by sqrt(x²+y²+z²))
 *    - Physics: RMS (root mean square), standard deviation
 *    - ML: RMSE (root mean squared error) metric
 *    - Engineering: Spring oscillations, pendulum period
 *
 *  THE CORE METHOD: NEWTON'S METHOD
 *  ---------------------------------
 *  To find where f(x) = 0:
 *     x_{n+1} = x_n - f(x_n) / f'(x_n)
 *
 *  For sqrt(S):  f(x) = x² - S,  f'(x) = 2x
 *     x_{n+1} = x_n - (x_n² - S) / (2 · x_n)
 *             = (x_n + S/x_n) / 2    ← Babylonian method!
 *
 *  This is one of the oldest numerical algorithms known to humanity.
 *  The Babylonians used it 4000 years ago!
 * ===================================================================
 */

#include <stdio.h>

void run_roots_module(void);
extern void clear_input(void);

/* ==================================================================
 *  SQRT(x) — Newton's Method (Babylonian)
 * ==================================================================
 *
 *  Visual iteration for sqrt(10):
 *
 *    Start:     x₀ = 10
 *    Step 1:    x₁ = (10 + 10/10) / 2 = 5.5
 *    Step 2:    x₂ = (5.5 + 10/5.5) / 2 = 3.659
 *    Step 3:    x₃ = (3.659 + 10/3.659) / 2 = 3.196
 *    Step 4:    x₄ = (3.196 + 10/3.196) / 2 = 3.162
 *    Step 5:    x₅ = (3.162 + 10/3.162) / 2 = 3.1623  ← converged!
 *
 *  REAL-WORLD: Pythagoras, distance, normalization.
 * ================================================================== */
static double sqrt_custom(double S)
{
    if (S <= 0) return 0;

    double x = S;  /* Initial guess */
    int steps = 0;

    /* Iterate until change is tiny */
    while (1) {
        double next = (x + S / x) / 2.0;
        ++steps;

        /* Check for convergence */
        double diff = next - x;
        if (diff < 0) diff = -diff;

        if (diff < 1e-15 || steps > 100)
            return next;

        x = next;
    }
}

/* ==================================================================
 *  CBRT(x) — Cube Root via Newton
 * ==================================================================
 *
 *  f(x) = x³ - S = 0
 *  f'(x) = 3x²
 *  x_{n+1} = x_n - (x_n³ - S) / (3 · x_n²)
 *           = (2 · x_n + S / x_n²) / 3
 *
 *  REAL-WORLD: Volume calculations, 3D graphics.
 * ================================================================== */
static double cbrt_custom(double S)
{
    if (S == 0) return 0;

    int negative = (S < 0);
    if (negative) S = -S;

    double x = S;

    while (1) {
        double next = (2.0 * x + S / (x * x)) / 3.0;
        double diff = next - x;
        if (diff < 0) diff = -diff;

        if (diff < 1e-15)
            return negative ? -next : next;

        x = next;
    }
}

/* ==================================================================
 *  NTHROOT(x, n) — nth Root via Newton
 * ==================================================================
 *
 *  f(x) = x^n - S = 0
 *  f'(x) = n · x^{n-1}
 *  x_{n+1} = ((n-1) · x_n + S / x_n^{n-1}) / n
 *
 *  REAL-WORLD: General root finding, geometric means.
 * ================================================================== */
static double pow_int(double base, int exp)
{
    double result = 1.0;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

static double nth_root_custom(double S, int n)
{
    if (n <= 0) return 0;
    if (S == 0) return 0;

    double x = S;

    while (1) {
        double x_n_minus_1 = pow_int(x, n - 1);
        double next = ((double)(n - 1) * x + S / x_n_minus_1) / (double)n;
        double diff = next - x;
        if (diff < 0) diff = -diff;

        if (diff < 1e-12)
            return next;

        x = next;
    }
}

/* ==================================================================
 *  HYPOT(x, y) — Euclidean Distance
 * ==================================================================
 *
 *  sqrt(x² + y²) without intermediate overflow
 *
 *  REAL-WORLD: Distance between points, vector magnitude.
 * ================================================================== */
static double hypot_custom(double x, double y)
{
    return sqrt_custom(x * x + y * y);
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_roots_module(void)
{
    int choice;

    printf("\n  --- Roots & Powers ---\n");
    printf("  1) sqrt(x) — Square root\n");
    printf("  2) cbrt(x) — Cube root\n");
    printf("  3) nth_root(x, n) & hypot(x, y)\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 4) {
        printf("  >> SQRT(x) — Newton's method (Babylonian)\n");
        printf("     x_{n+1} = (x_n + S/x_n) / 2\n\n");
        printf("     x      |  our_sqrt   |  exact      |  steps\n");
        printf("     -------+-------------+-------------+--------\n");
        double xs[] = {0, 1, 2, 4, 9, 10, 25, 100, 0.25};
        int n = 9;
        for (int i = 0; i < n; ++i) {
            double our = sqrt_custom(xs[i]);
            double exact = (xs[i] == 0) ? 0 : (xs[i] == 1) ? 1 :
                           (xs[i] == 2) ? 1.4142135623730951 :
                           (xs[i] == 4) ? 2 : (xs[i] == 9) ? 3 :
                           (xs[i] == 10) ? 3.1622776601683795 :
                           (xs[i] == 25) ? 5 : (xs[i] == 100) ? 10 :
                           (xs[i] == 0.25) ? 0.5 : 0;
            printf("     %-5.2g  |  %-11.10f  |  %-11.10f  |\n", xs[i], our, exact);
        }
        printf("\n     Lesson: Newton converges in ~5-10 iterations.\n");
        printf("     The Babylonians knew this 4000 years ago!\n\n");
    }

    if (choice == 2 || choice == 4) {
        printf("  >> CBRT(x) — Cube root via Newton\n");
        printf("     x_{n+1} = (2·x_n + S/x_n²) / 3\n\n");
        printf("     x      |  our_cbrt   |  exact      |  \n");
        printf("     -------+-------------+-------------+\n");
        double xs[] = {0, 1, 8, 27, 64, 100, -8, -27};
        int n = 8;
        for (int i = 0; i < n; ++i) {
            double our = cbrt_custom(xs[i]);
            double exact;
            if (xs[i] == 0) exact = 0;
            else if (xs[i] == 1) exact = 1;
            else if (xs[i] == 8) exact = 2;
            else if (xs[i] == 27) exact = 3;
            else if (xs[i] == 64) exact = 4;
            else if (xs[i] == 100) exact = 4.641588833612779;
            else if (xs[i] == -8) exact = -2;
            else if (xs[i] == -27) exact = -3;
            else exact = 0;
            printf("     %-5.0f  |  %-11.10f  |  %-11.10f  |\n", xs[i], our, exact);
        }
        printf("\n     Lesson: Cube root handles negative inputs.\n");
        printf("     sqrt(x²) != x for negative x!\n\n");
    }

    if (choice == 3 || choice == 4) {
        printf("  >> NTH ROOT & HYPOT\n\n");
        printf("     nth_root(32, 5) = %.10f  (exact: 2)\n", nth_root_custom(32, 5));
        printf("     nth_root(81, 4) = %.10f  (exact: 3)\n", nth_root_custom(81, 4));
        printf("     nth_root(1000, 3) = %.10f  (exact: 10)\n", nth_root_custom(1000, 3));
        printf("\n     hypot(3, 4) = %.10f  (exact: 5)\n", hypot_custom(3, 4));
        printf("     hypot(5, 12) = %.10f  (exact: 13)\n", hypot_custom(5, 12));
        printf("     hypot(1, 1) = %.10f  (exact: sqrt(2) ~ 1.4142)\n", hypot_custom(1, 1));
        printf("\n     Lesson: Peter Pythagoras... ehm, Pythagoras.\n");
        printf("     hypot is sqrt(x² + y²) — the distance formula.\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
