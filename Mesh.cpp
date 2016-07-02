#include "Mesh.h"
#include <fstream>
#include <system_error>

using namespace rabbit;
using namespace std;

Mesh::Mesh(std::string fileName)
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
        m_triangles.emplace_back(Triangle(P0, P1, P2));
    }
    infile.close();
}

std::vector<Triangle>& Mesh::GetTriangles()
{
    return m_triangles;
}
