#pragma once
#include "IProximityQueries.h"
#include "Triangle.h"
#include "Vec3.h"
#include "AABB.h"
#include "BNode.h"
#include "NodeData.h"
namespace rabbit
{

template<typename PolygonType>
class Mesh;

class TriMeshProxQueryV3 : public IProximityQueries<Triangle<Vec3>, TriMeshProxQueryV3>
{
public:

    typedef AABB<Vec3> AABB3;
    typedef Triangle<Vec3> Tri3;
    typedef BNode<NodeData> BVHNode;

    TriMeshProxQueryV3(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

    std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold);

    ~TriMeshProxQueryV3();

private:

	// Calculate the bounding box for all the triangles in the mesh
    void Preprocess();

    void PartitionMesh();

    std::vector<AABB<Vec3>> m_aabb; ///< vector of bounding boxes for all the triangles in the mesh.
                                    ///< These are precomputed so that no run-time costs are incurred.

    std::vector<BVHNode*> m_bvhTreeNodes;

};

}
