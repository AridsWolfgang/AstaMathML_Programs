#ifndef SPRING_HPP
#define SPRING_HPP

#include "physics.hpp"

struct SpringMassSim {
    double mass, k, damping, dt;
    SpringMassSim(double m, double k_, double damp, double dt_)
        : mass(m), k(k_), damping(damp), dt(dt_) {}
    void run(double x0, double v0, double t_end,
             std::vector<double> &t_vals, std::vector<double> &x_vals) const;
};

#endif
