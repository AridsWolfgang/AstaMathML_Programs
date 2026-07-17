#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-7
#define MAX_ITER 1000

/* 01_roots.c */
typedef double (*fn_double)(double);
double bisection(fn_double f, double a, double b, int *iters);
double newton_raphson(fn_double f, fn_double df, double x0, int *iters);
double secant_method(fn_double f, double x0, double x1, int *iters);

/* 02_integration.c */
double rectangle_rule(fn_double f, double a, double b, int n);
double trapezoidal_rule(fn_double f, double a, double b, int n);
double simpson_rule(fn_double f, double a, double b, int n);

/* 03_ode.c */
typedef double (*ode_func)(double t, double y);
void euler_method(ode_func f, double y0, double t0, double tf, double h, double *t_vals, double *y_vals, int *steps);
void runge_kutta_4(ode_func f, double y0, double t0, double tf, double h, double *t_vals, double *y_vals, int *steps);

/* 04_linsys.c */
int gauss_elimination(int n, double *a, double *b, double *x);
int gauss_seidel(int n, double *a, double *b, double *x, int max_iter, double tol);

/* utils */
void clear_input(void);

#endif
