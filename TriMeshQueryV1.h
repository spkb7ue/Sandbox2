#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
#include "Vec3.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

class TriMeshProxQueryV1 : public IProximityQueries<Triangle<Vec3>, TriMeshProxQueryV1>
{
public:
    TriMeshProxQueryV1(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

    std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold);
};

}