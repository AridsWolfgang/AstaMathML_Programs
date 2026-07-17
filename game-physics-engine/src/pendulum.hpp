#ifndef PENDULUM_HPP
#define PENDULUM_HPP

#include "physics.hpp"

struct PendulumSim {
    double L, g0, dt;
    PendulumSim(double len, double g_ = G, double dt_ = 0.01)
        : L(len), g0(g_), dt(dt_) {}
    void run(double theta0, double omega0, double t_end,
             std::vector<double> &t_vals,
             std::vector<double> &theta_vals) const;
};

#endif
