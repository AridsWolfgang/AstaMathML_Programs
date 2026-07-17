/*
 * ===================================================================
 *  MODULE 6: STATISTICS
 * ===================================================================
 *
 *  REAL-WORLD USE:
 *    - Data Science: Every analysis starts with descriptive stats
 *    - Quality Control: Mean + stddev to detect defects
 *    - Finance: Risk = stddev of returns; Sharpe ratio
 *    - ML: Feature normalization (z-score = (x - mean) / stddev)
 *    - A/B Testing: Compare means and variances of groups
 *
 *  FUNCTIONS:
 *    mean       — Average of a dataset
 *    median     — Middle value (50th percentile)
 *    variance   — Average squared deviation from mean
 *    stddev     — Square root of variance
 *    min, max   — Range boundaries
 *    correlation — How two variables relate (Pearson's r)
 *
 *  KEY INSIGHT:
 *    Statistics is about understanding DATA through numbers.
 *    "All models are wrong, but some are useful." — George Box
 * ===================================================================
 */

#include <stdio.h>

void run_stats_module(void);
extern void clear_input(void);

/* Bubble sort for median (small datasets only) */
static void sort_array(double *arr, int n)
{
    for (int i = 0; i < n - 1; ++i) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                double tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

/* ==================================================================
 *  MEAN (Average)
 * ==================================================================
 *
 *  μ = (x₁ + x₂ + ... + x_n) / n
 *
 *  REAL-WORLD: Average height, average income, average test score.
 *  The most common statistic in the world.
 * ================================================================== */
static double mean(const double *data, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; ++i) sum += data[i];
    return sum / n;
}

/* ==================================================================
 *  MEDIAN
 * ==================================================================
 *
 *  The middle value when data is sorted.
 *  50% of values are below, 50% above.
 *
 *  REAL-WORLD: Median income (better than mean for skewed data),
 *  median house price, median age.
 * ================================================================== */
static double median(double *data, int n)
{
    sort_array(data, n);

    if (n % 2 == 1)
        return data[n / 2];
    else
        return (data[n / 2 - 1] + data[n / 2]) / 2.0;
}

/* ==================================================================
 *  VARIANCE & STANDARD DEVIATION
 * ==================================================================
 *
 *  σ² = Σ (x_i - μ)² / n      (population variance)
 *  σ  = sqrt(σ²)               (standard deviation)
 *
 *  Visual:
 *    Data points:    x₁  x₂  x₃  x₄  x₅
 *    Mean (μ):       --------+---------
 *    Deviations:     ←---→   ←---→
 *                    (x_i - μ)
 *    Variance:       Average of squared deviations
 *    Stddev:         sqrt(variance) — back to original units
 *
 *  REAL-WORLD: σ tells you how spread out the data is.
 *    - σ = 1: 68% of data within ±1σ of mean (normal distribution)
 *    - σ = 2: 95% within ±2σ
 *    - σ = 3: 99.7% within ±3σ
 * ================================================================== */
static double variance(const double *data, int n, double m)
{
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        double diff = data[i] - m;
        sum += diff * diff;
    }
    return sum / n;
}

static double stddev_custom(const double *data, int n, double m)
{
    double var = variance(data, n, m);
    /* sqrt via Newton */
    double x = var;
    for (int i = 0; i < 20; ++i)
        x = (x + var / x) / 2.0;
    return x;
}

/* ==================================================================
 *  PEARSON CORRELATION COEFFICIENT
 * ==================================================================
 *
 *  r = Σ (x_i - x̄)(y_i - ȳ) / (n · σ_x · σ_y)
 *
 *  r ranges from -1 to +1:
 *    +1: Perfect positive correlation
 *     0: No linear correlation
 *    -1: Perfect negative correlation
 *
 *  Visual:
 *    r ≈ 0.9:   r ≈ 0.5:    r ≈ 0.0:    r ≈ -0.8:
 *    ↗ ↗ ↗      ↗  ↗        · · ·       · · ↘
 *    ↗ ↗ ↗      · ↗ ·        · · ·       · · ·
 *    ↗ ↗ ↗      · · ·        · · ·       · · ·
 *
 *  REAL-WORLD: Does studying more hours correlate with higher grades?
 *  Does ice cream sales correlate with drowning? (spurious!)
 * ================================================================== */
