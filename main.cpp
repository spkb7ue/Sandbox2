#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Mesh.h"
using namespace std;
struct Point{
    float x,y,z;
};

namespace
{
    const std::string fileName = "rabbit.triangles";
}

int main()
{
    rabbit::Mesh mesh(fileName);
}
