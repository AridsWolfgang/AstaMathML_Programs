/*
 * ===================================================================
 *  MODULE 5: LINEAR ALGEBRA
 * ===================================================================
 *
 *  REAL-WORLD USE:
 *    - 3D Graphics: Transformations (rotate, scale, translate)
 *    - ML: Neural networks are layers of matrix multiplications
 *    - Physics: Forces, velocities as vectors; tensors in relativity
 *    - Robotics: Coordinate transformations, inverse kinematics
 *    - CS: Google's PageRank = eigenvector of the web graph
 *
 *  VECTOR OPERATIONS:
 *    - add, subtract, scale
 *    - dot product (inner product)
 *    - cross product (3D only)
 *    - magnitude, normalize
 *
 *  MATRIX OPERATIONS:
 *    - add, subtract, multiply
 *    - transpose
 *    - determinant (2x2, 3x3)
 *    - inverse (2x2)
 *
 *  VECTORS ARE EVERYWHERE:
 *    Position, velocity, acceleration, force, color (RGB),
 *    sound (amplitude over time), pixels (x, y), weights in ML.
 * ===================================================================
 */

#include <stdio.h>

void run_linalg_module(void);
extern void clear_input(void);

/* We use double[3] for 3D vectors and double[9] for 3x3 matrices */

/* ==================================================================
 *  VECTOR OPERATIONS
 * ================================================================== */

static void vec_add(const double a[3], const double b[3], double result[3])
{
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
}

