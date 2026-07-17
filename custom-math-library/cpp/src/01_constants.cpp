#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>

void runConstantsModule()
{
    int choice;
    std::cout << "\n  --- Constants ---\n"
              << "  1) Pi (slow: Leibniz)\n"
              << "  2) Pi (fast: Machin)\n"
              << "  3) e (Taylor)\n"
              << "  4) Phi (continued fraction)\n"
              << "  5) All\n  Choice: ";
    std::cin >> choice;
    std::cout << "\n";

    if (choice == 1 || choice == 5) {
        std::cout << "  >> PI (Leibniz series)\n"
                  << "     π = 4 × (1 - 1/3 + 1/5 - 1/7 + ...)\n\n";
        int terms[] = {10, 100, 1000, 10000, 100000};
        for (int t : terms) {
            double sum = 0;
            for (int n = 0; n < t; ++n) {
                double term = 1.0 / (2 * n + 1);
                sum += (n % 2 == 0) ? term : -term;
            }
            double pi = 4 * sum;
            std::cout << "     " << std::setw(6) << t << " terms: "
                      << std::fixed << std::setprecision(12) << pi
                      << "  error: " << std::scientific << (pi - M_PI) << "\n";
        }
        std::cout << "     Lesson: Leiniz converges VERY slowly.\n\n";
    }

    if (choice == 2 || choice == 5) {
        std::cout << "  >> PI (Machin's formula)\n"
                  << "     π/4 = 4·arctan(1/5) - arctan(1/239)\n\n";
        for (int t = 2; t <= 12; t += 2) {
            auto arctan = [](double x, int n) {
                double sum = 0, xp = x;
                for (int i = 0; i < n; ++i) {
                    double term = xp / (2 * i + 1);
                    sum += (i % 2 == 0) ? term : -term;
                    xp *= x * x;
                }
                return sum;
            };
            double pi = 4 * (4 * arctan(1.0/5, t) - arctan(1.0/239, t));
            std::cout << "     " << t << " terms: "
                      << std::fixed << std::setprecision(12) << pi
                      << "  error: " << std::scientific << (pi - M_PI) << "\n";
        }
        std::cout << "     Lesson: 10 terms ~ double precision!\n\n";
    }

    if (choice == 3 || choice == 5) {
        std::cout << "  >> EULER's e\n"
                  << "     e = 1 + 1/1! + 1/2! + 1/3! + ...\n\n";
        for (int t = 2; t <= 20; t += 2) {
            double sum = 1, fact = 1;
            for (int n = 1; n <= t; ++n) {
                fact *= n;
                sum += 1.0 / fact;
            }
            std::cout << "     " << t << " terms: "
                      << std::fixed << std::setprecision(12) << sum
                      << "  error: " << std::scientific << (sum - std::exp(1)) << "\n";
        }
        std::cout << "     Lesson: ~15 terms = full precision.\n\n";
    }

    if (choice == 4 || choice == 5) {
        std::cout << "  >> PHI (continued fraction)\n"
                  << "     φ = 1 + 1/(1 + 1/(1 + ...))\n\n";
        for (int t = 1; t <= 10; ++t) {
            double phi = 1;
            for (int i = 0; i < t; ++i) phi = 1 + 1/phi;
            double exact = (1 + std::sqrt(5)) / 2;
            std::cout << "     " << t << " iters: "
                      << std::fixed << std::setprecision(12) << phi
                      << "  error: " << std::scientific << (phi - exact) << "\n";
        }
        std::cout << "     Lesson: Continued fractions converge fast.\n\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
