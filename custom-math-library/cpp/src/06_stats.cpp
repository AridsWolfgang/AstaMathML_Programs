#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

void runStatsModule()
{
    int choice;
    std::cout << "\n  --- Statistics ---\n"
              << "  1) Mean & Median\n  2) Variance & StdDev\n"
              << "  3) Correlation\n  4) All\n  Choice: ";
    std::cin >> choice;
    std::cout << "\n";

    double scores[] = {85, 92, 78, 90, 88, 76, 95, 89, 84, 91, 73, 87};
    int n = 12;

    if (choice == 1 || choice == 4) {
        double sum = std::accumulate(scores, scores+n, 0.0);
        double mu = sum / n;
        std::vector<double> s(scores, scores+n);
        std::sort(s.begin(), s.end());
        double med = (n % 2) ? s[n/2] : (s[n/2-1] + s[n/2]) / 2;
        std::cout << "  Mean   = " << mu << "\n  Median = " << med << "\n\n";
    }

    if (choice == 2 || choice == 4) {
        double mu = std::accumulate(scores, scores+n, 0.0) / n;
        double var = 0;
        for (double x : scores) var += (x-mu)*(x-mu);
        var /= n;
        double sd = std::sqrt(var);
        std::cout << "  Variance = " << var << "\n  StdDev   = " << sd
                  << "\n  68% in [" << mu-sd << ", " << mu+sd << "]\n\n";
    }

    if (choice == 3 || choice == 4) {
        double hours[] = {2,3,5,4,6,1,7,5,3,6,2,4};
        double s2[] = {72,78,88,85,92,65,95,90,80,91,70,82};
        double mx = std::accumulate(hours, hours+n, 0.0)/n;
        double my = std::accumulate(s2, s2+n, 0.0)/n;
        double cov = 0, vx = 0, vy = 0;
        for (int i = 0; i < n; ++i) {
            cov += (hours[i]-mx)*(s2[i]-my);
            vx += (hours[i]-mx)*(hours[i]-mx);
            vy += (s2[i]-my)*(s2[i]-my);
        }
        double r = cov / std::sqrt(vx*vy);
        std::cout << "  Pearson's r = " << r << " (strong correlation)\n\n";
    }

    std::cin.ignore(10000, '\n');
    std::cout << "  Press Enter...";
    std::cin.get();
}
