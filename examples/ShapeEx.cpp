#include <IShape.h>
using namespace std;
using namespace rabbit;

namespace
{
    struct float4
    {
        float w,x,y,z;
    };

    struct float3
    {
        float x,y,z;
    };

    struct TrianglePropsMinimal
    {
        enum VertIndices :unsigned {v0 = 0, v1 = 1, v2 = 2};
        enum EdgeIndices :unsigned {e0 = 0, e1 = 1, e2 = 2};
        struct PointIntersectionResult{};
        static const unsigned NUM_VERTICES = 3;
    };

/*
    // Triangle which can't be modified
    struct rTriangle : public IShape<float4, false, rTriangle, TrianglePropsMinimal>
    {
        rTriangle(float4& v0, float4& v1, float4& v2):IShape<float4,
                                                             false,
                                                             rTriangle,
                                                             TrianglePropsMinimal>
                                                             (v0, v1, v2){}

    };


    */


}

int main()
{


}
