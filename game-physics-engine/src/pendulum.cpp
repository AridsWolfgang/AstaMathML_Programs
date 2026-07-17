#include "pendulum.hpp"

static void pendulum_deriv(double theta, double omega, double L, double g0,
                           double &dtheta, double &domega)
{
    dtheta = omega;
    domega = -(g0 / L) * std::sin(theta);
}

void PendulumSim::run(double theta0, double omega0, double t_end,
                      std::vector<double> &t_vals,
                      std::vector<double> &theta_vals) const
{
    t_vals.clear();
    theta_vals.clear();
    double t = 0.0, theta = theta0, omega = omega0;
    t_vals.push_back(t);
    theta_vals.push_back(theta);
    while (t < t_end)
    {
        double k1_th, k1_om;
        pendulum_deriv(theta, omega, L, g0, k1_th, k1_om);
        double k2_th, k2_om;
        pendulum_deriv(theta + 0.5 * dt * k1_th, omega + 0.5 * dt * k1_om, L, g0, k2_th, k2_om);
        double k3_th, k3_om;
        pendulum_deriv(theta + 0.5 * dt * k2_th, omega + 0.5 * dt * k2_om, L, g0, k3_th, k3_om);
        double k4_th, k4_om;
        pendulum_deriv(theta + dt * k3_th, omega + dt * k3_om, L, g0, k4_th, k4_om);
        theta += (dt / 6.0) * (k1_th + 2.0 * k2_th + 2.0 * k3_th + k4_th);
        omega += (dt / 6.0) * (k1_om + 2.0 * k2_om + 2.0 * k3_om + k4_om);
        t += dt;
        t_vals.push_back(t);
        theta_vals.push_back(theta);
        if (t_vals.size() > 5000) break;
    }
}
