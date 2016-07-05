#include <fstream>
#include <memory>
#include <iterator>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>
#include "Mesh.h"
#include "TriangularMeshBuildingPolicy.h"
#include <string>

using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";
    mt19937::result_type seed = time(0);
    auto real_rand = std::bind(std::uniform_real_distribution<double>(0,5), mt19937(seed));
}

int main()
{


   // RigidTriangle::VertType point(1.3f, 0.0f, 2.0f);
    //TriangleMeshBuilder b;
    ///Mesh2<RigidTriangle, TriangleMeshBuilder> mesh(b);
}
