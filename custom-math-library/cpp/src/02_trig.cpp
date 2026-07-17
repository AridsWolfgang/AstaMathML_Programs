#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>

double sin_taylor(double x, int t)
{
    while (x > M_PI) x -= 2*M_PI;
    while (x < -M_PI) x += 2*M_PI;
    int sign = 1;
    if (x > M_PI/2) { x = M_PI - x; }
    else if (x < -M_PI/2) { x = -M_PI - x; sign = -1; }
    double sum = 0, term = x, x2 = x*x;
    for (int n = 0; n < t; ++n) {
        if (n > 0) term *= x2 / (2*n * (2*n+1));
        sum += (n % 2 == 0) ? term : -term;
    }
    return sign * sum;
}

double cos_taylor(double x, int t) {
    return sin_taylor(x + M_PI/2, t);
}

void runTrigModule()
{
    int choice;
    std::cout << "\n  --- Trigonometry ---\n"
              << "  1) sin(x)\n  2) cos(x)\n  3) tan(x)\n  4) All\n  Choice: ";
    std::cin >> choice;

    double angles[] = {0, 30, 45, 60, 90, 180, 360};
    const char* labels[] = {"0°", "30°", "45°", "60°", "90°", "180°", "360°"};
    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        std::cout << "  >> SIN(x) — Taylor series (10 terms)\n"
                  << "     Angle  |  our_sin     |  exact       |  error\n"
                  << "     -------+-------------+-------------+----------\n";
        for (int i = 0; i < 7; ++i) {
            double rad = angles[i] * M_PI / 180;
            double our = sin_taylor(rad, 10);
            double exact = std::sin(rad);
            std::cout << "     " << std::setw(4) << labels[i] << "  |  "
                      << std::fixed << std::setprecision(8) << our << "  |  "
                      << exact << "  |  " << std::scientific << (our-exact) << "\n";
        }
        std::cout << "\n";
    }

    if (choice == 2 || choice == 4) {
        std::cout << "  >> COS(x)\n"
                  << "     Angle  |  our_cos     |  exact       |  error\n"
                  << "     -------+-------------+-------------+----------\n";
        for (int i = 0; i < 7; ++i) {
            double rad = angles[i] * M_PI / 180;
            double our = cos_taylor(rad, 10);
            double exact = std::cos(rad);
            std::cout << "     " << std::setw(4) << labels[i] << "  |  "
                      << std::fixed << std::setprecision(8) << our << "  |  "
                      << exact << "  |  " << std::scientific << (our-exact) << "\n";
        }
        std::cout << "\n";
    }

    if (choice == 3 || choice == 4) {
        std::cout << "  >> TAN(x) = sin(x)/cos(x)\n"
                  << "     Angle  |  our_tan     |  exact       |  error\n"
                  << "     -------+-------------+-------------+----------\n";
        for (int i = 0; i < 7; ++i) {
            if (angles[i] == 90) { std::cout << "     90°   |  undefined\n"; continue; }
            double rad = angles[i] * M_PI / 180;
            double our = sin_taylor(rad, 10) / cos_taylor(rad, 10);
            double exact = std::tan(rad);
            std::cout << "     " << std::setw(4) << labels[i] << "  |  "
                      << std::fixed << std::setprecision(8) << our << "  |  "
                      << exact << "  |  " << std::scientific << (our-exact) << "\n";
        }
        std::cout << "\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
