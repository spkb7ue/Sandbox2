#include <fstream>
#include <memory>
#include <iomanip>
#include <iterator>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>
#include <string>
#include "AABB.h"
#include "Vec3.h"
#include "Mesh.h"
#include "TriMeshQueryV3.h"
#include "TriangularMeshBuildingPolicy.h"
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

    // Test if the object can be constructed
    auto buildingPolicy = GetMeshBuildingPolicy(FILE_NAME);
    std::shared_ptr<TriMesh> mesh = std::make_shared<TriMesh>(buildingPolicy);
    TriMeshProxQueryV3 proximityQueries(mesh);

    Vec3 point;
    double dist;
    bool foundPoint;

    // I already know that the closest dist from this point to the rabbit mesh is around 0.5
    // This was calculated from a third party software.
    Vec3 testPoint = Vec3(0.5204630973461957,   0.7220916475699011,   0.0396895110889990);
    Vec3 expectedClosestPoint = Vec3(0.0693250000000000,   0.5797399900000000,  -0.1722300000000000);
    double expectedDist = 0.518362283032093;

    std::tie(point, dist, foundPoint) = proximityQueries.CalculateClosestPoint(testPoint, 10.6);
    cout<<point<<endl;
    cout<<foundPoint<<endl;
    cout<<testPoint<<endl;
    cout<<dist<<endl;
}
