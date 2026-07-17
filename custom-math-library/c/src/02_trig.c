/*
 * ===================================================================
 *  MODULE 2: TRIGONOMETRY
 * ===================================================================
 *
 *  We implement sin, cos, and tan using TAYLOR SERIES — the same
 *  method your calculator uses internally.
 *
 *  REAL-WORLD USE:
 *    - Game physics: trajectory, rotation, collision
 *    - Signal processing: Fourier transforms, audio synthesis
 *    - Computer graphics: 3D rotation matrices, camera angles
 *    - Navigation: GPS coordinates, bearing calculations
 *    - Robotics: Inverse kinematics, joint angles
 *
 *  THE TAYLOR SERIES FOR SIN AND COS:
 *
 *     sin(x) = x - x³/3! + x⁵/5! - x⁷/7! + ...
 *     cos(x) = 1 - x²/2! + x⁴/4! - x⁶/6! + ...
 *
 *  Visual for sin(x):
 *
 *     f(x) ≈ P₀ + P₁·x + P₂·x² + P₃·x³ + ...
 *            ↑     ↑      ↑
 *          0th   1st    2nd derivative at x=0
 *
 *     sin(0) = 0, sin'(0) = 1, sin''(0) = 0, sin'''(0) = -1, ...
 *
 *     So: sin(x) ≈ x - x³/6 + x⁵/120 - ...
 *     (Only odd powers survive because sin is odd)
 *
 *  ACCURACY:
 *    10 terms gives double-precision for |x| < π/2.
 *    We use argument reduction to keep x in [-π, π].
 *
 *  COMPLEXITY: O(n) time where n = number of terms
 * ===================================================================
 */

#include <stdio.h>

void run_trig_module(void);
extern void clear_input(void);

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ==================================================================
 *  DEGREES TO RADIANS
 * ================================================================== */
static double deg_to_rad(double deg)
{
    return deg * M_PI / 180.0;
}

/* ==================================================================
 *  SIN(x) — Taylor Series
 * ==================================================================
 *
 *  We reduce the argument to [0, π/2] using trig identities,
 *  then apply the Taylor series.
 *
 *  Argument reduction:
 *    sin(x + 2πk) = sin(x)        ← periodicity
 *    sin(π - x)   = sin(x)         ← reflection
 *    sin(-x)      = -sin(x)        ← oddness
 * ---------------------------------------------------------------- */
static double sin_taylor(double x_rad, int terms)
{
    /* Reduce to [-π, π] */
    while (x_rad > M_PI)  x_rad -= 2.0 * M_PI;
    while (x_rad < -M_PI) x_rad += 2.0 * M_PI;

    int sign = 1;
    double x = x_rad;

    /* Reflect [π/2, π] to [0, π/2] for better convergence */
    if (x > M_PI / 2.0) {
        x = M_PI - x;
    } else if (x < -M_PI / 2.0) {
        x = -M_PI - x;
        sign = -1;
    }

    double sum = 0.0;
    double term = x;    /* First term: x^1 / 1! */
    double x_sq = x * x;

    for (int n = 0; n < terms; ++n) {
        if (n > 0) {
            /* Each iteration: multiply by x², divide by (2n)(2n+1) */
            term *= x_sq / (double)((2 * n) * (2 * n + 1));
        }

        if (n % 2 == 0)
            sum += term;
        else
            sum -= term;
    }

    return sign * sum;
}

/* ==================================================================
 *  COS(x) — Taylor Series
 * ==================================================================
 *
 *  Same as sin, but even powers only:
 *    cos(x) = 1 - x²/2! + x⁴/4! - x⁶/6! + ...
 * ---------------------------------------------------------------- */
static double cos_taylor(double x_rad, int terms)
{
    /* Reduce to [-π, π] */
    while (x_rad > M_PI)  x_rad -= 2.0 * M_PI;
    while (x_rad < -M_PI) x_rad += 2.0 * M_PI;

    double x = x_rad;
    /* Reflect to [0, π/2] */
    if (x < 0) x = -x;
    if (x > M_PI / 2.0) {
        x = M_PI - x;
        return -cos_taylor(x, terms);  /* cos(π-x) = -cos(x) */
    }

    double sum = 0.0;
    double term = 1.0;  /* First term: x^0 / 0! = 1 */
    double x_sq = x * x;

    for (int n = 0; n < terms; ++n) {
        if (n > 0) {
            /* Multiply by x², divide by (2n-1)(2n) */
            term *= x_sq / (double)((2 * n - 1) * (2 * n));
        }

        if (n % 2 == 0)
            sum += term;
        else
            sum -= term;
    }

    return sum;
}

