#include "../header.h"

void clear_input(void) { while (getchar() != '\n'); }

static double demo_f(double x) { return x * x * x - 4.0 * x + 1.0; }
static double demo_df(double x) { return 3.0 * x * x - 4.0; }
static double demo_int_f(double x) { return x * x; }
static double demo_ode_f(double t, double y) { (void)t; return -2.0 * y; }

void menu_roots(void)
{
    printf("\n=== ROOT FINDING ===\n");
    printf("Function: f(x) = x^3 - 4x + 1\n");
    int iters;
    double r;
    r = bisection(demo_f, 0.0, 1.0, &iters);
    printf("Bisection:       root = %8.6lf  (iters=%d)\n", r, iters);
    r = newton_raphson(demo_f, demo_df, 0.5, &iters);
    printf("Newton-Raphson:  root = %8.6lf  (iters=%d)\n", r, iters);
    r = secant_method(demo_f, 0.0, 1.0, &iters);
    printf("Secant:          root = %8.6lf  (iters=%d)\n", r, iters);
}

void menu_integration(void)
{
    printf("\n=== NUMERICAL INTEGRATION ===\n");
    printf("Function: f(x) = x^2  from 0 to 1\n");
    int n = 100;
    double exact = 1.0 / 3.0;
    double r = rectangle_rule(demo_int_f, 0.0, 1.0, n);
    printf("Rectangle:   %8.6lf  (err=%8.2e)\n", r, fabs(r - exact));
    r = trapezoidal_rule(demo_int_f, 0.0, 1.0, n);
    printf("Trapezoidal: %8.6lf  (err=%8.2e)\n", r, fabs(r - exact));
    r = simpson_rule(demo_int_f, 0.0, 1.0, n);
    printf("Simpson:     %8.6lf  (err=%8.2e)\n", r, fabs(r - exact));
    printf("Exact:       %8.6lf\n", exact);
}

void menu_ode(void)
{
    printf("\n=== ODE SOLVING ===\n");
    printf("dy/dt = -2y,  y(0)=1,  t=[0,2]\n");
    double t_vals[1000], y_vals[1000];
    int steps;
    double h = 0.1;
    printf("\n  t      Euler      RK4        Exact\n");
    euler_method(demo_ode_f, 1.0, 0.0, 2.0, h, t_vals, y_vals, &steps);
    double *t_rk4 = malloc(steps * sizeof(double));
    double *y_rk4 = malloc(steps * sizeof(double));
    int steps_rk4;
    runge_kutta_4(demo_ode_f, 1.0, 0.0, 2.0, h, t_rk4, y_rk4, &steps_rk4);
    for (int i = 0; i < steps && i < 15; i++)
    {
        double exact = exp(-2.0 * t_vals[i]);
        printf("%4.1f  %9.6f  %9.6f  %9.6f\n", t_vals[i], y_vals[i], y_rk4[i], exact);
    }
    free(t_rk4); free(y_rk4);
}

void menu_linsys(void)
{
    printf("\n=== LINEAR SYSTEM SOLVING ===\n");
    int n = 3;
    double A[9] = {4.0, -1.0, 0.0, -1.0, 4.0, -1.0, 0.0, -1.0, 4.0};
    double b[3] = {5.0, 5.0, 5.0};
    double x[3];
    double A2[9];
    memcpy(A2, A, 9 * sizeof(double));
    double b2[3]; memcpy(b2, b, 3 * sizeof(double));
    if (gauss_elimination(n, A, b, x))
    {
        printf("Gauss elimination:\n");
        for (int i = 0; i < n; i++) printf("  x%d = %10.6lf\n", i + 1, x[i]);
    }
    else printf("Gauss elimination failed (singular?)\n");
    double x2[3];
    if (gauss_seidel(n, A2, b2, x2, 100, 1e-6))
    {
        printf("Gauss-Seidel:\n");
        for (int i = 0; i < n; i++) printf("  x%d = %10.6lf\n", i + 1, x2[i]);
    }
    else printf("Gauss-Seidel did not converge\n");
}

void menu_compare(void)
{
    printf("\n=== COMPARE ALL METHODS ===\n");
    printf("Comparing root-finding speed\n\n");
    int iters_b, iters_n, iters_s;
    bisection(demo_f, 0.0, 1.0, &iters_b);
    newton_raphson(demo_f, demo_df, 0.5, &iters_n);
    secant_method(demo_f, 0.0, 1.0, &iters_s);
    printf("Method            Iters\n");
    printf("------------------------\n");
    printf("Bisection         %3d\n", iters_b);
    printf("Newton-Raphson    %3d\n", iters_n);
    printf("Secant            %3d\n", iters_s);
    printf("\nComparing integration accuracy (n=100)\n\n");
    double exact = 1.0 / 3.0;
    double rect_err = fabs(rectangle_rule(demo_int_f, 0.0, 1.0, 100) - exact);
    double trap_err = fabs(trapezoidal_rule(demo_int_f, 0.0, 1.0, 100) - exact);
    double simp_err = fabs(simpson_rule(demo_int_f, 0.0, 1.0, 100) - exact);
    printf("Method            Error\n");
    printf("------------------------\n");
    printf("Rectangle         %.2e\n", rect_err);
    printf("Trapezoidal       %.2e\n", trap_err);
    printf("Simpson           %.2e\n", simp_err);
}

int main(void)
{
    int choice;
    do {
        printf("\n===== NUMERIC METHODS ENGINE =====\n");
        printf("1) Root Finding\n");
        printf("2) Numerical Integration\n");
        printf("3) ODE Solving\n");
        printf("4) Linear System Solving\n");
        printf("5) Compare All Methods\n");
        printf("0) Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { choice = -1; clear_input(); }
        switch (choice) {
            case 1: menu_roots(); break;
            case 2: menu_integration(); break;
            case 3: menu_ode(); break;
            case 4: menu_linsys(); break;
            case 5: menu_compare(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid option.\n"); clear_input();
        }
    } while (choice != 0);
    return 0;
}
