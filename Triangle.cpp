#include "Triangle.h"
#include <memory>

using namespace rabbit;


Triangle::Triangle(const Point& p0, const Point& p1, const Point& p2)
:P0(p0),
 P1(p1),
 P2(p2),
 P0_P1(p1-p0),
 P0_P2(p2-p0),
 P1_P2(p2-p1),
 Normal((Vec3::crossProduct(P0_P1,P0_P2)).normalise())
{

}

bool Triangle::IsPointWithinExtrudedTriangle(const Point& p)
{
    float u,v;
    std::tie(u,v) = CalcBarycentricCoords(p);
    const float TOL = 0.01f;
    return ( u >= -TOL && v >= -TOL && u+v <= 1.0f + TOL);
}

std::pair<Point,float> Triangle::ProjectPointOntoTrianglePlane(const Point& p) const
{
    const auto p_P0 = P0-p;
    auto signedDist = Vec3::dotProduct(p_P0, Normal);
    return std::make_pair(Point(p + Normal*signedDist), -signedDist);
}

std::pair<float,float> Triangle::CalcBarycentricCoords(const Point& P)const
{
    // Assume P = P0 + u*P0_P1 + v*P0_P2, goal is to calculate u and v
    // Project this equation along P0_P1 and P0_P2 and we obtain a set
    // of equations which can then be solved.
    //  |x| = |a, b||u|
    //  |y| = |c, d||v|
    //
    const auto P0_P = P - P0;
    const auto x = Vec3::dotProduct(P0_P1,P0_P);
    const auto y = Vec3::dotProduct(P0_P2,P0_P);

    const auto a = Vec3::dotProduct(P0_P1,P0_P1);
    const auto b = Vec3::dotProduct(P0_P1,P0_P2);
    const auto c = Vec3::dotProduct(P0_P2,P0_P1);
    const auto d = Vec3::dotProduct(P0_P2,P0_P2);

    const auto inverseDet = 1.0f/(a*d - c*b);
    const auto u = inverseDet * (d*x - b*y);
    const auto v = inverseDet * (-c*x + a*y);
    return std::make_pair(u,v);
}
