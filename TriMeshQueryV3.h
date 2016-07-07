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
    typedef BNode<NodeData> AABBNode;

    TriMeshProxQueryV3(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh);

    std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold);
    ~TriMeshProxQueryV3();
private:

    void Preprocess();

    std::pair<AABBNode*, AABBNode*> GenerateNodes(AABBNode* parent);

    std::vector<AABB3> m_aabb;
    std::vector<AABBNode*> m_aabbTree;
};

}
