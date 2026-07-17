#include "spring.hpp"

void SpringMassSim::run(double x0, double v0, double t_end,
                        std::vector<double> &t_vals, std::vector<double> &x_vals) const
{
    t_vals.clear();
    x_vals.clear();
    double x = x0, v = v0, t = 0.0;
    t_vals.push_back(t);
    x_vals.push_back(x);
    while (t < t_end)
    {
        double a = -(k / mass) * x - (damping / mass) * v;
        v += a * dt;
        x += v * dt;
        t += dt;
        t_vals.push_back(t);
        x_vals.push_back(x);
        if (t_vals.size() > 5000) break;
    }
}
