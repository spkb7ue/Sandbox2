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
}
