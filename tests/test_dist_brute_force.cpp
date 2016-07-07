#include <vector>
#include <iostream>
#include "Mesh.h"
#include "TriangularMeshBuildingPolicy.h"
#include "TriMeshDistBrutForce.h"
#define BOOST_TEST_MODULE Test_Mesh
#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE(TestMesh_TestasdfadsfCtorV)
{
    auto buildingPolicy = GetMeshBuildingPolicy(FILE_NAME);
    std::shared_ptr<TriMesh> mesh = std::make_shared<TriMesh>(buildingPolicy);
    TriMeshBruteForce proximityQueries(mesh);
    Vec3 closestPoint;
    double dist;Vec3 point;
    std::tie(point, dist) = proximityQueries.CalculateClosestPoint(Vec3(0.5204630973461957,   0.7220916475699011,   0.0396895110889990));
    cout<<point<<endl;
}
