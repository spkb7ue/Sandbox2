#include "Triangle.h"
#include "Mesh.h"
#include "Vec3.h"
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

    static mt19937::result_type seed = time(0);
    static auto real_rand = std::bind(std::uniform_real_distribution<double>(0,1), mt19937(seed));

    Point GenerateTriangleInternalPoint(const Triangle<Vec3>& t)
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
    std::tuple<Point, Point,double> GenerateExtrudedPointData(const Triangle<Vec3>& t)
    {
        const double DIST_SCALE = 10.0f;
        double sign = real_rand() > 0.5f ? 1.0f : -1.0f;
        double distance = DIST_SCALE * real_rand() * sign;
        auto PointWithinTriangle = GenerateTriangleInternalPoint(t);
        auto ExtrudedPoint = PointWithinTriangle + t.Normal()*distance;
        return std::make_tuple(PointWithinTriangle, ExtrudedPoint, distance);
    };

    Point GeneratePointOutsideExtrudedTriangle(const Triangle<Vec3>& t)
    {
        Point withinTriangle = std::get<1>(GenerateExtrudedPointData(t));
        return withinTriangle + t.P0P1()*(1.0f + std::max(real_rand(),0.01));
    }

    class TestData
    {
    public:
        TestData()
        {
            auto builder = GetMeshBuildingPolicy();
            rabbit::Mesh<Triangle<Vec3>> mesh(builder);
            m_testTriangles = mesh.GetPolygons();
        }

        const std::vector<Triangle<Vec3>>& GetTestTriangles(){return m_testTriangles;}
    private:
        std::vector<Triangle<Vec3>> m_testTriangles;
    };
    TestData testData;

    const unsigned NUM_ITERATIONS = 100;
}

BOOST_AUTO_TEST_CASE(TestTriangle_CheckPointSegDist_Internal)
{
    const auto& triangles = testData.GetTestTriangles();

    for(const Triangle<Vec3>& t : triangles)
    {
        {   /// Test P0 with P0P1
            auto px = t.CheckPointSegDist(t.P0(),t.P0P1(),t.P0());
            BOOST_ASSERT(px.IsWithinExtrudedNormal);
            BOOST_ASSERT(std::abs(px.IRes.Dist) < EPSILON);
        }

        {   /// Test P0 with P0P2
            auto px = t.CheckPointSegDist(t.P0(),t.P0P2(),t.P0());
            BOOST_ASSERT(px.IsWithinExtrudedNormal);
            BOOST_ASSERT(std::abs(px.IRes.Dist) < EPSILON);
        }
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CalcAABB)
{
    const auto& triangles = testData.GetTestTriangles();

    for(const auto& t : triangles)
    {
        AABB<Vec3> aabb = t.CalculateAABB();

        // Assert that all the triangle vertices are inside the aabb
        BOOST_ASSERT(aabb.IsPointWithinAABB(t.P0()));
        BOOST_ASSERT(aabb.IsPointWithinAABB(t.P1()));
        BOOST_ASSERT(aabb.IsPointWithinAABB(t.P2()));
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CTor)
{
    const auto& triangles = testData.GetTestTriangles();

    for(const Triangle<Vec3>& t : triangles)
    {
        // Ensure that the the triangles on construction are set up correctly
        BOOST_ASSERT(std::abs(Vec3::dotProduct(t.Normal(), t.P0P1())) < EPSILON);
        BOOST_ASSERT(std::abs(Vec3::dotProduct(t.Normal(), t.P0P2())) < EPSILON);
        BOOST_ASSERT(std::abs(Vec3::dotProduct(t.Normal(), t.P1P2())) < EPSILON);
        BOOST_ASSERT(std::abs(t.Normal().magnitude()-1.0f) < EPSILON);
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CalcBarycentricCoords)
{
    const auto& triangles = testData.GetTestTriangles();

    for(const auto& t : triangles)
    {
        // We expect that the barycentric coordinates are 0,0
        // for all the 3 vertices
        {   // Test vertex P0
            auto coords = t.CalcBarycentricCoords(t.P0());
            BOOST_ASSERT(std::abs(coords.u)<EPSILON);
            BOOST_ASSERT(std::abs(coords.v)<EPSILON);
        }

        {   // Test vertex P1
            auto coords = t.CalcBarycentricCoords(t.P1());
            BOOST_ASSERT(std::abs(coords.u-1.0f)<EPSILON);
            BOOST_ASSERT(std::abs(coords.v)<EPSILON);
        }

        {   // Test vertex P2
            auto coords = t.CalcBarycentricCoords(t.P2());
            BOOST_ASSERT(std::abs(coords.u)<EPSILON);
            BOOST_ASSERT(std::abs(coords.v-1.0f)<EPSILON);
        }

        const double TOLERANCE = 0.005f;

        // This loop tests a internal point for the triangle
        for(unsigned i = 0; i < NUM_ITERATIONS; ++i)
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

        // This loop tests a point in the extruded volume.
        for(unsigned i = 0; i<NUM_ITERATIONS; ++i)
        {
            // Also test a point in the volume generated by extruding the triangle
            // along its normal. Make sure that the bary centric coordinates
            // correspond to projected point onto the triangle
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& expectedProjectedPoint = std::get<0>(testPointData);
            const Point& expectedExtrudedPoint = std::get<1>(testPointData);

            auto coords = t.CalcBarycentricCoords(expectedExtrudedPoint);
            Point calculatedProjectedPoint = t.P0() + t.P0P1()*coords.u + t.P0P2()*coords.v;
            BOOST_ASSERT(calculatedProjectedPoint.isSameAs(expectedProjectedPoint, 1.0e-3));
        }

        // This loop tests a point outside the extruded volume
        for(unsigned i = 0; i<NUM_ITERATIONS; ++i)
        {
            const auto testPointData = GeneratePointOutsideExtrudedTriangle(t);
            auto coords = t.CalcBarycentricCoords(testPointData);
            bool insideTriCoords = (coords.u >= 0.0 && coords.v >=0.0 && coords.u + coords.v <= 1.0);
            BOOST_ASSERT(!insideTriCoords);
            Point calculatedProjectedPoint = t.P0() + t.P0P1()*coords.u + t.P0P2()*coords.v;
            BOOST_ASSERT(!t.IsPointWithinShapeExtrudedAlongNormal(calculatedProjectedPoint));
        }
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_ProjectPointOntoShapePlane)
{
    const auto& triangles = testData.GetTestTriangles();

    for(unsigned i=0;i<NUM_ITERATIONS;++i)
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
                const auto calculatedProjectedPoint = calculatedData.Point;
                const auto calculatedDist = calculatedData.Dist;

                bool isCalculatedDistanceWithinTolerance = std::abs( std::abs(expectedExtrudeDist) - std::abs(calculatedDist)) < 1.e-5;
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
    const auto& triangles = testData.GetTestTriangles();

    for(const auto& t : triangles)
    {
        for(unsigned i = 0; i < NUM_ITERATIONS; ++i)
        {
            const auto testPointData = GenerateExtrudedPointData(t);
            const Point& extrudedPoint = std::get<1>(testPointData);
            BOOST_ASSERT(t.IsPointWithinShapeExtrudedAlongNormal(extrudedPoint));
            BOOST_ASSERT(!t.IsPointWithinShapeExtrudedAlongNormal(GeneratePointOutsideExtrudedTriangle(t)));

        }
    }
}
