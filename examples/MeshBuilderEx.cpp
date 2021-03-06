#include <IMeshBuildingPolicy.h>
#include <TriangularMeshBuildingPolicy.h>
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

// We can create a Mesh with any custom polygon type.
class CustomShapeMeshBuildingPolicy : public IMeshBuildingPolicy<Quad>
{
    virtual void GeneratePolygons(std::vector<Quad>& polygons) override
    {
        // The code to populate polygons goes here.
    }
};

int main()
{
    {   // Here we create a mesh consisting of polygon Quad.
        std::shared_ptr< IMeshBuildingPolicy<Quad> > quadMeshBuilder = std::make_shared<CustomShapeMeshBuildingPolicy>();
        std::shared_ptr<Mesh<Quad>> mesh = std::make_shared<Mesh<Quad>>(quadMeshBuilder);
    }

    {   // Lets do our regular triangular mesh now.
        const std::string FILE_NAME = "rabbit.triangles"; ///< File with the triangulated mesh
        auto triMeshBuilder = std::make_shared<TriangularMeshBuilingPolicy>(FILE_NAME);
    }
}
