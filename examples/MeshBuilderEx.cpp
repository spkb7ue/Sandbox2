#include <iostream>
#include <IMeshBuildingPolicy.h>
#include <Triangle.h>
#include <Mesh.h>

using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";

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

// Dummy mesh builder.
class DummyMeshBuildingPolicy : public IMeshBuildingPolicy<RigidTriangle>
{
    virtual void GeneratePolygons(std::vector<RigidTriangle>& triangles) override
    {

    }
};

// We can create a Mesh with any custom polygon type
// Here is an example
class CustomShapeMeshBuildingPolicy : public IMeshBuildingPolicy<Quad>
{
    virtual void GeneratePolygons(std::vector<Quad>& triangles) override
    {

    }
};


int main()
{
    {
        // Can be used to create an empty mesh object with polygon type RigidTriangle
        std::shared_ptr< IMeshBuildingPolicy<RigidTriangle> > dummy = std::make_shared<DummyMeshBuildingPolicy>();
        Mesh<RigidTriangle> mesh(dummy);
    }

    {
        std::shared_ptr< IMeshBuildingPolicy<Quad> > quadMeshBuilder = std::make_shared<CustomShapeMeshBuildingPolicy>();
        Mesh<Quad> mesh(quadMeshBuilder);
    }
}
