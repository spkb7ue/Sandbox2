#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
#include "Vec3.h"
namespace rabbit
{

template<typename PolygonType> class Mesh;

/**
* This class implements the proximity query between point and a triangular mesh
* by calculating the distance between point and each triangle in the mesh.
* By far the most ineffienct approach.
*/
class TriMeshProxQueryV1 : public IProximityQueries<Triangle<Vec3>, TriMeshProxQueryV1>
{
public:
    TriMeshProxQueryV1(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

	/**
	*	See documentation for IProximityQueries<PolygonType, ProximityQueryMethod>::CalculateClosestPoint
	*/ 
    std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold);
};

}
