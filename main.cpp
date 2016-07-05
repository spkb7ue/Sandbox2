
#include <memory>
#include <iterator>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>
#include "IShape.h"
#include "Mesh.h"
#include "Triangle.h"
#include "TriangularMeshBuildingPolicy.h"
#include <string>

using namespace std;
using namespace rabbit;
namespace
{
const std::string fileName = "rabbit.triangles";

}

int main()
{
    std::shared_ptr<TriangularMeshBuilingPolicy> buildingPolicy = std::make_shared<TriangularMeshBuilingPolicy>(fileName);
    Mesh<RigidTriangle> mesh{buildingPolicy};
    mesh.GetPolygons();

   // RigidTriangle::VertType point(1.3f, 0.0f, 2.0f);
    //TriangleMeshBuilder b;
    ///Mesh2<RigidTriangle, TriangleMeshBuilder> mesh(b);
}
