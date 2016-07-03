#include <vector>
#include <iostream>
#include "Mesh.h"
#define BOOST_TEST_MODULE Test_Mesh
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;
namespace
{
    const std::string FILE_NAME = "rabbit.triangles"; ///< File with the triangulated mesh
    const unsigned NUM_TRIANGLES = 2204;    ///< Number of triangles in the mesh
}

BOOST_AUTO_TEST_CASE(TestMesh_TestCtorV1)
{
    try
    {
        Mesh mesh(FILE_NAME);

        // Make sure we have loaded the right number of triangles
        std::vector<Triangle>& triangles = mesh.GetTriangles();
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
        Mesh mesh("DummyFile");
        std::string error = "rabbit::Mesh ctor should have thrown as DummyFile does not exist";
        BOOST_FAIL(error.c_str());
    }
    catch(...)
    {
        // If the constructor throws, then the test has passed
    }
}
