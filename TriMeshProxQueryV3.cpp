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

Bounds TriMeshProxQueryV3::RecalculateBounds(const Bounds& bounds, double dim, int index)const
{
    Bounds bd = bounds; // Create a copy first
    if(index == 0)
    {
        bd.xMax = bd.xMax + dim;
    }
    else if(index == 1)
    {
        bd.yMax = bd.yMin + dim;
    }
    else if(index == 2)
    {
        bd.zMax = bd.zMin + dim;
    }
    else
    {
        // should never get here.
        throw;
    }
    return bd;
}

void TriMeshProxQueryV3::RecursivePartition(BVHNode* node)
{
    double dim; int index;
    NodeData& nodeDat = node->Data();
    std::tie(dim, index) = nodeDat.aabb.GetLargestDim();

    // bounds for child 1
    Bounds boundsChild1 = RecalculateBounds(nodeDat.aabb.GetBounds(), dim/2.0, index);
    AABB3 aabb_child1(boundsChild1);
    std::vector<int> triangleIndicesChild1;
    std::vector<int> triangleIndicesChild2;
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();

    // Check which triangles are in first box, and which are in the second
    for(unsigned i=0;i<nodeDat.polygons.size();++i)
    {
        int triangleIndex = nodeDat.polygons[i];
        if(triangles[triangleIndex].IsTriangleInsideAABB(aabb_child1))
        {
            triangleIndicesChild1.push_back(triangleIndex);
        }
        else
        {
            triangleIndicesChild2.push_back(triangleIndex);
        }
    }

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
