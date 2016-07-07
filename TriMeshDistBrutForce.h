#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
#include "Vec3.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

class TriMeshBruteForce : public IProximityQueries<Triangle<Vec3>, TriMeshBruteForce>
{
public:
    TriMeshBruteForce(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

    std::pair<Vec3,double> CalculateClosestPointImpl(const Vec3& point);
};

}
