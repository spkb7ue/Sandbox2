#include <vector>
#include <iostream>
#include "Mesh.h"
#include "TriangularMeshBuildingPolicy.h"
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
}

BOOST_AUTO_TEST_CASE(TestMesh_TestCtorV)
{
    try
    {
        auto buildingPolicy = GetMeshBuildingPolicy(FILE_NAME);
        Mesh<Triangle<Vec3>> mesh(buildingPolicy);

        // Make sure we have loaded the right number of triangles
        std::vector<Triangle<Vec3>>& triangles = mesh.GetPolygons();
        BOOST_ASSERT(triangles.size() == NUM_TRIANGLES);
    }
    catch(...)
    {
        std::string error = "Unable to construct Mesh from file: " + FILE_NAME;
        BOOST_FAIL(error.c_str());
    }
}
