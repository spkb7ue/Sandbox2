#include <fstream>
#include <system_error>"
#include "TriangularMeshBuilingPolicy.h"
using namespace rabbit;

TriangularMeshBuilingPolicy::TriangularMeshBuilingPolicy(std::string fileName):m_fileName(fileName){}

void TriangularMeshBuilingPolicy::GeneratePolygons(std::vector<RigidTriangle>& triangles)
{
    ifstream infile;
    infile.open(fileName.c_str());
    if(infile.fail())
    {
        throw std::runtime_error("Unable to open mesh file:" + fileName);
    }
    Point P0, P1, P2;
    while(infile >> P0.X() >> P0.Y() >> P0.Z()
                 >> P1.X() >> P1.Y() >> P1.Z()
                 >> P2.X() >> P2.Y() >> P2.Z()
          )
    {
        triangles.emplace_back(RigidTriangle(P0, P1, P2));
    }
    infile.close();
}
