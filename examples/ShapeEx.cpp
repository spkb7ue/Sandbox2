#include <Vec3.h>
#include <Triangle.h>
#include <random>
#include <functional>
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

    mt19937::result_type seed = time(0);
    auto real_rand = std::bind(std::uniform_real_distribution<double>(0,1), mt19937(seed));
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

    {   // Can also create a triangle using other types
        // This instance will not support all the functions until
        // all the associated mathematical operators/operations are
        // implemented in float3
        float3 p0, p1, p2; ///< Verts
        float3 e0, e1, e2; ///< Edges
        float3 normal;
        p0.x = 10.0; p0.y = 20.0; p0.z = 30.0;

        // Since the mathematical functions required to calculate
        // edges/normal are not defined in float3, we specify the
        // values explicitly in ctor.
        Triangle<float3> trianglef3(p0,p1,p2,e0,e1,e2, normal);
        cout<<trianglef3.GetVerts(ids::zero).x<<","<<trianglef3.GetVerts(ids::zero).y<<","<<trianglef3.GetVerts(ids::zero).z<<endl;

        //trianglef3.GetVerts(3); ///< This line causes compile error.
    }

    {   // easily create shapes
        enum pt_ids{
            zero = 0,
            one = 1,
            two = 2,
            three = 3,
            four = 4
        };

        Polygon<float4, 5, pt_ids> pentagon;
        pentagon.GetVerts(pt_ids::zero).w = 10.0; // etc.
    }
}
