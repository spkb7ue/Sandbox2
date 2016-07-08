#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
#include "Vec3.h"
#include "AABB.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

/**
* @brief This class implements the proximity query between point and a triangular mesh
* by calculating by first doing a check with the axis aligned bounding box for
* the triangle which is much cheaper than calculating the distance to the triangle.
* Somewhat better than TriMeshProxQueryV1, but still plenty of room for improvement
*/
class TriMeshProxQueryV2 : public IProximityQueries<Triangle<Vec3>, TriMeshProxQueryV2>
{
public:
    TriMeshProxQueryV2(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

   /**
	* See documentation for IProximityQueries<PolygonType, ProximityQueryMethod>::CalculateClosestPoint
	*/
    std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold);

private:

	// Calculate the bounding box for all the triangles in the mesh
    void Preprocess();

    std::vector<AABB<Vec3>> m_aabb; ///< vector of bounding boxes for all the triangles in the mesh.
									  ///< These are precomputed so that no run-time costs are incurred.
};

}
