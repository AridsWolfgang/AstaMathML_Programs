#include "../header.h"

int gauss_elimination(int n, double *a, double *b, double *x)
{
    for (int i = 0; i < n; i++)
    {
        int max_row = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(a[k * n + i]) > fabs(a[max_row * n + i]))
                max_row = k;
        if (fabs(a[max_row * n + i]) < 1e-12) return 0;
        if (max_row != i)
        {
            for (int j = 0; j < n; j++)
            {
                double tmp = a[i * n + j];
                a[i * n + j] = a[max_row * n + j];
                a[max_row * n + j] = tmp;
            }
            double tmp = b[i];
            b[i] = b[max_row];
            b[max_row] = tmp;
        }
        for (int k = i + 1; k < n; k++)
        {
            double factor = a[k * n + i] / a[i * n + i];
            for (int j = i; j < n; j++)
                a[k * n + j] -= factor * a[i * n + j];
            b[k] -= factor * b[i];
        }
    }
    for (int i = n - 1; i >= 0; i--)
    {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++)
            x[i] -= a[i * n + j] * x[j];
        x[i] /= a[i * n + i];
    }
    return 1;
}

int gauss_seidel(int n, double *a, double *b, double *x, int max_iter, double tol)
{
    for (int i = 0; i < n; i++) x[i] = 0.0;
    for (int iter = 0; iter < max_iter; iter++)
    {
        double max_err = 0.0;
        for (int i = 0; i < n; i++)
        {
            double sum = b[i];
            for (int j = 0; j < n; j++)
            {
                if (i != j) sum -= a[i * n + j] * x[j];
            }
            double x_new = sum / a[i * n + i];
            max_err = fmax(max_err, fabs(x_new - x[i]));
            x[i] = x_new;
        }
        if (max_err < tol) return 1;
    }
    return 0;
}
