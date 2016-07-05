#include <Vec3.h>
#include <Triangle.h>
using namespace std;
using namespace rabbit;

namespace
{

struct float3
{
    float x,y,z;
};

}

int main()
{
    Vec3 p,q,r(1.0f, 2.0f, 1.0f);
    Triangle<Vec3>v1(p, q, r);
    v1.ProjectPointOntoShapePlane(q);
}
