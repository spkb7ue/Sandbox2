#include "Triangle.h"
#include "Mesh.h"
#include <random>
#include <memory>
#define BOOST_TEST_MODULE Test_Triangle
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;

namespace
{
    const std::string FILE_NAME = "rabbit.triangles"; ///< File with the triangulated mesh
    template <typename T>

    T Sign(T value)
    {
        return value > T(0) ? T(1) : T(-1);
    }

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

    Point GeneratePointOutsideExtrudedTriangle(const Triangle& t)
    {
        Point withinTriangle = std::get<1>(GenerateExtrudedPointData(t));
        return withinTriangle + t.P0_P1*(1.0f + std::max(real_rand(),0.01));
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CTor)
{
    // Create test triangles
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    for(const auto& t : triangles)
    {
        BOOST_ASSERT(Vec3::dotProduct(t.Normal, t.P0_P1) < Vec3::EPSILON);
        BOOST_ASSERT(Vec3::dotProduct(t.Normal, t.P0_P2) < Vec3::EPSILON);
        BOOST_ASSERT(Vec3::dotProduct(t.Normal, t.P1_P2) < Vec3::EPSILON);
        BOOST_ASSERT(std::abs(t.Normal.magnitude()-1.0f) < Vec3::EPSILON);
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CalcBarycentricCoords)
{
    // Create test triangles
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    double u,v;
    for(const Triangle& t : triangles)
    {
        {   // Test vertex P0
            std::tie(u,v) = t.CalcBarycentricCoords(t.P0);
            BOOST_ASSERT(std::abs(u)<Vec3::EPSILON);
            BOOST_ASSERT(std::abs(v)<Vec3::EPSILON);
        }

        {   // Test vertex P1
            std::tie(u,v) = t.CalcBarycentricCoords(t.P1);
            BOOST_ASSERT(std::abs(u-1.0f)<Vec3::EPSILON);
            BOOST_ASSERT(std::abs(v)<Vec3::EPSILON);
        }

        {   // Test vertex P2
            std::tie(u,v) = t.CalcBarycentricCoords(t.P2);
            BOOST_ASSERT(std::abs(u)<Vec3::EPSILON);
            BOOST_ASSERT(std::abs(v-1.0f)<Vec3::EPSILON);
        }

        const double TOLERANCE = 0.005f;

        for(unsigned i = 0; i < 100; ++i)
        {
            Point internalPoint = GenerateTriangleInternalPoint(t);
            std::tie(u,v) = t.CalcBarycentricCoords(internalPoint);
            {   // Assert that the calculated Barycentric coordinates obey the constraints for
                // a point internal to a triangle
                std::stringstream error_msg;
                error_msg << "u:"<<u<<"\tv:"<<v;
                BOOST_ASSERT_MSG(u >= 0.0f - TOLERANCE && u <= 1.0f + TOLERANCE, error_msg.str().c_str());
                BOOST_ASSERT_MSG(v >= 0.0f - TOLERANCE && v <= 1.0f + TOLERANCE, error_msg.str().c_str());
                BOOST_ASSERT_MSG(u + v < 1.0f + TOLERANCE, error_msg.str().c_str());
            }

            {   // Make sure that the Barycentric coordinates actually give us the internal point
                const Point calculatedPoint = t.P0 + t.P0_P1*u + t.P0_P2*v;
                BOOST_ASSERT(calculatedPoint.isSameAs(internalPoint, 1.0e-3));

            }
        }

        for(unsigned i = 0; i<100; ++i)
        {
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& expectedProjectedPoint = std::get<0>(testPointData);
            const Point& expectedExtrudedPoint = std::get<1>(testPointData);

            std::tie(u,v) = t.CalcBarycentricCoords(expectedExtrudedPoint);
            Point calculatedProjectedPoint = t.P0 + t.P0_P1*u + t.P0_P2*v;
            BOOST_ASSERT(calculatedProjectedPoint.isSameAs(expectedProjectedPoint, 1.0e-3));
        }
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_ProjectPointOntoTrianglePlane)
{
    // Load test triangles from file
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    for(unsigned i=0;i<100;++i)
    {
        // We repeat the test 100 times just to get sufficient coverage of tested points
        for(const auto& t : triangles)
        {
            const auto testPointData = GenerateExtrudedPointData(t);

            const Point& expectedProjectedPoint = std::get<0>(testPointData);
            const Point& expectedExtrudedPoint = std::get<1>(testPointData);
            const double& expectedExtrudeDist = std::get<2>(testPointData);

            {   // Calculate using ProjectPointOntoTrianglePlane
                const auto calculatedData = t.ProjectPointOntoTrianglePlane(expectedExtrudedPoint);
                const auto calculatedProjectedPoint = calculatedData.first;
                const auto calculatedDist = calculatedData.second;

                BOOST_ASSERT_MSG(Sign(expectedExtrudeDist) == Sign(calculatedDist),
                                 "Signs of the computed distance don't match expected value");

                bool isCalculatedDistanceWithinTolerance = std::abs(expectedExtrudeDist - calculatedDist) < 1.e-5;
                BOOST_ASSERT_MSG(isCalculatedDistanceWithinTolerance,
                                "The distance from point to triangle does not match expected data");

                bool areProjectedPointsIdentical = calculatedProjectedPoint.isSameAs(expectedProjectedPoint, 1.e-5);
                BOOST_ASSERT_MSG(areProjectedPointsIdentical,"Projected point onto the triangle plane is incorrect");
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_IsPointWithinExtrudedTriangle)
{
    // Load test triangles from file
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    for(const Triangle& t : triangles)
    {
        for(unsigned i = 0; i < 100; ++i)
        {
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& extrudedPoint = std::get<1>(testPointData);
            {
                bool pass = t.IsPointWithinExtrudedTriangle(extrudedPoint);
                if(!pass)
                {
                    auto bc = t.CalcBarycentricCoords(extrudedPoint);
                    cout<<extrudedPoint<<endl;
                    cout<<bc.first<<", "<<bc.second<<endl;
                    cout<<t.CalcPointFromBarycentricCoords(bc)<<endl;
                    cout<<t.ProjectPointOntoTrianglePlane(extrudedPoint).first<<endl;
                    cout<<t.ProjectPointOntoTrianglePlane(extrudedPoint).second<<endl;
                    cout<<t.Area<<endl;
                    cout<<t.P0<<endl;
                    cout<<t.P1<<endl;
                    cout<<t.P2<<endl;
                    cout<<"hEre"<<endl;
                    cin.get();
                }
            }
            BOOST_ASSERT(t.IsPointWithinExtrudedTriangle(extrudedPoint));

            {
                Point p = GeneratePointOutsideExtrudedTriangle(t);
                bool pass = !t.IsPointWithinExtrudedTriangle(p);
                if(!pass)
                {
                    auto bc = t.CalcBarycentricCoords(p);
                    cout<<p<<endl;
                    cout<<bc.first<<", "<<bc.second<<endl;
                    cout<<t.CalcPointFromBarycentricCoords(bc)<<endl;
                    cout<<t.ProjectPointOntoTrianglePlane(p).first<<endl;
                    cin.get();
                }
                BOOST_ASSERT(!t.IsPointWithinExtrudedTriangle(GeneratePointOutsideExtrudedTriangle(t)));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CheckPointSegDist_Internal)
{
    // Create test triangles
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    for(const Triangle& t : triangles)
    {
        for(unsigned i=0;i<1000;++i)
        {
            Point p = GenerateTriangleInternalPoint(t);
            int numSegs = 0;

            {   // Check internal P0_P1
                auto px = t.CheckPointSegDist(t.P0, t.P0_P1, p);
                if(std::get<2>(px))
                {
                    ++numSegs;
                }
            }

            {   // Check internal P0_P2
                auto px = t.CheckPointSegDist(t.P0, t.P0_P2, p);
                if(std::get<2>(px))
                {
                    ++numSegs;
                }
            }

            {   // Check internal P0_P2
                auto px = t.CheckPointSegDist(t.P1, t.P1_P2, p);
                if(std::get<2>(px))
                {
                    ++numSegs;
                }
            }

            BOOST_ASSERT(numSegs >= 2);
        }
    }
}
