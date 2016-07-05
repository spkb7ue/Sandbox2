#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

class TriangularMeshProximityQueries : public IProximityQueries<Triangle<Vec3>, TriangularMeshProximityQueries>
{
public:
    TriangularMeshProximityQueries(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh):
        IProximityQueries<Triangle<Vec3>, TriangularMeshProximityQueries>(mesh){}

        Vec3 CalculateClosestPointImpl(const Vec3& point)
        {
            return point;
        }
};

}
