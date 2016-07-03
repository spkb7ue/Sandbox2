#include "Vec3.h"
#include <iostream>
#define BOOST_TEST_MODULE Test_Vec3
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;

namespace
{
    mt19937::result_type seed = time(0);
    auto real_rand = std::bind(std::uniform_real_distribution<double>(0.0,1), mt19937(seed));
}

BOOST_AUTO_TEST_CASE(TestVec3_DotProduct)
{
    for(unsigned i=0;i<1000;++i)
    {
        const double a = real_rand();
        const double b = real_rand();
        const double c = real_rand();
        const double d = real_rand();
        const double e = real_rand();
        const double f = real_rand();

        const Vec3 v1(a, b, c);
        const Vec3 v2(d, e, f);
        const auto expected = a*d + b*e + c*f;

        {
            auto dotProd = Vec3::dotProduct(v1, v2);
            BOOST_ASSERT(dotProd == expected);
        }

        {
            auto dotProd = v1.dotProduct(v2);
            BOOST_ASSERT(dotProd == expected);
        }
    }

}


BOOST_AUTO_TEST_CASE(TestVec3_Normalize)
{
    for(unsigned i=0;i<1000000;++i)
    {
        const double a = real_rand();
        const double b = real_rand();
        const double c = real_rand();
        const double d = real_rand();
        const double e = real_rand();
        const double f = real_rand();

        const Vec3 v1(a, b, c);
        const Vec3 v2(d, e, f);
        BOOST_ASSERT(std::abs((v1.normalise()).magnitude()-1.0) < Vec3::EPSILON);
        BOOST_ASSERT(std::abs((v2.normalise()).magnitude()-1.0) < Vec3::EPSILON);
    }
}


BOOST_AUTO_TEST_CASE(TestVec3_CrossProduct)
{
    for(unsigned i=0;i<100000;++i)
    {
        const double a = real_rand();
        const double b = real_rand();
        const double c = real_rand();
        const double d = real_rand();
        const double e = real_rand();
        const double f = real_rand();

        const Vec3 v1(a, b, c);
        const Vec3 v2(d, e, f);
        auto v1CrossV2 = Vec3::crossProduct(v1,v2);
        BOOST_ASSERT(std::abs(Vec3::dotProduct(v1CrossV2, v1)) < 1.0e-14);
        BOOST_ASSERT(std::abs(Vec3::dotProduct(v1CrossV2, v2)) < 1.0e-14);
    }
}


BOOST_AUTO_TEST_CASE(TestVec3_Addition)
{
    for(unsigned i=0;i<10000;++i)
    {
        const double a = real_rand();
        const double b = real_rand();
        const double c = real_rand();
        const double d = real_rand();
        const double e = real_rand();
        const double f = real_rand();

        const Vec3 v1(a, b, c);
        const Vec3 v2(d, e, f);
        auto v3 = v1 + v2;
        BOOST_ASSERT(v3.X() == a+d);
        BOOST_ASSERT(v3.Y() == b+e);
        BOOST_ASSERT(v3.Z() == c+f);
    }
}

BOOST_AUTO_TEST_CASE(TestVec3_ScalarMultiplication)
{
    for(unsigned i=0;i<100000;++i)
    {
        const double a = real_rand();
        const double b = real_rand();
        const double c = real_rand();

        const Vec3 v1(a, b, c);
        const auto scalar = real_rand();
        auto v3 = v1*scalar;
        BOOST_ASSERT(v3.X() == a*scalar);
        BOOST_ASSERT(v3.Y() == b*scalar);
        BOOST_ASSERT(v3.Z() == c*scalar);
    }
}


BOOST_AUTO_TEST_CASE(TestVec3_IsSame)
{
    for(unsigned i=0;i<100000;++i)
    {
        const double a = real_rand();
        const double b = real_rand();
        const double c = real_rand();
        const double d = real_rand();
        const double e = real_rand();
        const double f = real_rand();

        const Vec3 v1(a, b, c);
        const Vec3 v2(d, e, f);
        Vec3 v3 = v1*real_rand();
        Vec3 v4 = v1;
        BOOST_ASSERT(!v3.isSameAs(v1));
        BOOST_ASSERT(v4.isSameAs(v1));
    }
}
