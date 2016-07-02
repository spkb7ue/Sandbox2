#include "Triangle.h"

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

    return true;
}

std::pair<Point,float> Triangle::ProjectPointOntoTrianglePlane(const Point& p) const
{
    const auto p_P0 = P0-p;
    auto signedDist = Vec3::dotProduct(p_P0, Normal);


    return std::make_pair(Point(p + Normal*signedDist), -signedDist);
}
