#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "physics.hpp"

struct AABB {
    Vec2 min, max;
    AABB(Vec2 mn = Vec2(), Vec2 mx = Vec2()) : min(mn), max(mx) {}
    bool overlaps(const AABB &other) const;
    Vec2 get_center() const { return (min + max) * 0.5; }
};

struct Circle {
    Vec2 center;
    double radius;
    Circle(Vec2 c = Vec2(), double r = 1.0) : center(c), radius(r) {}
    bool overlaps(const Circle &other) const;
    bool overlaps_aabb(const AABB &box) const;
};

void resolve_circle_circle(Circle &a, Circle &b, double ma, double mb);
void resolve_circle_aabb(Circle &c, double m, AABB &box);

#endif
