#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cmath>
#include <array>

using Vec3 = std::array<double, 3>;

void runLinalgModule()
{
    int choice;
    std::cout << "\n  --- Linear Algebra ---\n"
              << "  1) Vectors\n  2) Matrices\n  3) 3D rotation\n  4) All\n  Choice: ";
    std::cin >> choice;
    std::cout << "\n";

    if (choice == 1 || choice == 4) {
        Vec3 a = {3, 4, 0}, b = {1, 2, 3}, r;
        auto pv = [](const char* n, const Vec3& v) {
            std::cout << "  " << n << " = [" << v[0] << ", " << v[1] << ", " << v[2] << "]\n";
        };
        pv("a", a); pv("b", b);
        for (int i = 0; i < 3; ++i) r[i] = a[i] + b[i];
        pv("a+b", r);
        double dot = 0;
        for (int i = 0; i < 3; ++i) dot += a[i] * b[i];
        std::cout << "  a·b = " << dot << "\n";
        double mag = std::sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
        std::cout << "  |a| = " << mag << "\n\n";
    }

    if (choice == 2 || choice == 4) {
        std::array<double, 9> I = {1,0,0, 0,1,0, 0,0,1};
        std::cout << "  Identity matrix:\n";
        for (int r = 0; r < 3; ++r)
            std::cout << "    [" << I[r*3] << ", " << I[r*3+1] << ", " << I[r*3+2] << "]\n";
        std::cout << "\n";
    }

    if (choice == 3 || choice == 4) {
        Vec3 pt = {1, 0, 0};
        double c = std::cos(M_PI/4), s = std::sin(M_PI/4);
        std::array<double, 9> R = {c, -s, 0, s, c, 0, 0, 0, 1};
        std::cout << "  Point = [1, 0, 0]\n";
        Vec3 res;
        for (int r = 0; r < 3; ++r)
            res[r] = R[r*3]*pt[0] + R[r*3+1]*pt[1] + R[r*3+2]*pt[2];
        std::cout << "  Rotated 45° = [" << res[0] << ", " << res[1] << ", " << res[2] << "]\n";
        std::cout << "\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
