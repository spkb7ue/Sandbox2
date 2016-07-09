#include "TriMeshProxQueryV3.h"
#include "Mesh.h"
#include "NodeData.h"
#include <algorithm>
#include <iostream>
using namespace std;

using namespace rabbit;
namespace
{
    typedef Triangle<Vec3> Tri;
    typedef IntersectionResult<Vec3> IntRes;
}

TriMeshProxQueryV3::TriMeshProxQueryV3(std::shared_ptr<Mesh<Tri>> mesh):
        IProximityQueries<Tri, TriMeshProxQueryV3>(mesh)
{
    Preprocess();
}

TriMeshProxQueryV3::~TriMeshProxQueryV3()
{
    std::for_each(m_bvhTreeNodes.begin(), m_bvhTreeNodes.end(),[](BVHNode* nd){
        if(nd != nullptr) delete nd;
    });
    m_bvhTreeNodes.clear();
}

void TriMeshProxQueryV3::PartitionMesh()
{
    {   // Create root node
        AABB3 totalBoundingBox = AABB<Vec3>::CalculateAABB(m_aabb);
        NodeData rootNodeData(totalBoundingBox);
        for(unsigned i=0;i<m_aabb.size();++i){rootNodeData.polygons.push_back(i);}
        BVHNode *rootNode = new BVHNode(rootNodeData);
        m_bvhTreeNodes.push_back(rootNode);
    }
}

void TriMeshProxQueryV3::Preprocess()
{
    const std::vector<Tri>& triangles = m_mesh->GetPolygons();
    std::for_each(triangles.begin(), triangles.end(), [this](const Tri& t)
    {
        m_aabb.emplace_back(t.CalculateAABB());
    });

    PartitionMesh();
}

std::tuple<Vec3,double,bool> TriMeshProxQueryV3::CalculateClosestPointImpl(const Vec3& point,double distThreshold)
{
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
    Vec3 closestPoint;

    double minDist = distThreshold;
    bool foundPoint = false;

    for(unsigned i = 0; i < m_aabb.size(); ++i)
    {
        IntRes resAABB = m_aabb[i].CalcShortestDistanceFrom(point, minDist);
        if(resAABB.Dist < minDist)
        {   // We only bother looking at the triangle if the distance to
            // AABB is less than the threshold

            IntersectionResult<Vec3> resTri = triangles[i].CalcShortestDistanceFrom(point, minDist);
            if(resTri.Dist < minDist)
            {
                // Now that we have found atleast one point to the mesh,
                // we update the minDist
                minDist = resTri.Dist;
                closestPoint = resTri.Point;
                foundPoint = true;
            }
        }
    }

    return std::make_tuple(closestPoint, minDist, foundPoint);
}
