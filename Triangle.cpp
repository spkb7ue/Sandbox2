#include "Triangle.h"

using namespace rabbit;


Triangle::Triangle(const Point& p0, const Point& p1, const Point& p2)
:P0(p0),
 P1(p1),
 P2(p2),
 P0_P1(p1-p0),
 P0_P2(p2-p0),
 Normal((Vec3::crossProduct(P0_P1,P0_P2)).normalise())
{

}
