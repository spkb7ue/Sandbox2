#include "TriMeshQueryV3.h"
#include "Mesh.h"
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


void TriMeshProxQueryV3::Preprocess()
{
    const std::vector<Tri>& triangles = m_mesh->GetPolygons();
    std::for_each(triangles.begin(), triangles.end(), [this](const Tri& t)
    {
        m_aabb.emplace_back(t.CalculateAABB());
    });

    BNode<

    NodeData rootNode(AABB3::CalculateAABB(m_aabb));
    rootNode.triIndices.resize(triangles.size());
    for(int i=0;i<rootNode.triIndices.size();++i){rootNode.triIndices[i] = i;}

}

TriMeshProxQueryV3::~TriMeshProxQueryV3()
{
    std::for_each(m_aabbTree.begin(), m_aabbTree.end(),[this](AABBNode* nd)
    {
        delete nd;
    });
    m_aabbTree.clear();
}
