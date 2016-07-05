#include "Triangle.h"
#include "Mesh.h"
#include "TriangularMeshBuildingPolicy.h"
#include <random>
#include <iomanip>
#include <memory>
#define BOOST_TEST_MODULE Test_Triangle
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;

namespace
{
    const std::string FILE_NAME = "rabbit.triangles"; ///< File with the triangulated mesh
    std::shared_ptr<TriangularMeshBuilingPolicy> GetMeshBuildingPolicy()
    {
        return std::make_shared<TriangularMeshBuilingPolicy>(FILE_NAME);
    }

    template <typename T>
    T Sign(T value)
    {
        return value > T(0) ? T(1) : T(-1);
    }

    static mt19937::result_type seed = time(0);
    static auto real_rand = std::bind(std::uniform_real_distribution<double>(0,1), mt19937(seed));

    Point GenerateTriangleInternalPoint(const RigidTriangle& t)
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
        return t.P0()*r0 + t.P1()*r1 + t.P2()*r2;
    }

    // Generate random point within a triangle extruded along its normal
    std::tuple<Point, Point,double> GenerateExtrudedPointData(const RigidTriangle& t)
    {
        const double DIST_SCALE = 10.0f;
        double sign = real_rand() > 0.5f ? 1.0f : -1.0f;
        double distance = DIST_SCALE * real_rand() * sign;
        auto PointWithinTriangle = GenerateTriangleInternalPoint(t);
        auto ExtrudedPoint = PointWithinTriangle + t.Normal()*distance;
        return std::make_tuple(PointWithinTriangle, ExtrudedPoint, distance);
    };

    Point GeneratePointOutsideExtrudedTriangle(const RigidTriangle& t)
    {
        Point withinTriangle = std::get<1>(GenerateExtrudedPointData(t));
        return withinTriangle + t.P0P1()*(1.0f + std::max(real_rand(),0.01));
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CTor)
{
    // Create test triangles
    auto builder = GetMeshBuildingPolicy();
    rabbit::Mesh<RigidTriangle> mesh(builder);
    const auto& triangles = mesh.GetPolygons();

    for(const RigidTriangle& t : triangles)
    {

        if(std::abs(Vec3::dotProduct(t.Normal(), t.P0P1())) < Vec3::EPSILON)
        {

        }
        else
        {
            cout<<setprecision(10)<<t.Normal()<<endl<<t.P0P1()<<endl;
            cout<<t.P0()<<endl;
            cout<<t.P1()<<endl;
            cout<<t.P2()<<endl;
            cout<<t.Vert(TriangleProps3D::VertIndices::eP0)<<endl;
            cin.get();
        }
        BOOST_ASSERT(std::abs(Vec3::dotProduct(t.Normal(), t.P0P1())) < Vec3::EPSILON);
        BOOST_ASSERT(std::abs(Vec3::dotProduct(t.Normal(), t.P0P2())) < Vec3::EPSILON);
        BOOST_ASSERT(std::abs(Vec3::dotProduct(t.Normal(), t.P1P2())) < Vec3::EPSILON);
        BOOST_ASSERT(std::abs(t.Normal().magnitude()-1.0f) < Vec3::EPSILON);
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CalcBarycentricCoords)
{
    // Create test triangles
    auto builder = GetMeshBuildingPolicy();
    rabbit::Mesh<RigidTriangle> mesh(builder);
    const auto& triangles = mesh.GetPolygons();

    for(const auto& t : triangles)
    {
        {   // Test vertex P0
            auto coords = t.CalcBarycentricCoords(t.P0());
            BOOST_ASSERT(std::abs(coords.u)<Vec3::EPSILON);
            BOOST_ASSERT(std::abs(coords.v)<Vec3::EPSILON);
        }

        {   // Test vertex P1
            auto coords = t.CalcBarycentricCoords(t.P1());
            BOOST_ASSERT(std::abs(coords.u-1.0f)<Vec3::EPSILON);
            BOOST_ASSERT(std::abs(coords.v)<Vec3::EPSILON);
        }

        {   // Test vertex P2
            auto coords = t.CalcBarycentricCoords(t.P2());
            BOOST_ASSERT(std::abs(coords.u)<Vec3::EPSILON);
            BOOST_ASSERT(std::abs(coords.v-1.0f)<Vec3::EPSILON);
        }

        const double TOLERANCE = 0.005f;

        for(unsigned i = 0; i < 100; ++i)
        {
            Point internalPoint = GenerateTriangleInternalPoint(t);
            auto coords = t.CalcBarycentricCoords(internalPoint);
            {   // Assert that the calculated Barycentric coordinates obey the constraints for
                // a point internal to a triangle
                std::stringstream error_msg;
                error_msg << "u:"<<coords.u<<"\tv:"<<coords.v;
                BOOST_ASSERT_MSG(coords.u >= 0.0f - TOLERANCE && coords.u <= 1.0f + TOLERANCE, error_msg.str().c_str());
                BOOST_ASSERT_MSG(coords.v >= 0.0f - TOLERANCE && coords.v <= 1.0f + TOLERANCE, error_msg.str().c_str());
                BOOST_ASSERT_MSG(coords.u + coords.v < 1.0f + TOLERANCE, error_msg.str().c_str());
            }

            {   // Make sure that the Barycentric coordinates actually give us the internal point
                const Point calculatedPoint = t.P0() + t.P0P1()*coords.u + t.P0P2()*coords.v;
                BOOST_ASSERT(calculatedPoint.isSameAs(internalPoint, 1.0e-3));

            }
        }

        for(unsigned i = 0; i<100; ++i)
        {
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& expectedProjectedPoint = std::get<0>(testPointData);
            const Point& expectedExtrudedPoint = std::get<1>(testPointData);

            auto coords = t.CalcBarycentricCoords(expectedExtrudedPoint);
            Point calculatedProjectedPoint = t.P0() + t.P0P1()*coords.u + t.P0P2()*coords.v;
            BOOST_ASSERT(calculatedProjectedPoint.isSameAs(expectedProjectedPoint, 1.0e-3));
        }
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_ProjectPointOntoTrianglePlane)
{
    // Load test triangles from file
    auto builder = GetMeshBuildingPolicy();
    rabbit::Mesh<RigidTriangle> mesh(builder);
    const auto& triangles = mesh.GetPolygons();

    for(unsigned i=0;i<100;++i)
    {
        // We repeat the test 100 times just to get sufficient coverage of tested points
        for(const auto& t : triangles)
        {
            const auto testPointData = GenerateExtrudedPointData(t);

            const Point& expectedProjectedPoint = std::get<0>(testPointData);
            const Point& expectedExtrudedPoint = std::get<1>(testPointData);
            const double& expectedExtrudeDist = std::get<2>(testPointData);

            {   // Calculate using ProjectPointOntoShapePlane
                const auto calculatedData = t.ProjectPointOntoShapePlane(expectedExtrudedPoint);
                const auto calculatedProjectedPoint = calculatedData.P;
                const auto calculatedDist = calculatedData.Dist;

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
    auto builder = GetMeshBuildingPolicy();
    rabbit::Mesh<RigidTriangle> mesh(builder);
    const auto& triangles = mesh.GetPolygons();

    for(const auto& t : triangles)
    {
        for(unsigned i = 0; i < 100; ++i)
        {
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& extrudedPoint = std::get<1>(testPointData);
            BOOST_ASSERT(t.IsPointWithinShapeExtrudedAlongNormal(extrudedPoint));
            BOOST_ASSERT(!t.IsPointWithinShapeExtrudedAlongNormal(GeneratePointOutsideExtrudedTriangle(t)));

        }
    }
}
