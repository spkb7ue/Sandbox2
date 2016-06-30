#pragma once

#include "Point.h"

namespace rabbit
{
    struct Triangle
    {
        Point P0;
        Point P1;
        Point P2;
        Triangle(const Point& p0, const Point& p1, const Point& p2):
            P0(p0),P1(p1),P2(p2){}
    };
}