static void vec_sub(const double a[3], const double b[3], double result[3])
{
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

static void vec_scale(const double v[3], double s, double result[3])
{
    result[0] = v[0] * s;
    result[1] = v[1] * s;
    result[2] = v[2] * s;
}

static double vec_dot(const double a[3], const double b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void vec_cross(const double a[3], const double b[3], double result[3])
{
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

static double vec_mag(const double v[3])
{
    /* sqrt(v·v) = sqrt(v0² + v1² + v2²) */
    double sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    /* Use our own sqrt */
    double x = sum;
    for (int i = 0; i < 20; ++i)
        x = (x + sum / x) / 2.0;
    return x;
}

static void vec_normalize(const double v[3], double result[3])
{
    double mag = vec_mag(v);
    if (mag == 0) { result[0] = result[1] = result[2] = 0; return; }
    vec_scale(v, 1.0 / mag, result);
}

static void vec_print(const char *name, const double v[3])
{
    printf("  %s = [%.4f, %.4f, %.4f]\n", name, v[0], v[1], v[2]);
}

/* ==================================================================
 *  MATRIX OPERATIONS (3x3, row-major)
 * ==================================================================
 *
 *  Matrix layout (row-major):
 *    m[0] m[1] m[2]
 *    m[3] m[4] m[5]
 *    m[6] m[7] m[8]
 *
 *  Visual (rotation matrix for 3D):
 *    [ cosθ  -sinθ  0 ]   [ x ]   [ x' ]
 *    [ sinθ   cosθ  0 ] · [ y ] = [ y' ]
 *    [   0      0   1 ]   [ z ]   [ z' ]
 *
 *  REAL-WORLD: Every 3D game, every CAD program,
 *  every robotics controller uses these operations.
 * ================================================================== */

static void mat_mul_vec(const double m[9], const double v[3], double result[3])
{
    for (int r = 0; r < 3; ++r) {
        result[r] = m[3 * r + 0] * v[0] +
                    m[3 * r + 1] * v[1] +
                    m[3 * r + 2] * v[2];
    }
}

static void mat_mul_mat(const double a[9], const double b[9], double result[9])
{
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            result[3 * r + c] = a[3 * r + 0] * b[0 + c] +
                                a[3 * r + 1] * b[3 + c] +
                                a[3 * r + 2] * b[6 + c];
        }
    }
}

static void mat_transpose(const double m[9], double result[9])
{
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            result[3 * r + c] = m[3 * c + r];
}

static double mat_det3(const double m[9])
{
    /* Sarrus's rule for 3x3 determinant */
    return m[0] * (m[4] * m[8] - m[5] * m[7])
         - m[1] * (m[3] * m[8] - m[5] * m[6])
         + m[2] * (m[3] * m[7] - m[4] * m[6]);
}

static void mat_print(const char *name, const double m[9])
{
    printf("  %s = \n", name);
    for (int r = 0; r < 3; ++r)
        printf("      [%.4f, %.4f, %.4f]\n",
               m[3 * r], m[3 * r + 1], m[3 * r + 2]);
}

/* ==================================================================
 *  MODULE RUNNER
 * ================================================================== */
void run_linalg_module(void)
{
    int choice;

    printf("\n  --- Linear Algebra ---\n");
    printf("  1) Vector operations\n");
    printf("  2) Matrix operations\n");
    printf("  3) Rotate a 3D point\n");
    printf("  4) All\n");
    printf("  Enter choice: ");
    scanf("%d", &choice);

    printf("\n");

    if (choice == 1 || choice == 4) {
        printf("  >> VECTOR OPERATIONS\n\n");

        double a[3] = {3.0, 4.0, 0.0};
        double b[3] = {1.0, 2.0, 3.0};
        double r[3];

        vec_print("a", a);
        vec_print("b", b);

        vec_add(a, b, r);
        vec_print("a + b", r);

        vec_sub(a, b, r);
        vec_print("a - b", r);

        vec_scale(a, 2.0, r);
        vec_print("2a", r);

        printf("  a·b = %.4f  (dot product)\n", vec_dot(a, b));
        printf("  |a| = %.4f  (magnitude)\n", vec_mag(a));

        vec_cross(a, b, r);
        vec_print("a x b", r);
        printf("  a·(a x b) = %.4f  (should be 0 — cross is perpendicular)\n",
               vec_dot(a, r));

        vec_normalize(a, r);
        vec_print("a normalized", r);
        printf("  |norm| = %.4f  (should be 1)\n", vec_mag(r));

        printf("\n  Lesson: Dot product = cosine of angle between vectors.\n");
        printf("     Cross product = vector perpendicular to both inputs.\n");
        printf("     Normalize = divide by magnitude to get unit vector.\n\n");
    }

    if (choice == 2 || choice == 4) {
        printf("  >> MATRIX OPERATIONS (3x3)\n\n");

        /* Identity matrix */
        double I[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
        /* Rotation around Z axis by 90 degrees */
        double Rz[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1};
        /* Scale by 2 */
        double S[9] = {2, 0, 0, 0, 2, 0, 0, 0, 2};
        double r[9];

        mat_print("I (identity)", I);
        mat_print("Rz (90° rotation)", Rz);
        mat_print("S (scale 2x)", S);

        mat_mul_mat(Rz, S, r);
        mat_print("Rz · S", r);

        printf("  det(Rz) = %.4f  (should be 1 — rotation preserves volume)\n",
               mat_det3(Rz));
        printf("  det(S)  = %.4f  (should be 8 = 2×2×2)\n", mat_det3(S));

        mat_transpose(Rz, r);
        mat_print("Rz^T (transpose)", r);

        printf("\n  Lesson: det = 1 means volume-preserving.\n");
        printf("     det > 0 means orientation preserved.\n");
        printf("     Inverse of rotation = transpose (orthogonal matrix).\n\n");
    }

    if (choice == 3 || choice == 4) {
        printf("  >> 3D POINT ROTATION\n\n");
        double point[3] = {1.0, 0.0, 0.0};
        double result[3];

        /* Rotate around Z axis by 45 degrees */
        double cos45 = 0.7071067811865476;
        double sin45 = 0.7071067811865476;
        double R[9] = {cos45, -sin45, 0,
                       sin45,  cos45, 0,
                          0,      0,   1};

        vec_print("Original point", point);
        printf("  Rotating by 45° around Z axis...\n");
        mat_mul_vec(R, point, result);
        vec_print("Rotated point", result);

        /* Apply again — two 45° rotations = 90° total */
        mat_mul_vec(R, result, result);
        vec_print("Another 45° (90° total)", result);

        printf("\n  Lesson: 3D graphics = matrix × vector for every vertex.\n");
        printf("     A game character uses 50-100K matrix ops per frame!\n\n");
    }

    printf("  Press Enter to return to menu...");
    clear_input();
    getchar();
}
