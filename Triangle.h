#pragma once

#include "Vec3.h"

namespace rabbit
{
    struct Triangle
    {
        Point P0;
        Point P1;
        Point P2;
        Vec3 P0_P1; ///< P0 to P1 Vector
        Vec3 P0_P2; ///< P0 to P2 Vector;
        Vec3 P1_P2; ///< P1 to P2 Vector
        Vec3 Normal; ///< Normal to the Triangle's plane
        float Area;
        bool IsDegenerate;

        bool IsPointWithinExtrudedTriangle(const Point& p)const;

        std::pair<Point,float> ProjectPointOntoTrianglePlane(const Point& p) const;

        std::pair<float,float> CalcBarycentricCoords(const Point& p)const;

        Point CalcPointFromBarycentricCoords(const float u, const float v) const;

        Triangle(const Point& p0, const Point& p1, const Point& p2);
    };
}
