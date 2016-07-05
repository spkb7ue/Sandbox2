#include <fstream>
#include <memory>
#include <iomanip>
#include <iterator>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>
#include "Mesh.h"
#include "TriangularMeshBuildingPolicy.h"
#include <string>

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
    std::shared_ptr<TriangularMeshBuilingPolicy> buildingPolicy = std::make_shared<TriangularMeshBuilingPolicy>(fileName);
    Mesh<Triangle<Vec3>> mesh{buildingPolicy};
    std::vector<Triangle<Vec3>>& triangles = mesh.GetPolygons();

    Vec3 v0(-0.71519500999999996,  -0.22722999999999999,   0.07012000000000000);
    Vec3 v1(-0.68001502999999996,  -0.24351000000000000,  -0.00998000000000000);
    Vec3 v2(-0.63365501000000002,  -0.34187000000000001,  -0.01984000000000000);

    Triangle<Vec3> t(v0,v1,v2);
    Vec3 testPoint(0.521829221900000,   0.244939351700000,  0.445484475100000);
    auto px = t.CalcShortestDistanceFrom(testPoint);
    cout<<px.Dist<<endl;
    cout<<px.Point<<endl;
    return 1;

    fstream testDataFile, outputDataFile;
    testDataFile.open("td.txt",ios::app|ios::out);
    outputDataFile.open("op1.txt",ios::app|ios::out);
    for(unsigned i=0;i<1;++i)
    {
        double a = real_rand();
        double b = real_rand();
        double c = real_rand();
        Vec3 testPoint(0.521829221900000,   0.244939351700000,  0.445484475100000);
        //testDataFile<<setw(10)<<setprecision(10)<<a<<"\t"<<b<<"\t"<<c<<endl;

        for(Triangle<Vec3>& t : triangles)
        {
            IntersectionResult<Vec3> res = t.CalcShortestDistanceFrom(testPoint);
            outputDataFile<<setw(10)<<setprecision(10)<<res.Dist<<"\t"<<res.Point.X()<<"\t"<<res.Point.Y()<<"\t"<<res.Point.Z()<<endl;
        }
    }

    outputDataFile.close();
    testDataFile.close();

   // RigidTriangle::VertType point(1.3f, 0.0f, 2.0f);
    //TriangleMeshBuilder b;
    ///Mesh2<RigidTriangle, TriangleMeshBuilder> mesh(b);
}
