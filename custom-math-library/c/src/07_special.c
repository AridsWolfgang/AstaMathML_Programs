/*
 * ===================================================================
 *  MODULE 7: SPECIAL FUNCTIONS & UTILITIES
 * ===================================================================
 *
 *  REAL-WORLD USE:
 *    - factorial:  Combinations, probability, algorithm analysis
 *    - fibonacci:  Nature (spirals, petals), algorithm (Fibonacci heap)
 *    - gcd/lcm:    Fraction reduction, cryptography (RSA)
 *    - is_prime:   Cryptography, hashing, random number generation
 *    - sigmoid:    Machine learning (logistic regression, neural nets)
 *    - relu:       Deep learning activation function
 *
 *  KEY INSIGHT:
 *    These "small" functions are the BUILDING BLOCKS of
 *    everything from cryptography to artificial intelligence.
 * ===================================================================
 */

#include <stdio.h>

void run_special_module(void);
extern void clear_input(void);

/* ==================================================================
 *  FACTORIAL
 * ==================================================================
 *
 *  n! = n × (n-1) × (n-2) × ... × 1
 *  5! = 5 × 4 × 3 × 2 × 1 = 120
 *
 *  REAL-WORLD: How many ways to arrange 52 cards?
 *  52! ≈ 8 × 10⁶⁷ — larger than atoms in the universe!
 * ================================================================== */
static unsigned long long factorial_iter(int n)
{
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

/* ==================================================================
 *  BINOMIAL COEFFICIENT
 * ==================================================================
 *
 *  C(n, k) = n! / (k! × (n-k)!)
 *
 *  Also known as "n choose k" — number of ways to pick k items
 *  from a set of n items, without regard to order.
 *
 *  Pascal's Triangle visual:
 *           1
 *         1   1
 *       1   2   1
 *     1   3   3   1
 *   1   4   6   4   1
 *
 *  REAL-WORLD: Lottery odds, probability, binomial theorem.
 * ================================================================== */
static unsigned long long binomial(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;

    /* Use the multiplicative formula to avoid overflow:
     * C(n,k) = Π (n - k + i) / i   for i = 1..k */
    unsigned long long result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * (unsigned long long)(n - k + i) / (unsigned long long)i;
    }
    return result;
}

/* ==================================================================
 *  FIBONACCI (Iterative)
 * ==================================================================
 *
 *  0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, ...
 *
 *  REAL-WORLD: Fibonacci search, Fibonacci heap (priority queue),
 *  nature (flower petals, pinecones, nautilus shells).
 *
 *  Fun fact: F(n) / F(n-1) → φ (golden ratio) as n → ∞
 * ================================================================== */
