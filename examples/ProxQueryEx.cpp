#include <Vec3.h>
#include <Triangle.h>
#include <random>
#include <memory>
#include <Mesh.h>
#include <TriangularMeshBuildingPolicy.h>
#include <TriMeshProxQueryV1.h>
#include <TriMeshProxQueryV2.h>
#include <TriMeshProxQueryV3.h>
using namespace std;
using namespace rabbit;

namespace
{
    const std::string FILE_NAME = "rabbit.triangles"; ///< File with the triangulated mesh
    const unsigned NUM_TRIANGLES = 2204;    ///< Number of triangles in the mesh

    std::shared_ptr<TriangularMeshBuilingPolicy> GetMeshBuildingPolicy(std::string fileName)
    {
        return std::make_shared<TriangularMeshBuilingPolicy>(fileName);
    }
    typedef Mesh<Triangle<Vec3>> TriMesh;
}

int main()
{
    auto buildingPolicy = GetMeshBuildingPolicy(FILE_NAME);
    std::shared_ptr<TriMesh> mesh = std::make_shared<TriMesh>(buildingPolicy);
    TriMeshProxQueryV3 proximityQueriesV3(mesh);
}
