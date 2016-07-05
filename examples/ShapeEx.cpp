#include <Vec3.h>
#include <Triangle.h>
#include <random>
using namespace std;
using namespace rabbit;

namespace
{
    // commonly used cuda structs for which nvcc compiler
    // already has predefined mathematical operations.

    struct float3
    {
        float x,y,z;
    };

    struct float4
    {
        float w,x,y,z;
    };

    static mt19937::result_type seed = time(0);
    static auto real_rand = std::bind(std::uniform_real_distribution<double>(0,1), mt19937(seed));
}

int main()
{
    {   // Create a triangle using Vec3 which already supports all the
        // required algebraic operations.
        Vec3 p(real_rand(),real_rand(),real_rand());
        Vec3 q(real_rand(),real_rand(),real_rand());
        Vec3 r(real_rand(),real_rand(),real_rand());

        Triangle<Vec3> triangle(p, q, r);
    }
}
