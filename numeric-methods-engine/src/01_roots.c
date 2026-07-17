#include "../header.h"

double bisection(fn_double f, double a, double b, int *iters)
{
    double c;
    *iters = 0;
    if (f(a) * f(b) >= 0) return NAN;
    for (int i = 0; i < MAX_ITER; i++)
    {
        c = (a + b) / 2.0;
        (*iters)++;
        if (f(c) == 0.0 || (b - a) / 2.0 < EPS) return c;
        if (f(c) * f(a) < 0) b = c;
        else a = c;
    }
    return c;
}

double newton_raphson(fn_double f, fn_double df, double x0, int *iters)
{
    double x = x0;
    *iters = 0;
    for (int i = 0; i < MAX_ITER; i++)
    {
        double fx = f(x);
        double dfx = df(x);
        if (fabs(dfx) < 1e-12) return NAN;
        double x1 = x - fx / dfx;
        (*iters)++;
        if (fabs(x1 - x) < EPS) return x1;
        x = x1;
    }
    return x;
}

double secant_method(fn_double f, double x0, double x1, int *iters)
{
    double x_prev = x0, x_curr = x1;
    *iters = 0;
    for (int i = 0; i < MAX_ITER; i++)
    {
        double f_curr = f(x_curr);
        double f_prev = f(x_prev);
        if (fabs(f_curr - f_prev) < 1e-12) return NAN;
        double x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev);
        (*iters)++;
        if (fabs(x_next - x_curr) < EPS) return x_next;
        x_prev = x_curr;
        x_curr = x_next;
    }
    return x_curr;
}
