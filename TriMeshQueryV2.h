#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
#include "Vec3.h"
#include "AABB.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

class TriMeshProxQueryV2 : public IProximityQueries<Triangle<Vec3>, TriMeshProxQueryV2>
{
public:
    TriMeshProxQueryV2(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

    std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold);

private:

    void Preprocess();
    std::vector< AABB<Vec3> > m_aabb;
};

}
