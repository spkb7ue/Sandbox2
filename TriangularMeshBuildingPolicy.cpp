#include <fstream>
#include <system_error>
#include "TriangularMeshBuildingPolicy.h"
using namespace rabbit;
using namespace std;

TriangularMeshBuilingPolicy::TriangularMeshBuilingPolicy(std::string fileName):m_fileName(fileName){}

void TriangularMeshBuilingPolicy::GeneratePolygons(std::vector<Triangle<Vec3>>& triangles)
{
    ifstream infile;
    infile.open(m_fileName.c_str());
    if(infile.fail())
    {
        throw std::runtime_error("Unable to open mesh file:" + m_fileName);
    }
    Point P0, P1, P2;
    while(infile >> P0.X() >> P0.Y() >> P0.Z()
                 >> P1.X() >> P1.Y() >> P1.Z()
                 >> P2.X() >> P2.Y() >> P2.Z()
          )
    {
        triangles.emplace_back(P0, P1, P2);
    }
    infile.close();
}
