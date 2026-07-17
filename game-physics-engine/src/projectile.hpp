#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "physics.hpp"

struct ProjectileSim {
    double v0, angle_deg, dt, drag_coeff;
    ProjectileSim(double v, double a, double d, double c = 0.01)
        : v0(v), angle_deg(a), dt(d), drag_coeff(c) {}
    void run(std::vector<Vec2> &trajectory, std::vector<double> &times) const;
};

#endif
