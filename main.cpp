#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Mesh.h"
using namespace std;
struct Point{
    float x,y,z;
};

int main()
{
    rabbit::Mesh m;
	cout<<"Hellp\n";
	ifstream infile;
    infile.open("rabbit.triangles");
    std::vector<Point> points;

    Point tmp;
    while(infile >> tmp.x >> tmp.y >> tmp.z)
    {
        points.push_back(tmp);
    }
    infile.close();
    cout<<points.size()<<endl;
}
