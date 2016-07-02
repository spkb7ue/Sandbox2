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
    const unsigned NUM_TRIANGLES = 2204;    ///< Number of triangles in the mesh
    template <typename T>
    T Sign(T value)
    {
        return value > T(0) ? T(1) : T(-1);
    }
}

BOOST_AUTO_TEST_CASE(TestTriangle_CTor)
{
    // Create test triangles
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    std::for_each(triangles.begin(), triangles.end(),[](const Triangle& t){
        BOOST_ASSERT(Vec3::dotProduct(t.Normal, t.P0_P1) < Vec3::EPSILON);
        BOOST_ASSERT(Vec3::dotProduct(t.Normal, t.P0_P2) < Vec3::EPSILON);
        BOOST_ASSERT(Vec3::dotProduct(t.Normal, t.P1_P2) < Vec3::EPSILON);
        BOOST_ASSERT(std::abs(t.Normal.magnitude()-1.0f) < Vec3::EPSILON);
    });
}

BOOST_AUTO_TEST_CASE(TestTriangle_IsPointWithinExtrudedTriangle)
{
    // Load test triangles from file
    rabbit::Mesh mesh(FILE_NAME);
    const auto& triangles = mesh.GetTriangles();

    mt19937::result_type seed = time(0);
    auto real_rand = std::bind(std::uniform_real_distribution<float>(0,1), mt19937(seed));

    // Generate random point within a triangle
    auto generateExtrudedPointData = [&real_rand](const Triangle& t)->std::tuple<Point, Point,float>
    {
        // Point (p) internal to triangle can be calculated using
        // p = P0 + s*(P0_P1) + r*(PO_P2) where 0 <= (s,r) <= 1 and
        // s + r <= 1
        float s = real_rand();
        float r = std::min(1.0f-s, real_rand());
        const float DIST_SCALE = 10.0f;
        BOOST_ASSERT_MSG(s + r <= 1.0f, "The triangles internal generated parameters are invalid. Cannot proceed");

        float sign = real_rand() > 0.5f ? 1.0f : -1.0f;
        float distance = DIST_SCALE * real_rand() * sign;
        auto PointWithinTriangle = t.P0 + t.P0_P1*s + t.P0_P2*r;
        auto ExtrudedPoint = PointWithinTriangle + t.Normal*distance;
        return std::make_tuple(PointWithinTriangle, ExtrudedPoint, distance);
    };

    std::for_each(triangles.begin(), triangles.end(),[&generateExtrudedPointData](const Triangle& t){
        auto testPointData = generateExtrudedPointData(t);
        auto generatedData = t.ProjectPointOntoTrianglePlane(std::get<1>(testPointData));

        BOOST_ASSERT_MSG(Sign(std::get<2>(testPointData)) == Sign(generatedData.second),
                         "Signs of the computed distance don't match expected value");

        bool isCalculatedDistanceWithinTolerance = std::abs(std::get<2>(testPointData) - generatedData.second) < Vec3::EPSILON;
        BOOST_ASSERT_MSG(isCalculatedDistanceWithinTolerance,
                        "The distance from point to triangle does not match expected data");

    });

}