static double correlation(const double *x, const double *y, int n)
{
    double mx = mean(x, n);
    double my = mean(y, n);
    double sx = stddev_custom(x, n, mx);
    double sy = stddev_custom(y, n, my);

    double cov = 0.0;
    for (int i = 0; i < n; ++i)
        cov += (x[i] - mx) * (y[i] - my);
    cov /= n;

    return cov / (sx * sy);
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_stats_module(void)
{
    int choice;

    printf("\n  --- Statistics ---\n");
    printf("  1) Mean & Median\n");
    printf("  2) Variance & Std Dev\n");
    printf("  3) Correlation\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    /* Student test scores dataset */
    double scores[] = {85, 92, 78, 90, 88, 76, 95, 89, 84, 91, 73, 87};
    int n = sizeof(scores) / sizeof(scores[0]);

    /* Study hours vs. test scores (for correlation demo) */
    double hours[] = {2, 3, 5, 4, 6, 1, 7, 5, 3, 6, 2, 4};
    double scores2[] = {72, 78, 88, 85, 92, 65, 95, 90, 80, 91, 70, 82};

    if (choice == 1 || choice == 4) {
        printf("  >> MEAN & MEDIAN\n\n");
        printf("  Test scores: ");
        for (int i = 0; i < n; ++i) printf("%.0f ", scores[i]);
        printf("\n");

        double m = mean(scores, n);
        /* Copy data for median (it sorts) */
        double sorted[12];
        for (int i = 0; i < n; ++i) sorted[i] = scores[i];
        double med = median(sorted, n);

        printf("\n     Mean   = %.2f  (sum / n)\n", m);
        printf("     Median = %.2f  (middle value when sorted)\n", med);

        printf("\n     Lesson: Mean is sensitive to outliers.\n");
        printf("     Median is robust to outliers.\n");
        printf("     If mean > median, data is right-skewed.\n\n");

        /* Demo skewness */
        double income[] = {30, 32, 35, 33, 31, 120};  /* CEO earns 120K */
        int n_inc = 6;
        double sorted_inc[6];
        for (int i = 0; i < n_inc; ++i) sorted_inc[i] = income[i];
        printf("  Incomes (K): ");
        for (int i = 0; i < n_inc; ++i) printf("%.0f ", income[i]);
        printf("\n     Mean   = %.2f  (pulled up by outlier)\n", mean(income, n_inc));
        printf("     Median = %.2f  (unaffected)\n", median(sorted_inc, n_inc));
        printf("     The CEO's high salary skews the mean!\n\n");
    }

    if (choice == 2 || choice == 4) {
        printf("  >> VARIANCE & STANDARD DEVIATION\n\n");
        printf("  Test scores: ");
        for (int i = 0; i < n; ++i) printf("%.0f ", scores[i]);
        printf("\n");

        double m = mean(scores, n);
        double var = variance(scores, n, m);
        double sd = stddev_custom(scores, n, m);

        printf("\n     Mean      = %.2f\n", m);
        printf("     Variance  = %.2f  (avg squared deviation)\n", var);
        printf("     Std Dev   = %.2f  (√variance)\n", sd);
        printf("     68%% of scores: [%.1f, %.1f]\n", m - sd, m + sd);
        printf("     95%% of scores: [%.1f, %.1f]\n", m - 2*sd, m + 2*sd);

        printf("\n     Lesson: StdDev is the 'spread' in original units.\n");
        printf("     In a normal distribution, 68%% is within ±1σ.\n\n");
    }

    if (choice == 3 || choice == 4) {
        printf("  >> CORRELATION\n\n");
        printf("  Hours studied: ");
        for (int i = 0; i < n; ++i) printf("%.0f ", hours[i]);
        printf("\n  Test scores:   ");
        for (int i = 0; i < n; ++i) printf("%.0f ", scores2[i]);
        printf("\n\n");

        double r = correlation(hours, scores2, n);
        printf("  Pearson's r = %.4f\n", r);

        if (r > 0.7) printf("  Interpretation: Strong positive correlation\n");
        else if (r > 0.3) printf("  Interpretation: Moderate positive correlation\n");
        else printf("  Interpretation: Weak or no correlation\n");

        printf("\n     Lesson: Correlation ≠ Causation!\n");
        printf("     Ice cream sales and drowning both rise in summer.\n");
        printf("     But ice cream doesn't cause drowning — heat does.\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
