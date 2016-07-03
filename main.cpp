#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "Mesh.h"
#include "Triangle.h"
using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";
    static mt19937::result_type seed = time(0);
    static auto real_rand = std::bind(std::uniform_real_distribution<double>(0,1), mt19937(seed));

    Point GenerateTriangleInternalPoint(const Triangle& t)
    {
        // Point (p) internal to triangle can be represented as r0*P0 + r1*P1 + r2*P3
        // where r0 + r1 + r2 = 1;
        double r0 = real_rand();
        double r1 = real_rand();
        double r2 = real_rand();
        auto mag = r0 + r1 + r2;
        r0 /= mag;
        r1 /= mag;
        r2 /= mag;
        return t.P0*r0 + t.P1*r1 + t.P2*r2;
    }

    // Generate random point within a triangle extruded along its normal
    std::tuple<Point, Point,double> GenerateExtrudedPointData(const Triangle& t)
    {
        const double DIST_SCALE = 10.0f;
        double sign = real_rand() > 0.5f ? 1.0f : -1.0f;
        double distance = DIST_SCALE * real_rand() * sign;
        auto PointWithinTriangle = GenerateTriangleInternalPoint(t);
        auto ExtrudedPoint = PointWithinTriangle + t.Normal*distance;
        return std::make_tuple(PointWithinTriangle, ExtrudedPoint, distance);
    };
}

int main()
{
    Point p0(-0.369095000000000,  -0.541160010000000,   0.131570000000000);
    Point p1(-0.388785020000000,  -0.531530000000000 ,  0.125940000000000);
    Point p2(-0.4127550100000000 , -0.5245500200000000 ,  0.0715800000000000);
    Point p(-0.4127550100000000,  -0.5245500200000000  , 0.0715800000000000);
    Triangle t(p0, p1, p2);
    cout<<"Hello World\n"<<endl;
    cout<<boolalpha;
    auto pt = t.CheckPointSegDist(p0, p1-p0, p);

}
