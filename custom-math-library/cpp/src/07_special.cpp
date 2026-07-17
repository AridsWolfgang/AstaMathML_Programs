#include <iostream>
#include <iomanip>
#include <cmath>

void runSpecialModule()
{
    int choice;
    std::cout << "\n  --- Special Functions ---\n"
              << "  1) Factorial\n  2) Fibonacci\n  3) GCD & Primes\n"
              << "  4) ML Activations\n  5) All\n  Choice: ";
    std::cin >> choice;
    std::cout << "\n";

    if (choice == 1 || choice == 5) {
        std::cout << "  >> FACTORIAL\n";
        unsigned long long f = 1;
        for (int i = 0; i <= 10; ++i) {
            if (i > 1) f *= i;
            std::cout << "     " << i << "! = " << f << "\n";
        }
        std::cout << "\n";
    }

    if (choice == 2 || choice == 5) {
        std::cout << "  >> FIBONACCI\n    ";
        unsigned long long p2=0, p1=1, c;
        for (int i = 0; i <= 20; ++i) {
            std::cout << p2 << " ";
            c = p1 + p2; p2 = p1; p1 = c;
        }
        std::cout << "\n\n";
    }

    if (choice == 3 || choice == 5) {
        auto gcd = [](int a, int b) { while (b) { int t=b; b=a%b; a=t; } return a; };
        std::cout << "  gcd(48, 18) = " << gcd(48, 18) << "\n";
        std::cout << "  gcd(1071, 462) = " << gcd(1071, 462) << "\n\n";
        std::cout << "  Primes up to 50: ";
        for (int n = 2; n <= 50; ++n) {
            bool prime = n > 1;
            for (int d = 2; d*d <= n && prime; ++d) if (n%d == 0) prime = false;
            if (prime) std::cout << n << " ";
        }
        std::cout << "\n\n";
    }

    if (choice == 4 || choice == 5) {
        auto exp_c = [](double x) {
            double s=1, t=1;
            for (int n=1; n<=15; ++n) { t*=x/n; s+=t; }
            return s;
        };
        std::cout << "  >> SIGMOID & RELU\n"
                  << "     x   |  sigmoid    |  ReLU\n"
                  << "     -----+------------+-------\n";
        for (double x : {-5.0, -2.0, -1.0, 0.0, 1.0, 2.0, 5.0}) {
            double sig = x > 0 ? 1/(1+exp_c(-x)) : exp_c(x)/(1+exp_c(x));
            double rel = x > 0 ? x : 0;
            std::cout << "     " << std::setw(3) << x << "  |  "
                      << std::fixed << std::setprecision(6) << sig << "  |  " << rel << "\n";
        }
        std::cout << "\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
