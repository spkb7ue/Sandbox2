#pragma once

#include "Vec3.h"

namespace rabbit
{
    struct Triangle
    {
        Point P0;
        Point P1;
        Point P2;
        Vec3 P0_P1; ///< This is P0 to P1 vector
        Vec3 P0_P2; ///< This is the P0 to P2 vector;
        Vec3 Normal; ///< Normal to the Triangle's plane

        bool IsPointWithinExtrudedTriangle(const Point& p);

        Triangle(const Point& p0, const Point& p1, const Point& p2);
    };
}
