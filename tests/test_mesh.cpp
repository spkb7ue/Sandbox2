#include <vector>
#include "Mesh.h"
#define BOOST_TEST_MODULE Test_Mesh
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";
}


BOOST_AUTO_TEST_CASE(TestMesh_TestCtor)
{
    try
    {
        Mesh mesh(fileName);
        //std::vector<Triangle>& meshTriangles = mesh.GetTriangles();

    }
    catch(...)
    {
        std::string error = "Mesh constructor failed while loading file: " + fileName;
        BOOST_FAIL(error.c_str());
    }

}
