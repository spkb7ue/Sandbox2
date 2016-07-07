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
#include "TriMeshQueryV2.h"
using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";
    mt19937::result_type seed = time(0);
    auto real_rand = std::bind(std::uniform_real_distribution<double>(0,5), mt19937(seed));
}

int main()
{
    AABB<Vec3> aabb(Vec3(), Vec3());
}
