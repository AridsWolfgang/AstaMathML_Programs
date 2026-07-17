#include "collision.hpp"

bool AABB::overlaps(const AABB &other) const
{
    return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y);
}

bool Circle::overlaps(const Circle &other) const
{
    double d = (center - other.center).len();
    return d < radius + other.radius;
}

bool Circle::overlaps_aabb(const AABB &box) const
{
    double cx = center.x, cy = center.y;
    if (cx < box.min.x) cx = box.min.x;
    else if (cx > box.max.x) cx = box.max.x;
    if (cy < box.min.y) cy = box.min.y;
    else if (cy > box.max.y) cy = box.max.y;
    double dx = center.x - cx, dy = center.y - cy;
    return (dx * dx + dy * dy) < radius * radius;
}

void resolve_circle_circle(Circle &a, Circle &b, double ma, double mb)
{
    Vec2 diff = a.center - b.center;
    double d = diff.len();
    if (d == 0.0) return;
    double overlap = (a.radius + b.radius - d) * 0.5;
    Vec2 n = diff / d;
    double total_m = ma + mb;
    double ra = mb / total_m, rb = ma / total_m;
    a.center = a.center + n * (overlap * ra);
    b.center = b.center - n * (overlap * rb);
}

void resolve_circle_aabb(Circle &c, double m, AABB &box)
{
    (void)m;
    double cx = c.center.x, cy = c.center.y;
    if (cx < box.min.x) cx = box.min.x;
    else if (cx > box.max.x) cx = box.max.x;
    if (cy < box.min.y) cy = box.min.y;
    else if (cy > box.max.y) cy = box.max.y;
    Vec2 closest(cx, cy);
    Vec2 diff = c.center - closest;
    double d = diff.len();
    if (d < c.radius && d > 0)
    {
        c.center = closest + diff.normalized() * (c.radius + 0.001);
    }
}
