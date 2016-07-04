
#include <memory>
#include <iterator>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>
#include "IShape.h"
#include "IMesh.h"
#include "IMeshBuilder.h"
#include "Triangle.h"
#include <string>

using namespace std;
using namespace rabbit;
namespace
{
const std::string fileName = "rabbit.triangles";

struct TriangleMeshBuilder : public IMeshBuilder<RigidTriangle, TriangleMeshBuilder>
{
    void GeneratePolygons(std::vector<RigidTriangle>& polygons){
        RigidTriangle::VertType point(1.3f, 0.0f, 2.0f);
        polygons.emplace_back(RigidTriangle(point, point*3.0, point*2.0));
    }
};

}

int main()
{
    RigidTriangle::VertType point(1.3f, 0.0f, 2.0f);
    TriangleMeshBuilder b;
    Mesh2<RigidTriangle, TriangleMeshBuilder> mesh(b);
}
