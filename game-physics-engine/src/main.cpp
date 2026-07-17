#include "physics.hpp"
#include "projectile.hpp"
#include "spring.hpp"
#include "collision.hpp"
#include "pendulum.hpp"

void clear_input() { std::cin.clear(); std::cin.ignore(10000, '\n'); }

void demo_projectile()
{
    double v0, angle;
    std::cout << "\n=== PROJECTILE MOTION ===\n";
    std::cout << "Initial velocity (m/s): "; std::cin >> v0;
    std::cout << "Launch angle (degrees): "; std::cin >> angle;
    ProjectileSim sim(v0, angle, 0.01);
    std::vector<Vec2> traj;
    std::vector<double> times;
    sim.run(traj, times);
    std::cout << "\n Time(s)     X(m)       Y(m)\n";
    std::cout << "--------------------------------\n";
    for (size_t i = 0; i < traj.size() && i < 20; i++)
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(7) << times[i] << "  "
                  << std::setw(9) << traj[i].x << "  "
                  << std::setw(9) << traj[i].y << "\n";
    if (!traj.empty())
        std::cout << "Range: " << traj.back().x << " m\n";
}

void demo_spring()
{
    double m, k, damp, x0, t_end;
    std::cout << "\n=== SPRING-MASS SYSTEM ===\n";
    std::cout << "Mass (kg): "; std::cin >> m;
    std::cout << "Spring constant (N/m): "; std::cin >> k;
    std::cout << "Damping coefficient (Ns/m): "; std::cin >> damp;
    std::cout << "Initial displacement (m): "; std::cin >> x0;
    std::cout << "Simulation time (s): "; std::cin >> t_end;
    SpringMassSim sim(m, k, damp, 0.01);
    std::vector<double> t_vals, x_vals;
    sim.run(x0, 0.0, t_end, t_vals, x_vals);
    std::cout << "\n Time(s)     X(m)\n";
    std::cout << "---------------------\n";
    for (size_t i = 0; i < t_vals.size() && i < 20; i++)
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(7) << t_vals[i] << "  "
                  << std::setw(9) << x_vals[i] << "\n";
}

void demo_collision()
{
    std::cout << "\n=== COLLISION DETECTION ===\n";
    Circle c1(Vec2(0, 0), 1.0);
    Circle c2(Vec2(1.5, 0), 1.0);
    AABB box(Vec2(-2, -2), Vec2(-1, -1));
    std::cout << "Circle 1 @ (0,0) r=1.0\n";
    std::cout << "Circle 2 @ (1.5,0) r=1.0\n";
    std::cout << "AABB: min(-2,-2) max(-1,-1)\n\n";
    if (c1.overlaps(c2))
        std::cout << "Circle-Circle: OVERLAP (resolving)\n";
    else
        std::cout << "Circle-Circle: No overlap\n";
    resolve_circle_circle(c1, c2, 1.0, 1.0);
    std::cout << "After resolution:\n";
    std::cout << "  C1: (" << c1.center.x << ", " << c1.center.y << ")\n";
    std::cout << "  C2: (" << c2.center.x << ", " << c2.center.y << ")\n";
    if (c1.overlaps_aabb(box))
        std::cout << "Circle-AABB: OVERLAP\n";
    else
        std::cout << "Circle-AABB: No overlap\n";
}

void demo_pendulum()
{
    double L, theta0, t_end;
    std::cout << "\n=== SIMPLE PENDULUM ===\n";
    std::cout << "Length (m): "; std::cin >> L;
    std::cout << "Initial angle (degrees): "; std::cin >> theta0;
    std::cout << "Simulation time (s): "; std::cin >> t_end;
    PendulumSim sim(L);
    std::vector<double> t_vals, theta_vals;
    sim.run(theta0 * PI / 180.0, 0.0, t_end, t_vals, theta_vals);
    std::cout << "\n Time(s)   Theta(rad)\n";
    std::cout << "-----------------------\n";
    for (size_t i = 0; i < t_vals.size() && i < 20; i++)
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(7) << t_vals[i] << "  "
                  << std::setw(9) << theta_vals[i] << "\n";
}

void all_demos()
{
    std::cout << "\n======== ALL DEMOS SIDE BY SIDE ========\n";
    ProjectileSim proj(30.0, 45.0, 0.01);
    std::vector<Vec2> traj;
    std::vector<double> ptimes;
    proj.run(traj, ptimes);

    SpringMassSim spring(1.0, 10.0, 0.5, 0.01);
    std::vector<double> st, sx;
    spring.run(1.0, 0.0, 2.0, st, sx);

    PendulumSim pend(2.0);
    std::vector<double> pt, pth;
    pend.run(0.2, 0.0, 2.0, pt, pth);

    std::cout << " t      Proj.X   Proj.Y    SpringX   Pend.Theta\n";
    std::cout << "--------------------------------------------------\n";
    size_t n = ptimes.size();
    if (st.size() < n) n = st.size();
    if (pt.size() < n) n = pt.size();
    if (15 < n) n = 15;
    for (size_t i = 0; i < n; i++)
    {
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(4) << ptimes[i] << "  "
                  << std::setw(7) << traj[i].x << " "
                  << std::setw(7) << traj[i].y << "  "
                  << std::setw(7) << sx[i] << "  "
                  << std::setw(9) << pth[i] << "\n";
    }
}

int main()
{
    int choice;
    do {
        std::cout << "\n===== GAME PHYSICS ENGINE =====\n";
        std::cout << "1) Projectile Motion\n";
        std::cout << "2) Spring-Mass System\n";
        std::cout << "3) Collision Detection\n";
        std::cout << "4) Simple Pendulum\n";
        std::cout << "5) All Demos Side-by-Side\n";
        std::cout << "0) Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;
        if (!std::cin) { choice = -1; clear_input(); }
        switch (choice) {
            case 1: demo_projectile(); break;
            case 2: demo_spring(); break;
            case 3: demo_collision(); break;
            case 4: demo_pendulum(); break;
            case 5: all_demos(); break;
            case 0: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid option.\n"; clear_input();
        }
    } while (choice != 0);
    return 0;
}
