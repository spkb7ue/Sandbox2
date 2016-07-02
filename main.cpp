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
    static auto real_rand = std::bind(std::uniform_real_distribution<float>(0,1), mt19937(seed));

    Point GenerateTriangleInternalPoint(const Triangle& t)
    {
        // Point (p) internal to triangle can be represented as r0*P0 + r1*P1 + r2*P3
        // where r0 + r1 + r2 = 1;
        float r0 = real_rand();
        float r1 = real_rand();
        float r2 = real_rand();
        auto mag = r0 + r1 + r2;
        r0 /= mag;
        r1 /= mag;
        r2 /= mag;
        return t.P0*r0 + t.P1*r1 + t.P2*r2;
    }

    // Generate random point within a triangle extruded along its normal
    std::tuple<Point, Point,float> GenerateExtrudedPointData(const Triangle& t)
    {
        const float DIST_SCALE = 10.0f;
        float sign = real_rand() > 0.5f ? 1.0f : -1.0f;
        float distance = DIST_SCALE * real_rand() * sign;
        auto PointWithinTriangle = GenerateTriangleInternalPoint(t);
        auto ExtrudedPoint = PointWithinTriangle + t.Normal*distance;
        return std::make_tuple(PointWithinTriangle, ExtrudedPoint, distance);
    };
}

int main()
{
    cout<<"Hello World\n"<<endl;
    rabbit::Mesh mesh(fileName);
    const auto& triangles = mesh.GetTriangles();
    for(const Triangle& t: triangles)
    {
        float u,v;
        for(unsigned i = 0; i<100; ++i)
        {
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& expectedProjectedPoint = std::get<0>(testPointData);
            const Point& expectedExtrudedPoint = std::get<1>(testPointData);

            std::tie(u,v) = t.CalcBarycentricCoords(expectedExtrudedPoint);
            Point calculatedProjectedPoint = t.P0 + t.P0_P1*u + t.P0_P2*(v*3.0);
            cout<<calculatedProjectedPoint<<endl;
            cout<<expectedProjectedPoint<<endl;
            cout<<std::boolalpha<<calculatedProjectedPoint.isSameAs(expectedProjectedPoint)<<endl;
            cout<<"---------------\n";
            cin.get();
        }
    }
}
