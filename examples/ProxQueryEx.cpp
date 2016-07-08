#include <Vec3.h>
#include <Triangle.h>
#include <random>
#include <memory>
#include <Mesh.h>
#include <TriangularMeshBuildingPolicy.h>
#include <TriMeshProxQueryV1.h>
#include <TriMeshProxQueryV2.h>
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
    TriMeshProxQueryV1 proximityQueriesV1(mesh);
    TriMeshProxQueryV2 proximityQueriesV2(mesh);
    Vec3 point;
    double dist;
    bool foundPoint;

    // I already know that the closest dist from this point to the rabbit mesh is around 0.5
    // This was calculated from a third party software.
    Vec3 testPoint = Vec3(0.5204630973461957,   0.7220916475699011,   0.0396895110889990);
    Vec3 expectedClosestPoint = Vec3(0.0693250000000000,   0.5797399900000000,  -0.1722300000000000);
    double expectedDist = 0.518362283032093;

    std::tie(point, dist, foundPoint) = proximityQueriesV1.CalculateClosestPoint(testPoint, 0.6);
    cout<<"Expected Dist:"<<expectedDist<<endl;
    cout<<"Calculated Dist using V1:"<<dist<<endl;

    // Now calculate using V2
    std::tie(point, dist, foundPoint) = proximityQueriesV2.CalculateClosestPoint(testPoint, 0.6);
    cout<<"Calculated Dist using V2:"<<dist<<endl;


}
