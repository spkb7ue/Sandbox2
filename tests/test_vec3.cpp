#include "Vec3.h"
#include <iostream>
#define BOOST_TEST_MODULE Test_Vec3
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;

namespace
{
    const Vec3 v1(1.0f, 2.0f, 3.3f);
    const Vec3 v2(2.0f, 3.0f, 1.0f);
}

BOOST_AUTO_TEST_CASE(TestVec3_DotProduct)
{
    {
        auto dotProd = Vec3::dotProduct(v1, v2);
        BOOST_ASSERT(dotProd == 11.3f);
    }

    {
        auto dotProd = v1.dotProduct(v2);
        BOOST_ASSERT(dotProd == 11.3f);
    }
}

BOOST_AUTO_TEST_CASE(TestVec3_Normalize)
{
    BOOST_ASSERT(std::abs((v1.normalise()).magnitude()-1.0f) < Vec3::EPSILON);
    BOOST_ASSERT(std::abs((v2.normalise()).magnitude()-1.0f) < Vec3::EPSILON);
}

BOOST_AUTO_TEST_CASE(TestVec3_CrossProduct)
{
    auto v1CrossV2 = Vec3::crossProduct(v1,v2);
    BOOST_ASSERT(Vec3::dotProduct(v1CrossV2, v1) < Vec3::EPSILON);
    BOOST_ASSERT(Vec3::dotProduct(v1CrossV2, v2) < Vec3::EPSILON);
}
