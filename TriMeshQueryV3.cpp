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

    // Debug function. Just print out the tree to see its structure.
    void printIndices(TriMeshProxQueryV3::AABBNode* node)
    {
        if(node == nullptr)
        {
            return;
        }
        cout<<"P:"<<node->Data().triIndices.size();
        if(node->GetLeft()!= nullptr)
        {
            cout<<"\t C1:"<<node->GetLeft()->Data().triIndices.size();
        }

        if(node->GetRight()!= nullptr)
        {
            cout<<"\t C2:"<<node->GetRight()->Data().triIndices.size();
        }
        cout<<"\n------------------\n";

        printIndices(node->GetLeft());
        printIndices(node->GetRight());
    }
}

TriMeshProxQueryV3::TriMeshProxQueryV3(std::shared_ptr<Mesh<Tri>> mesh):
        IProximityQueries<Tri, TriMeshProxQueryV3>(mesh)
{
    Preprocess();
}

void TriMeshProxQueryV3::CalculateLeastDistRecursive(const Vec3& point,
                                                     double& minDist,
                                                     TriMeshProxQueryV3::AABBNode* node,
                                                     Vec3& closestPoint,
                                                     bool& found )
{

    double distToLeft = std::numeric_limits<double>::max();
    double distToRight = std::numeric_limits<double>::max();
    bool canGoLeft = false;
    bool canGoRight = false;
    if(node->GetLeft() != nullptr)
    {
        IntersectionResult<Vec3> res = node->GetLeft()->Data().aabb.CalcShortestDistanceFrom(point, minDist);
        node->GetLeft()->Data().dist = res.Dist;
        canGoLeft = res.Dist < minDist;
    }

    if(node->GetRight() != nullptr)
    {
        IntersectionResult<Vec3> res = node->GetRight()->Data().aabb.CalcShortestDistanceFrom(point, minDist);
        node->GetRight()->Data().dist = res.Dist;
        canGoRight = res.Dist < minDist;
    }

    if(canGoLeft || canGoRight)
    {
        if(node->GetLeft()->Data().dist < node->GetRight()->Data().dist)
        {
            CalculateLeastDistRecursive(point, minDist, node->GetLeft(), closestPoint, found);
        }
        else
        {
            CalculateLeastDistRecursive(point, minDist, node->GetRight(), closestPoint, found);
        }
    }


    if(node->GetLeft() == nullptr && node->GetRight() == nullptr)
    {
        // This means we have reached the leaf node.
        const std::vector<int>& indices = node->Data().triIndices;
        if(indices.size() == 0)
        {
            // something has gone wrong
            throw;
        }

        const std::vector<Tri>& triangles = m_mesh->GetPolygons();
        // Ok so we have calculated the

        for(int i=0;i<indices.size();++i)
        {
            // As Before, first do a check with the bounding box
            IntersectionResult<Vec3> resAABB = m_aabb[i].CalcShortestDistanceFrom(point, minDist);
            if(resAABB.Dist < minDist)
            {   // We only bother looking at the triangle if the distance to
                // AABB is less than the threshold
                IntersectionResult<Vec3> resTri = triangles[i].CalcShortestDistanceFrom(point, minDist);
                if(resTri.Dist < minDist)
                {
                    // Now that we have found atleast one point to the mesh,
                    // we update the minDist. This has a cumulative
                    minDist = resTri.Dist;
                    closestPoint = resTri.Point;
                    found = true;
                }
            }
        }
    }

    if(found)
    {
        TriMeshProxQueryV3::AABBNode* parent = node->GetParent();
        if(parent != nullptr)
        {
            parent->Data().dist = minDist;
            TriMeshProxQueryV3::AABBNode* sisterNode = parent->GetLeft() == node ? parent->GetRight() : parent->GetLeft();
            if(sisterNode->Data().dist < minDist)
            {
                CalculateLeastDistRecursive(point, minDist,sisterNode,closestPoint,found);
            }
        }
    }
}

std::tuple<Vec3,double,bool> TriMeshProxQueryV3::CalculateClosestPointImpl(const Vec3& point,double distThreshold)
{
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
    Vec3 closestPoint;
    bool foundPoint = false;
    double minDist = distThreshold;

    CalculateLeastDistRecursive(point, minDist, m_aabbTree[0],closestPoint, foundPoint);

    return std::make_tuple(closestPoint, minDist, foundPoint);
}

void TriMeshProxQueryV3::Preprocess()
{
    const std::vector<Tri>& triangles = m_mesh->GetPolygons();
    std::for_each(triangles.begin(), triangles.end(), [this](const Tri& t)
    {
        m_aabb.emplace_back(t.CalculateAABB());
    });

    // Create root node data
    NodeData rootNode(AABB3::CalculateAABB(m_aabb));
    rootNode.triIndices.resize(triangles.size());
    for(int i=0;i<rootNode.triIndices.size();++i){rootNode.triIndices[i] = i;}

    AABBNode* nd = new AABBNode(rootNode);
    m_aabbTree.push_back(nd);

    TriMeshProxQueryV3::AABBNode* c1(nullptr);
    TriMeshProxQueryV3::AABBNode* c2(nullptr);
    std::tie(c1,c2) = GenerateNodes(nd);
    nd->SetChildren(c1, c2);
}

