#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

class TriMeshBruteForce : public IProximityQueries<Triangle<Vec3>, TriMeshBruteForce>
{
public:
    TriMeshBruteForce(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh):
        IProximityQueries<Triangle<Vec3>, TriMeshBruteForce>(mesh){}

        Vec3 CalculateClosestPointImpl(const Vec3& point);
};

}
