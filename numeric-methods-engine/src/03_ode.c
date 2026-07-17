#include "../header.h"

void euler_method(ode_func f, double y0, double t0, double tf, double h, double *t_vals, double *y_vals, int *steps)
{
    int n = (int)((tf - t0) / h) + 1;
    *steps = n;
    t_vals[0] = t0;
    y_vals[0] = y0;
    for (int i = 1; i < n; i++)
    {
        t_vals[i] = t0 + i * h;
        y_vals[i] = y_vals[i - 1] + h * f(t_vals[i - 1], y_vals[i - 1]);
    }
}

void runge_kutta_4(ode_func f, double y0, double t0, double tf, double h, double *t_vals, double *y_vals, int *steps)
{
    int n = (int)((tf - t0) / h) + 1;
    *steps = n;
    t_vals[0] = t0;
    y_vals[0] = y0;
    for (int i = 1; i < n; i++)
    {
        double t = t_vals[i - 1];
        double y = y_vals[i - 1];
        double k1 = h * f(t, y);
        double k2 = h * f(t + h / 2.0, y + k1 / 2.0);
        double k3 = h * f(t + h / 2.0, y + k2 / 2.0);
        double k4 = h * f(t + h, y + k3);
        t_vals[i] = t + h;
        y_vals[i] = y + (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
    }
}
