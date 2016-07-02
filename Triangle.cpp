#include "Triangle.h"
#include <memory>

using namespace rabbit;
using namespace std;


Triangle::Triangle(const Point& p0, const Point& p1, const Point& p2)
:P0(p0),
 P1(p1),
 P2(p2),
 P0_P1(p1-p0),
 P0_P2(p2-p0),
 P1_P2(p2-p1),
 Normal((Vec3::crossProduct(P0_P1,P0_P2)).normalise()),
 Centroid((P0+P1+P2)*1.0f/3.0f),
 Area((Vec3::crossProduct(P0_P1,P0_P2)).magnitude()*0.5f),
IsDegenerate(Area < 1.0E-4f)
{

}

bool Triangle::IsPointWithinExtrudedTriangle(const Point& p)const
{
    float u,v;
    std::tie(u,v) = CalcBarycentricCoords(p);
    const float tol = 0.01f;
    return ( u >= -tol && v >= -tol && u+v <= 1.0f + tol);
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

Point Triangle::CalcPointFromBarycentricCoords(const float u, const float v) const
{
    return P0 + P0_P1*u + P0_P2*v;
}

Point Triangle::CalcPointFromBarycentricCoords(std::pair<float,float> coords)const
{
    return P0 + P0_P1*coords.first + P0_P2*coords.second;
}

std::pair<Point, float> Triangle::CalcInPlaneDistanceToTriangle(const Point& P)const
{
    // Calculate distance from point p to P0_P1
    const Vec3 normal_P0_P1 = (Vec3::crossProduct(Normal,P0_P1)).normalise();
    const Vec3 P_P0 = P0 - P;
    float distToP0P1 = Vec3::dotProduct(normal_P0_P1, P_P0);
    const Vec3 Px = P + normal_P0_P1*distToP0P1;
    const Vec3 P0_Px = Px - P0;
    const Vec3 unit_P0_P1 = P0_P1.normalise();
    float alongP0P1 = Vec3::dotProduct(unit_P0_P1, P0_Px);
    if(alongP0P1 > 0.0f and alongP0P1 <= P0_P1.magnitude())
    {
        return std::make_pair(Px, distToP0P1);
    }
}

boost::optional<Point> Triangle::CheckPointSegDist(const Vec3& origin,
                                                   const Vec3& seg,
                                                   const Vec3& P)const
{
    const Vec3 normalSeg = (Vec3::crossProduct(Normal,seg)).normalise();
    const Vec3 P_origin = origin - P;
    auto distToSeg = Vec3::dotProduct(normalSeg, P_origin);
    const Vec3 Px = P + normalSeg*distToSeg;
    const Vec3 origin_Px = Px - origin;
    const Vec3 unitVecAlongSeg = seg.normalise();
    auto alongSeg = Vec3::dotProduct(unitVecAlongSeg, origin_Px);
    if(alongSeg >= 0.0f && alongSeg <= seg.magnitude() + 0.01)
    {
        return Px;
    }
    else
    {
        cout<<P<<endl;
        cout<<P0_P1<<endl;
        cout<<P0_P2<<endl;
        cout<<P1_P2<<endl;
        cout<<origin_Px<<endl;
        cout<<alongSeg<<endl;
        cout<<seg.magnitude()<<endl;
        cout<<Area<<endl;
        cout<<Vec3::getDistance(Centroid, Px)<<endl;
        cin.get();
        return boost::none;
    }
}
