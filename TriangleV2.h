#pragma once

#include "Vec3.h"

namespace rabbit
{
struct TriangleV2
{
    Point P0;
    Point P1;
    Point P2;
    Vec3 P0_P1; ///< P0 to P1 Vector
    Vec3 P0_P2; ///< P0 to P2 Vector;
    Vec3 P1_P2; ///< P1 to P2 Vector
    Vec3 Normal; ///< Normal to the Triangle's plane

    bool IsPointWithinExtrudedTriangle(const Point& p)const;

    std::pair<Point,double> ProjectPointOntoTrianglePlane(const Point& p) const;

    std::pair<double,double> CalcBarycentricCoords(const Point& p)const;

    std::pair<Point, double> CalcShortestDistanceFrom(const Point&)const;

    // Debug functions
    Point CalcPointFromBarycentricCoords(const double u, const double v) const;
    Point CalcPointFromBarycentricCoords(std::pair<double,double> coords)const;
    std::tuple<Point, double, bool> CheckPointSegDist(const Vec3& origin,
                                                      const Vec3& seg,
                                                      const Vec3& P)const;

    TriangleV2(const Point& p0, const Point& p1, const Point& p2);
};
}
