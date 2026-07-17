#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>

double exp_taylor(double x, int t)
{
    double sum = 1, term = 1;
    for (int n = 1; n <= t; ++n) {
        term *= x / n;
        sum += term;
    }
    return sum;
}

double exp_custom(double x, int t)
{
    if (x < 0) return 1.0 / exp_custom(-x, t);
    if (x > 1) { double h = exp_custom(x/2, t); return h * h; }
    return exp_taylor(x, t);
}

void runExpLogModule()
{
    int choice;
    std::cout << "\n  --- Exponentials & Logarithms ---\n"
              << "  1) exp(x)\n  2) ln(x)\n  3) log10 & pow\n  4) All\n  Choice: ";
    std::cin >> choice;
    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        double xs[] = {-2, -1, 0, 0.5, 1, 2, 3, 5};
        std::cout << "  >> EXP(x) — 15 Taylor terms\n"
                  << "     x    |  our_exp     |  exact       |  error\n"
                  << "     ------+-------------+-------------+----------\n";
        for (double x : xs) {
            double our = exp_custom(x, 15);
            double exact = std::exp(x);
            std::cout << "     " << std::setw(4) << x << "  |  "
                      << std::fixed << std::setprecision(8) << our << "  |  "
                      << exact << "  |  " << std::scientific << (our-exact) << "\n";
        }
        std::cout << "\n";
    }

    if (choice == 2 || choice == 4) {
        auto ln_custom = [](double x, int iters) {
            double y = x/2;
            for (int i = 0; i < iters; ++i) {
                double ey = exp_custom(y, 15);
                y = y - (ey - x) / ey;
            }
            return y;
        };
        double xs[] = {0.5, 1, 2, 3, 10, 100};
        std::cout << "  >> LN(x) — Newton's method\n"
                  << "     x    |  our_ln      |  exact       |  error\n"
                  << "     ------+-------------+-------------+----------\n";
        for (double x : xs) {
            double our = ln_custom(x, 10);
            double exact = std::log(x);
            std::cout << "     " << std::setw(4) << x << "  |  "
                      << std::fixed << std::setprecision(8) << our << "  |  "
                      << exact << "  |  " << std::scientific << (our-exact) << "\n";
        }
        std::cout << "\n";
    }

    if (choice == 3 || choice == 4) {
        auto ln = [](double x) { double y=x/2; for (int i=0;i<10;++i){double e=exp_custom(y,15);y-=(e-x)/e;} return y; };
        auto log10 = [&](double x){ return ln(x) / ln(10); };
        auto pow = [&](double x, double y){ return exp_custom(y * ln(x), 15); };

        std::cout << "  >> LOG10 & POW\n\n";
        std::cout << "     log10(100)   = " << log10(100) << "  (exact: 2)\n";
        std::cout << "     log10(1000)  = " << log10(1000) << " (exact: 3)\n";
        std::cout << "     pow(2, 10)   = " << pow(2, 10) << " (exact: 1024)\n";
        std::cout << "     pow(5, 3)    = " << pow(5, 3) << " (exact: 125)\n";
        std::cout << "     pow(2, 0.5)  = " << pow(2, 0.5) << " (exact: ~1.4142)\n\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
