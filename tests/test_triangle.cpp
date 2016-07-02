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
    auto generateExtrudedPoint = [&real_rand](const Triangle& t)->std::pair<Point,float>
    {
        // Point (p) internal to triangle can be calculated using
        // p = P0 + s*(P0_P1) + r*(PO_P2) where 0 <= (s,r) <= 1 and
        // s + r <= 1
        float s = real_rand();
        float r = std::min(1.0f-s, real_rand());
        const float DIST_SCALE = 10.0f;
        BOOST_ASSERT(s + r <= 1.0f);

        float sign = real_rand() > 0.5f ? 1.0f : -1.0f;
        float distance = DIST_SCALE * real_rand() * sign;
        auto PointWithinTriangle = t.P0 + t.P0_P1*s + t.P0_P2*r;
        auto ExtrudedPoint = PointWithinTriangle + t.Normal*distance;
        return std::make_pair(ExtrudedPoint, distance);
    };

    std::for_each(triangles.begin(), triangles.end(),[](const Triangle& t){

    });
}
