#include <vector>
#include <iostream>
#include <AABB.h>
#include <Mesh.h>
#include <TriangularMeshBuildingPolicy.h>
#define BOOST_TEST_MODULE Test_AABB
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;
namespace
{
    mt19937::result_type seed = time(0);
    auto real_rand = std::bind(std::uniform_real_distribution<double>(0.0,1), mt19937(seed));
}

BOOST_AUTO_TEST_CASE(TestAABB_IsPointInsideAABB)
{
    // Test if the object can be constructed
    Vec3 center(real_rand(),real_rand(),real_rand());
    Vec3 halfExtents(real_rand(),real_rand(),real_rand());

    // These are our test vertices
    Vec3 boxVert0 = center + Vec3(-halfExtents.X(), -halfExtents.Y(), -halfExtents.Z());
    Vec3 boxVert1 = center + Vec3(halfExtents.X(), -halfExtents.Y(), -halfExtents.Z());
    Vec3 boxVert2 = center + Vec3(-halfExtents.X(), halfExtents.Y(), -halfExtents.Z());
    Vec3 boxVert3 = center + Vec3(halfExtents.X(), halfExtents.Y(), -halfExtents.Z());

    Vec3 boxVert4 = center + Vec3(-halfExtents.X(), -halfExtents.Y(), halfExtents.Z());
    Vec3 boxVert5 = center + Vec3(halfExtents.X(), -halfExtents.Y(), halfExtents.Z());
    Vec3 boxVert6 = center + Vec3(-halfExtents.X(), halfExtents.Y(), halfExtents.Z());
    Vec3 boxVert7 = center + Vec3(halfExtents.X(), halfExtents.Y(), halfExtents.Z());

    AABB<Vec3> aabb(center, halfExtents);

    BOOST_ASSERT(aabb.IsPointWithinAABB(center));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert0));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert1));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert2));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert3));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert4));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert5));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert6));
    BOOST_ASSERT(aabb.IsPointWithinAABB(boxVert7));
    BOOST_ASSERT(!aabb.IsPointWithinAABB(center + halfExtents*1.1));
}

BOOST_AUTO_TEST_CASE(TestAABB_CalculateShortestDistance)
{
    Vec3 center(real_rand(),real_rand(),real_rand());
    Vec3 halfExtents(real_rand(),real_rand(),real_rand());
    AABB<Vec3> aabb(center, halfExtents);
    Vec3 boxVert0 = center + Vec3(-halfExtents.X(), -halfExtents.Y(), -halfExtents.Z());

    // Assert that for a point inside, we get a zero distance
    BOOST_ASSERT(aabb.CalcShortestDistanceFrom(center).Dist < Vec3::EPSILON);
    BOOST_ASSERT(aabb.CalcShortestDistanceFrom(boxVert0).Dist < Vec3::EPSILON);

    double expectedDistance = std::max(0.01, real_rand());
    Vec3 centerToHalfExtents = center + halfExtents;
    Vec3 displacementDir = centerToHalfExtents.normalise();
    Vec3 pointOutside = center + halfExtents + displacementDir * expectedDistance;

    cout<<aabb.CalcShortestDistanceFrom(pointOutside).Dist<<endl;
    cout<<expectedDistance<<endl;

    // Assert that we do get the expected distance
    BOOST_ASSERT( std::abs(aabb.CalcShortestDistanceFrom(pointOutside).Dist-expectedDistance) < Vec3::EPSILON);
    double LARGE = 1000000000.0;
    // Assert that if the distance is greater than threshold, we get a large distance
    BOOST_ASSERT( aabb.CalcShortestDistanceFrom(pointOutside, expectedDistance - std::max(real_rand(),0.0001)).Dist > LARGE);



}