/* ==================================================================
 *  TAN(x)
 * ==================================================================
 *
 *  tan(x) = sin(x) / cos(x)
 *
 *  REAL-WORLD: Slope angles, gradient descent, roof pitch.
 * ================================================================== */
static double tan_custom(double x_rad, int terms)
{
    double s = sin_taylor(x_rad, terms);
    double c = cos_taylor(x_rad, terms);

    if (c == 0.0) {
        printf("    (tan is undefined — cos(x) = 0)\n");
        return 0.0;
    }
    return s / c;
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_trig_module(void)
{
    int choice;

    printf("\n  --- Trigonometry ---\n");
    printf("  1) sin(x)\n");
    printf("  2) cos(x)\n");
    printf("  3) tan(x)\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    double angles[] = {0, 30, 45, 60, 90, 180, 360};
    int num_angles = sizeof(angles) / sizeof(angles[0]);
    const char *labels[] = {"0°", "30°", "45°", "60°", "90°", "180°", "360°"};

    printf("\n");

    if (choice == 1 || choice == 4) {
        printf("  >> SIN(x) — Taylor series (10 terms)\n");
        printf("     sin(x) = x - x³/3! + x⁵/5! - ...\n\n");
        printf("     Angle   |  our_sin    |  exact      |  error\n");
        printf("     --------+-------------+-------------+-------------\n");
        for (int i = 0; i < num_angles; ++i) {
            double rad = deg_to_rad(angles[i]);
            double our = sin_taylor(rad, 10);
            /* Compare with standard library via compiler's built-in */
            double exact = (angles[i] == 0)  ? 0.0 :
                           (angles[i] == 30) ? 0.5 :
                           (angles[i] == 45) ? 0.7071067811865476 :
                           (angles[i] == 60) ? 0.8660254037844386 :
                           (angles[i] == 90) ? 1.0 :
                           (angles[i] == 180)? 0.0 :
                           (angles[i] == 360)? 0.0 : 0.0;
            printf("     %-6s  |  %.10f  |  %.10f  |  %+.2e\n",
                   labels[i], our, exact, our - exact);
        }
        printf("\n     Lesson: Taylor series converges near 0.\n");
        printf("     Argument reduction keeps x in [-π/2, π/2] for accuracy.\n\n");
    }

    if (choice == 2 || choice == 4) {
        printf("  >> COS(x) — Taylor series (10 terms)\n");
        printf("     cos(x) = 1 - x²/2! + x⁴/4! - ...\n\n");
        printf("     Angle   |  our_cos    |  exact      |  error\n");
        printf("     --------+-------------+-------------+-------------\n");
        for (int i = 0; i < num_angles; ++i) {
            double rad = deg_to_rad(angles[i]);
            double our = cos_taylor(rad, 10);
            double exact = (angles[i] == 0)  ? 1.0 :
                           (angles[i] == 30) ? 0.8660254037844386 :
                           (angles[i] == 45) ? 0.7071067811865476 :
                           (angles[i] == 60) ? 0.5 :
                           (angles[i] == 90) ? 0.0 :
                           (angles[i] == 180)? -1.0 :
                           (angles[i] == 360)? 1.0 : 0.0;
            printf("     %-6s  |  %.10f  |  %.10f  |  %+.2e\n",
                   labels[i], our, exact, our - exact);
        }
        printf("\n     Lesson: cos(x) = sin(x + π/2).\n");
        printf("     Even function: only even powers in series.\n\n");
    }

    if (choice == 3 || choice == 4) {
        printf("  >> TAN(x) = sin(x)/cos(x)\n\n");
        printf("     Angle   |  our_tan    |  exact      |  error\n");
        printf("     --------+-------------+-------------+-------------\n");
        for (int i = 0; i < num_angles; ++i) {
            double rad = deg_to_rad(angles[i]);
            double our = tan_custom(rad, 10);
            double exact = (angles[i] == 0)  ? 0.0 :
                           (angles[i] == 30) ? 0.5773502691896257 :
                           (angles[i] == 45) ? 1.0 :
                           (angles[i] == 60) ? 1.7320508075688772 :
                           (angles[i] == 90) ? 0.0 :  /* undefined */
                           (angles[i] == 180)? 0.0 :
                           (angles[i] == 360)? 0.0 : 0.0;
            if (angles[i] == 90) {
                printf("     %-6s  |  undefined (cos=0)\n", labels[i]);
            } else {
                printf("     %-6s  |  %.10f  |  %.10f  |  %+.2e\n",
                       labels[i], our, exact, our - exact);
            }
        }
        printf("\n     Lesson: tan has asymptotes where cos(x)=0.\n");
        printf("     Used in: slope calculation, gradient descent.\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
