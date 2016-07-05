#include <IShape.h>
using namespace std;
using namespace rabbit;

namespace
{
    struct float4
    {
        float w,x,y,z;
    };

    struct TrianglePropsMinimal
    {
        enum vid :unsigned {v0 = 0, v1 = 1, v2 = 2};
        enum eid :unsigned {e0 = 0, e1 = 1, e2 = 2};
        static const unsigned NUM_VERTICES = 3;
    };

    /*
    struct Triangle : public IShape<float4
    {

    };


*/

}

int main()
{


}
