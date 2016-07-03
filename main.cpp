#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "Mesh.h"
#include "Triangle.h"
#include <type_traits>
using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";
}

int main()
{
    Point p;
    Point q; q.Y() = 10.0f;
//    TriangleProps<Tri<Vec3,true>> t(p,q,p);
    // t.m_data.Vert(1) = q*3.0f;
    // cout<<t.m_data.Vert(1)<<endl;
}
