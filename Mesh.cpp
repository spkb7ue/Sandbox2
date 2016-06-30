#include "Mesh.h"
#include <fstream>

using namespace rabbit;
using namespace std;

Mesh::Mesh(std::string fileName)
{
    ifstream infile;
    infile.open(fileName.c_str());
    Point P0, P1, P2;
    while(infile >> P0.x >> P0.y >> P0.z
                 >> P1.x >> P1.y >> P1.z
                 >> P2.x >> P2.y >> P2.z
          )
    {
        m_triangles.emplace_back(Triangle(P0, P1, P2));
    }
    infile.close();

}
