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

BOOST_AUTO_TEST_CASE(TestMesh_TestCtorV1)
{
    try
    {
        auto buildingPolicy = GetMeshBuildingPolicy(FILE_NAME);
        Mesh<RigidTriangle> mesh(buildingPolicy);

        // Make sure we have loaded the right number of triangles
        std::vector<RigidTriangle>& triangles = mesh.GetPolygons();
        BOOST_ASSERT(triangles.size() == NUM_TRIANGLES);
    }
    catch(...)
    {
        std::string error = "Mesh constructor failed while loading file: " + FILE_NAME;
        BOOST_FAIL(error.c_str());
    }
}

BOOST_AUTO_TEST_CASE(TestMesh_TestCtorV2)
{
    try
    {
        auto buildingPolicy = GetMeshBuildingPolicy("DummyFile");
        Mesh<RigidTriangle> mesh(buildingPolicy);
        std::string error = "rabbit::Mesh ctor should have thrown as DummyFile does not exist";
        BOOST_FAIL(error.c_str());
    }
    catch(...)
    {
        // If the constructor throws, then the test has passed
    }
}