static unsigned long long fibonacci(int n)
{
    if (n <= 0) return 0;
    if (n == 1) return 1;

    unsigned long long prev2 = 0, prev1 = 1, current;

    for (int i = 2; i <= n; ++i) {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}

/* ==================================================================
 *  GCD & LCM
 * ==================================================================
 *
 *  GCD (Euclidean algorithm):
 *    gcd(a, b) = gcd(b, a mod b)
 *    gcd(48, 18) = gcd(18, 12) = gcd(12, 6) = gcd(6, 0) = 6
 *
 *  LCM:
 *    lcm(a, b) = a × b / gcd(a, b)
 *
 *  REAL-WORLD: Fraction reduction (divide by gcd),
 *  RSA encryption (uses gcd for key generation),
 *  gear ratios in engineering.
 * ================================================================== */
static int gcd(int a, int b)
{
    while (b != 0) {
        int temp = b;
        b = a % b;   /* Remainder */
        a = temp;
    }
    return a;
}

static int lcm(int a, int b)
{
    return a / gcd(a, b) * b;  /* Divide first to avoid overflow */
}

/* ==================================================================
 *  PRIME TESTING (Trial Division)
 * ==================================================================
 *
 *  A prime is a number > 1 divisible only by 1 and itself.
 *
 *  We check divisors up to √n. If none divide n, it's prime.
 *
 *  REAL-WORLD: RSA encryption uses product of two LARGE primes.
 *  Hash tables use prime sizes to reduce collisions.
 * ================================================================== */
static int is_prime(int n)
{
    if (n < 2) return 0;
    if (n == 2 || n == 3) return 1;
    if (n % 2 == 0) return 0;

    /* Check divisors up to sqrt(n) */
    int sqrt_n = (int)(n / 2);  /* Rough bound */
    for (int d = 3; d * d <= n && d <= sqrt_n; d += 2) {
        if (n % d == 0) return 0;
    }

    return 1;
}

/* ==================================================================
 *  SIGMOID — The ML Workhorse
 * ==================================================================
 *
 *  σ(x) = 1 / (1 + e^{-x})
 *
 *  Maps any real number to a value between 0 and 1.
 *  Used for: binary classification probabilities.
 *
 *  Visual:
 *    σ(x)    1 ──────────────────────
 *            |
 *          0.5 ──────────
 *            |
 *            0 ──────────────────────
 *              -∞          0       +∞
 *
 *  REAL-WORLD: Logistic regression, neural network activations,
 *  spam detection, disease diagnosis.
 * ================================================================== */
static double sigmoid(double x)
{
    /* e^{-x} via our exp (assuming it's defined in exp_log module) */
    extern double exp_custom(double x, int terms);

    if (x > 0) {
        /* For positive x: 1 / (1 + e^{-x}) — stable */
        return 1.0 / (1.0 + exp_custom(-x, 15));
    } else {
        /* For negative x: e^x / (1 + e^x) — stable */
        double ex = exp_custom(x, 15);
        return ex / (1.0 + ex);
    }
}

/* ==================================================================
 *  RELU — Rectified Linear Unit
 * ==================================================================
 *
 *  ReLU(x) = max(0, x)
 *
 *  Visual:
 *    ReLU(x)   |
 *              |         /
 *              |        /
 *              |       /
 *              |      /
 *              |─────/
 *              |    /
 *              |   /
 *              |  /
 *              | /
 *              |/
 *              ─────────────────────
 *              0
 *
 *  REAL-WORLD: The most popular activation function in deep learning.
 *  Used in: every modern neural network (CNN, RNN, Transformer).
 *  Simple, fast, solves the vanishing gradient problem of sigmoid.
 * ================================================================== */
static double relu(double x)
{
    return (x > 0) ? x : 0.0;
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_special_module(void)
{
    int choice;

    printf("\n  --- Special Functions ---\n");
    printf("  1) Factorial & Binomial\n");
    printf("  2) Fibonacci\n");
    printf("  3) GCD, LCM & Primes\n");
    printf("  4) ML Activation Functions (Sigmoid, ReLU)\n");
    printf("  5) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 5) {
        printf("  >> FACTORIAL & BINOMIAL\n\n");
        printf("  n!  (factorials):\n");
        for (int n = 0; n <= 10; ++n)
            printf("     %2d! = %llu\n", n, factorial_iter(n));
        printf("  (13! = 6,227,020,800 exceeds 32-bit int max!)\n");

        printf("\n  C(n,k) \"n choose k\" (binomial coefficients):\n");
        for (int n = 0; n <= 5; ++n) {
            printf("     C(%d, k): ", n);
            for (int k = 0; k <= n; ++k)
                printf("%llu ", binomial(n, k));
            printf("\n");
        }
        printf("  Lesson: These are Pascal's triangle numbers!\n");
        printf("     Used in: probability, combinatorics.\n\n");
    }

    if (choice == 2 || choice == 5) {
        printf("  >> FIBONACCI\n\n");
        printf("  Fibonacci sequence:\n    ");
        for (int i = 0; i <= 20; ++i)
            printf("%llu ", fibonacci(i));
        printf("\n\n");
        printf("  F(20)/F(19) = %.10f\n", (double)fibonacci(20) / fibonacci(19));
        printf("  Golden ratio = 1.6180339887...\n");
        printf("  Lesson: F(n)/F(n-1) approaches φ as n grows.\n\n");
    }

    if (choice == 3 || choice == 5) {
        printf("  >> GCD, LCM & PRIMES\n\n");
        printf("  gcd(48, 18) = %d\n", gcd(48, 18));
        printf("  gcd(1071, 462) = %d\n", gcd(1071, 462));
        printf("  lcm(12, 18) = %d\n", lcm(12, 18));
        printf("  lcm(7, 11) = %d (primes!)\n", lcm(7, 11));

        printf("\n  Primes up to 50: ");
        for (int i = 2; i <= 50; ++i)
            if (is_prime(i)) printf("%d ", i);
        printf("\n");
        printf("  Is 7919 prime? %s\n", is_prime(7919) ? "Yes" : "No");
        printf("  (7919 is the 1000th prime)\n");
        printf("  Lesson: GCD is one of the oldest algorithms (Euclid, ~300 BC).\n\n");
    }

    if (choice == 4 || choice == 5) {
        printf("  >> ML ACTIVATION FUNCTIONS\n\n");
        printf("  Sigmoid: σ(x) = 1 / (1 + e^{-x})\n");
        printf("     x     |  σ(x)\n");
        printf("     -------+---------\n");
        double xs[] = {-5, -2, -1, 0, 1, 2, 5};
        for (int i = 0; i < 7; ++i)
            printf("     %-4.0f   |  %.6f\n", xs[i], sigmoid(xs[i]));

        printf("\n  ReLU: r(x) = max(0, x)\n");
        printf("     x     |  ReLU(x)\n");
        printf("     -------+---------\n");
        for (int i = 0; i < 7; ++i)
            printf("     %-4.0f   |  %.1f\n", xs[i], relu(xs[i]));

        printf("\n  Lesson: Sigmoid squashes to (0,1) — good for probabilities.\n");
        printf("     ReLU is 0 for negative, identity for positive — cheap!\n");
        printf("     ReLU replaced sigmoid in most deep networks ca. 2012.\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
