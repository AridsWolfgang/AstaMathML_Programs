#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>

double sqrt_custom(double S)
{
    if (S <= 0) return 0;
    double x = S;
    while (true) {
        double nxt = (x + S/x) / 2;
        if (std::abs(nxt - x) < 1e-15) return nxt;
        x = nxt;
    }
}

void runRootsModule()
{
    int choice;
    std::cout << "\n  --- Roots & Powers ---\n"
              << "  1) sqrt(x)\n  2) cbrt(x)\n  3) nth_root & hypot\n  4) All\n  Choice: ";
    std::cin >> choice;
    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        double xs[] = {0, 1, 2, 4, 9, 10, 25, 100, 0.25};
        std::cout << "  >> SQRT(x) — Newton\n"
                  << "     x     |  our_sqrt    |  exact\n"
                  << "     ------+-------------+-----------\n";
        for (double x : xs)
            std::cout << "     " << std::setw(4) << x << "  |  "
                      << std::fixed << std::setprecision(10) << sqrt_custom(x) << "  |  "
                      << std::sqrt(x) << "\n";
        std::cout << "\n";
    }

    if (choice == 2 || choice == 4) {
        double xs[] = {0, 1, 8, 27, 64, 100, -8, -27};
        std::cout << "  >> CBRT(x)\n"
                  << "     x     |  our_cbrt    |  exact\n"
                  << "     ------+-------------+-----------\n";
        for (double x : xs) {
            double val = x;
            if (val == 0) { std::cout << "     " << std::setw(4) << x << "  |  0\n"; continue; }
            int neg = (val < 0); if (neg) val = -val;
            double v = val;
            while (true) {
                double nxt = (2*v + val/(v*v)) / 3;
                if (std::abs(nxt - v) < 1e-15) { v = neg ? -nxt : nxt; break; }
                v = nxt;
            }
            std::cout << "     " << std::setw(4) << x << "  |  "
                      << std::fixed << std::setprecision(10) << v << "  |  "
                      << std::cbrt(x) << "\n";
        }
        std::cout << "\n";
    }

    if (choice == 3 || choice == 4) {
        std::cout << "  >> HYPOT\n"
                  << "     hypot(3, 4) = " << sqrt_custom(3*3+4*4)
                  << "  (exact: 5)\n"
                  << "     hypot(5, 12) = " << sqrt_custom(5*5+12*12)
                  << "  (exact: 13)\n\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
