#include <iostream>
#include <IMeshBuildingPolicy.h>
#include <Mesh.h>

using namespace std;
using namespace rabbit;
namespace
{

    struct Point3
    {
        float x,y,z;
    };

    // Custom Shape. A quadrilateral with 4 verts of type Point3
    struct Quad
    {
        Point3 m_verts[4];
    };

}

// We can create a Mesh with any custom polygon type. Here is an example
class CustomShapeMeshBuildingPolicy : public IMeshBuildingPolicy<Quad>
{
    virtual void GeneratePolygons(std::vector<Quad>& triangles) override
    {

    }
};


int main()
{
    // Here we create a mesh consisting of polygon Quad.
    std::shared_ptr< IMeshBuildingPolicy<Quad> > quadMeshBuilder = std::make_shared<CustomShapeMeshBuildingPolicy>();
    Mesh<Quad> mesh(quadMeshBuilder);

}
