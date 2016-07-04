#include "TriangleV2.h"
#include <memory>
#include <iomanip>

using namespace rabbit;
using namespace std;

TriangleV2::TriangleV2(const Point& p0, const Point& p1, const Point& p2)
:P0(p0),
 P1(p1),
 P2(p2),
 P0_P1(p1-p0),
 P0_P2(p2-p0),
 P1_P2(p2-p1),
 Normal((Vec3::crossProduct(P0_P1,P0_P2)).normalise())
{

}

bool TriangleV2::IsPointWithinExtrudedTriangle(const Point& p)const
{
    double u,v;
    std::tie(u,v) = CalcBarycentricCoords(p);
    const double tol = 0.000001f;
    return ( u >= -tol && v >= -tol && u+v <= 1.0f + tol);
}

std::pair<Point,double> TriangleV2::ProjectPointOntoTrianglePlane(const Point& p) const
{
    const auto p_P0 = P0-p;
    auto signedDist = Vec3::dotProduct(p_P0, Normal);
    return std::make_pair(Point(p + Normal*signedDist), -signedDist);
}

std::pair<double,double> TriangleV2::CalcBarycentricCoords(const Point& P)const
{
    // Assume P = P0 + u*P0_P1 + v*P0_P2, goal is to calculate u and v
    // Project this equation along P0_P1 and P0_P2 and we obtain a set
    // of equations which can then be solved.
    //  |x| = |a, b||u|
    //  |y| = |c, d||v|

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

Point TriangleV2::CalcPointFromBarycentricCoords(const double u, const double v) const
{
    return P0 + P0_P1*u + P0_P2*v;
}

Point TriangleV2::CalcPointFromBarycentricCoords(std::pair<double,double> coords)const
{
    return P0 + P0_P1*coords.first + P0_P2*coords.second;
}

std::tuple<Point, double, bool> TriangleV2::CheckPointSegDist(const Vec3& origin,
                                                   const Vec3& seg,
                                                   const Vec3& P)const
{
    const Vec3 origin_P = P - origin;
    const Vec3 unitVecAlongSeg = seg.normalise();

    const Vec3 Px = origin + unitVecAlongSeg*Vec3::dotProduct(unitVecAlongSeg,origin_P);
    const Vec3 origin_Px = Px - origin;
    auto alongSeg = Vec3::dotProduct(unitVecAlongSeg, origin_Px);
    if(alongSeg >= -Vec3::EPSILON && alongSeg <= seg.magnitude() + Vec3::EPSILON)
    {
        return std::make_tuple(Px, (P-Px).magnitude(),  true);
    }
    else
    {
        if(alongSeg < 0.0)
        {
            return std::make_tuple(origin,origin_P.magnitude(), false);
        }
        else
        {
            auto otherEnd = origin + seg;
            return std::make_tuple(otherEnd, (otherEnd - P).magnitude(), false);
        }
    }
}

std::pair<Point, double> TriangleV2::CalcShortestDistanceFrom(const Point& p)const
{
    auto projectedPointData = ProjectPointOntoTrianglePlane(p);
    const auto pDist = projectedPointData.second;
    const Point projectedPoint = std::get<0>(projectedPointData);
    if(IsPointWithinExtrudedTriangle(projectedPoint))
    {
        return projectedPointData;
    }
    else
    {
        // go ahead and check with the other edges now.
        auto P0_P1_Data = CheckPointSegDist(P0,P0_P1,projectedPoint);
        auto d0 = std::get<1>(P0_P1_Data);

        auto P0_P2_Data = CheckPointSegDist(P0,P0_P2,projectedPoint);
        auto d1 = std::get<1>(P0_P2_Data);

        auto P1_P2_Data = CheckPointSegDist(P1,P1_P2,projectedPoint);
        auto d2 = std::get<1>(P1_P2_Data);
        if(d0 < d1)
        {
            if(d0 < d2)
            {
                auto dist = sqrt(d0*d0 + pDist*pDist);
                return std::make_pair(std::get<0>(P0_P1_Data), dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return std::make_pair(std::get<0>(P1_P2_Data), dist);
            }
        }
        else
        {
            if(d1 < d2)
            {
                auto dist = sqrt(d1*d1 + pDist*pDist);
                return std::make_pair(std::get<0>(P0_P2_Data), dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return std::make_pair(std::get<0>(P1_P2_Data), dist);
            }
        }
    }
}
