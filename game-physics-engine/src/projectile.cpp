#include "projectile.hpp"

void ProjectileSim::run(std::vector<Vec2> &trajectory, std::vector<double> &times) const
{
    trajectory.clear();
    times.clear();
    double theta = angle_deg * PI / 180.0;
    double vx = v0 * std::cos(theta);
    double vy = v0 * std::sin(theta);
    double x = 0.0, y = 0.0, t = 0.0;
    trajectory.push_back(Vec2(x, y));
    times.push_back(t);
    while (y >= 0.0)
    {
        double v = std::sqrt(vx * vx + vy * vy);
        double Fd = drag_coeff * v * v;
        double ax = -Fd * (vx / v) / 1.0;
        double ay = -G - Fd * (vy / v) / 1.0;
        if (v < 1e-12) { ax = 0; ay = -G; }
        vx += ax * dt;
        vy += ay * dt;
        x += vx * dt;
        y += vy * dt;
        t += dt;
        if (y < 0.0) y = 0.0;
        trajectory.push_back(Vec2(x, y));
        times.push_back(t);
        if (times.size() > 5000) break;
    }
}
