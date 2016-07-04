#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "Mesh.h"
#include "Triangle.h"
#include <type_traits>
using namespace std;
using namespace rabbit;
namespace
{
const std::string fileName = "rabbit.triangles";

}

int main()
{
    Mesh mesh(fileName);
    Point p(0.0, 50.0, 2.0);
    fstream file;
    file.open("p.txt", ios::app | ios::out);
    for(const RigidTriangle& t : mesh.m_triangles)
    {
        auto op = t.CalcShortestDistanceFrom(p);
        file<<op.Dist<<"\t"<<op.P.X()<<"\t"<<op.P.Y()<<"\t"<<op.P.Z()<<endl;
    }
    file.close();
}