TriMeshProxQueryV3::~TriMeshProxQueryV3()
{
    std::for_each(m_aabbTree.begin(), m_aabbTree.end(),[this](AABBNode* nd)
    {
        if(nd != nullptr)
            delete nd;
    });
    m_aabbTree.clear();
}

std::pair<TriMeshProxQueryV3::AABBNode*, TriMeshProxQueryV3::AABBNode*>
TriMeshProxQueryV3::GenerateNodes(AABBNode* parent)
{

    TriMeshProxQueryV3::AABBNode* child1AABBNode = nullptr;
    TriMeshProxQueryV3::AABBNode* child2AABBNode = nullptr;
    if(parent == nullptr || parent->Data().triIndices.size() == 1)
    {
        return std::make_pair(nullptr, nullptr);
    }

    NodeData parentData = parent->Data();
    double dim;
    int index;
    std::tie(dim,index) = parentData.aabb.GetLargestDim();

    Bounds boundsChild_1 = RecalculateBounds(parentData.aabb.GetBounds(), dim/2.0, index);

    // Create new bounding box for the child one.
    AABB<Vec3> aabbChild_1(boundsChild_1);

    std::vector<int> child1Indices;
    std::vector<int> child2Indices;

    PopulateChildIndices(child1Indices, child2Indices, aabbChild_1, parentData.triIndices);

    if(child1Indices.size() + child2Indices.size() != parentData.triIndices.size())
    {
        throw std::runtime_error("Found a bug. More unit tests....");
    }

    if(child1Indices.size() > 0)
    {
        NodeData child1NodeData(aabbChild_1);
        child1NodeData.triIndices = child1Indices;
        child1AABBNode = new TriMeshProxQueryV3::AABBNode(child1NodeData);
        m_aabbTree.push_back(child1AABBNode);
    }

    if(child2Indices.size() > 0 && child2Indices.size() != parentData.triIndices.size())
    {

        // Calculate AABB of child 2.
        std::vector<AABB3> child2aabbs;
        std::for_each(child2Indices.begin(), child2Indices.end(),[this, &child2aabbs](const int index)
        {
            child2aabbs.push_back(m_aabb[index]);
        });

        NodeData child2Node(AABB3::CalculateAABB(child2aabbs));
        child2Node.triIndices = child2Indices;
        child2AABBNode = new TriMeshProxQueryV3::AABBNode(child2Node);
        m_aabbTree.push_back(child2AABBNode);
    }

    if(child1AABBNode != nullptr)
    {
        TriMeshProxQueryV3::AABBNode* c1(nullptr);
        TriMeshProxQueryV3::AABBNode* c2(nullptr);
        std::tie(c1,c2) = GenerateNodes(child1AABBNode);
        child1AABBNode->SetChildren(c1, c2);
    }

    if(child2AABBNode != nullptr)
    {
        TriMeshProxQueryV3::AABBNode* c1(nullptr);
        TriMeshProxQueryV3::AABBNode* c2(nullptr);
        std::tie(c1,c2) = GenerateNodes(child2AABBNode);
        child2AABBNode->SetChildren(c1, c2);
    }

    return std::make_pair(child1AABBNode, child2AABBNode);
}

void TriMeshProxQueryV3::PopulateChildIndices(std::vector<int>& child1Indices,
                                               std::vector<int>& child2Indices,
                                               const AABB3& child1AABB,
                                               const std::vector<int>& parentIndices)
{

    auto isTriangleWithinAABB = [](const Triangle<Vec3>& tri, const AABB<Vec3>& aabb)->bool
    {
        return  aabb.IsPointWithinAABB(tri.P0()) &&
                aabb.IsPointWithinAABB(tri.P1()) &&
                aabb.IsPointWithinAABB(tri.P2());
    };


    const std::vector<Tri>& triangles = m_mesh->GetPolygons();

    for(int i=0;i<parentIndices.size(); ++i)
    {
        const int triangleIndex = parentIndices[i];
        if(isTriangleWithinAABB(triangles[triangleIndex], child1AABB))
        {
            child1Indices.push_back(triangleIndex);
        }
        else
        {
            child2Indices.push_back(triangleIndex);
        }
    }
}

Bounds TriMeshProxQueryV3::RecalculateBounds(const Bounds& bound, double dim, int index)
{
    Bounds val = bound; // First copy the existing bounds.
    if(index == 0)
    {   // only x values of the bound need to be updated
        val.xMax = val.xMin + dim;
    }
    else if (index == 1)
    {
        val.yMax = val.yMin + dim;
    }
    else if(index == 2)
    {
        val.zMax = val.zMin + dim;
    }
    else
    {
        // can't ever get here
        throw;
    }
    return val;
}
