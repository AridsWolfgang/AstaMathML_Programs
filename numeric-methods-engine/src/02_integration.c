#include "../header.h"

double rectangle_rule(fn_double f, double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += f(a + i * h + h / 2.0);
    return sum * h;
}

double trapezoidal_rule(fn_double f, double a, double b, int n)
{
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < n; i++)
        sum += f(a + i * h);
    return sum * h;
}

double simpson_rule(fn_double f, double a, double b, int n)
{
    if (n % 2 != 0) n++;
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += (i % 2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
    }
    return sum * h / 3.0;
}
