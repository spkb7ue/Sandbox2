#include <vector>
#define BOOST_TEST_MODULE TEST_Mesh
#include <boost/test/unit_test.hpp>

using namespace std;

namespace
{
struct SomeData
{
    SomeData()
    {
        x = 20.0;
        y = 2;
        z = nullptr;
    }
  double x;
  int y;
  char *z;
};

}

// Tests that the Foo::Bar() method does Abc.
BOOST_AUTO_TEST_CASE(TestDCTree_TestCtor)
{
	int i = 1;
}
