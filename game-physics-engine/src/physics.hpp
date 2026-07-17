#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <string>
#include <sstream>

struct Vec2 {
    double x, y;
    Vec2(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
    Vec2 operator+(const Vec2 &o) const { return Vec2(x + o.x, y + o.y); }
    Vec2 operator-(const Vec2 &o) const { return Vec2(x - o.x, y - o.y); }
    Vec2 operator*(double s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(double s) const { return Vec2(x / s, y / s); }
    double dot(const Vec2 &o) const { return x * o.x + y * o.y; }
    double len() const { return std::sqrt(x * x + y * y); }
    Vec2 normalized() const { double l = len(); return l > 0 ? *this / l : Vec2(); }
};

struct RigidBody {
    Vec2 pos, vel;
    double mass;
    RigidBody() : pos(), vel(), mass(1.0) {}
    RigidBody(Vec2 p, Vec2 v, double m) : pos(p), vel(v), mass(m) {}
};

constexpr double G = 9.81;
constexpr double PI = 3.14159265358979323846;

#endif
