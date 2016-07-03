#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "Mesh.h"
#include "Triangle.h"
using namespace std;
using namespace rabbit;
namespace
{
    const std::string fileName = "rabbit.triangles";
}

int main()
{
    Mesh mesh(fileName);
    std::vector<Triangle>& triangles = mesh.GetTriangles();
    Point origin(0.0, 0.0, 0.0);
    std::fstream file;
    file.open("f.txt", ios::app | ios::out);
    for(const Triangle& t: triangles)
    {
        Point P;
        double dist;
        std::tie(P, dist) = t.CalcShortestDistanceFrom(origin);
        file<<dist << "\t"<<P.X()<<"\t"<<P.Y()<<"\t"<<P.Z()<<endl;
    }
    file.close();

}
