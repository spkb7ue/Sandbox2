#include "TriMeshProxQueryV3.h"
#include "Mesh.h"
#include "NodeData.h"
#include <algorithm>
#include <iostream>
using namespace std;

using namespace rabbit;
namespace
{
    typedef IntersectionResult<Vec3> IntRes;
    typedef AABB<Vec3> AABB3;
    typedef Triangle<Vec3> Tri;
    typedef BNode<NodeData> BVHNode;
	typedef IntersectionResult<Vec3> IRes;

	void PrintNodeTriangles(BVHNode *node)
	{
		if (node == nullptr)
		{
			return;
		}
		cout << node->Data().indices.size();
		BVHNode* left = node->GetLeft();
		BVHNode* right = node->GetRight();
		if (left != nullptr)
		{
			cout << "\t" << left->Data().indices.size();
		}
		if (right != nullptr)
		{
			cout << "\t" << right->Data().indices.size();
		}
		cout << "\n----------------\n";
		cin.get();
		PrintNodeTriangles(left);
		PrintNodeTriangles(right);
	}
}

TriMeshProxQueryV3::TriMeshProxQueryV3(std::shared_ptr<Mesh<Tri>> mesh):
        IProximityQueries<Tri, TriMeshProxQueryV3>(mesh)
{
    Preprocess();
	PrintNodeTriangles(m_bvhTreeNodes[0]);
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
        bd.xMax = bd.xMin + dim;
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
    TriMeshProxQueryV3::AABB3 aabb_child1(boundsChild1);
    std::vector<int> triangleIndicesChild1;
    std::vector<int> triangleIndicesChild2;
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();

    // Check which triangles are in first box, and which are in the second
    for(unsigned i=0;i<nodeDat.indices.size();++i)
    {
        int triangleIndex = nodeDat.indices[i];
        if(triangles[triangleIndex].IsTriangleInsideAABB(aabb_child1)){
            triangleIndicesChild1.push_back(triangleIndex);
        }
        else{
            triangleIndicesChild2.push_back(triangleIndex);
        }
    }

    if(triangleIndicesChild1.size() != 0)
    {
        NodeData child1NodeData(aabb_child1);
        child1NodeData.indices = triangleIndicesChild1;
        BVHNode* child1 = new BVHNode(child1NodeData);
        node->SetLeft(child1);
        child1->SetParent(node);
        m_bvhTreeNodes.push_back(child1);
        RecursivePartition(child1);
    }

    if(triangleIndicesChild2.size() != 0 && triangleIndicesChild2.size() != nodeDat.indices.size())
    {
        AABB3 aabb_child2 = CalculateBoundingBoxForTriangles(triangleIndicesChild2);
        NodeData child2NodeData(aabb_child2);
        child2NodeData.indices = triangleIndicesChild2;
        BVHNode *child2 = new BVHNode(child2NodeData);
        node->SetRight(child2);
        child2->SetParent(node);
        m_bvhTreeNodes.push_back(child2);
        RecursivePartition(child2);
    }
}

TriMeshProxQueryV3::AABB3 TriMeshProxQueryV3::CalculateBoundingBoxForTriangles(const std::vector<int>& indices)const
{
    std::vector<TriMeshProxQueryV3::AABB3> aabb;
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
    for(unsigned i=0;i<indices.size();++i)
    {
        int triangleIndex = indices[i];
        aabb.push_back(triangles[triangleIndex].CalculateAABB());
    }

    return AABB<Vec3>::CalculateAABB(aabb);
}

void TriMeshProxQueryV3::PartitionMesh()
{
    {   // Create root node
        AABB3 totalBoundingBox = AABB<Vec3>::CalculateAABB(m_aabb);
        NodeData rootNodeData(totalBoundingBox);
        for(unsigned i=0;i<m_aabb.size();++i){rootNodeData.indices.push_back(i);}
        BVHNode *rootNode = new BVHNode(rootNodeData);
        m_bvhTreeNodes.push_back(rootNode);
        RecursivePartition(rootNode);
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
	throw;
}

void TriMeshProxQueryV3::UpdateNodeDistDown(BVHNode* node, 
											double& minDist, 
											const Vec3& point, 
											double distThreshold)
{
	if (node == nullptr){return;}
	NodeData& nodeDat = node->Data();
	if (!nodeDat.distUpdated)
	{
		IRes res = nodeDat.aabb.CalcShortestDistanceFrom(point);
		nodeDat.dist = res.Dist;
		nodeDat.distUpdated = true;
		if (nodeDat.dist > distThreshold)
		{
			minDist = nodeDat.dist;
			// nothing else to do in this tree.
			return;
		}
	}
	BVHNode* leftNode = node->GetLeft();
	BVHNode* rightNode = node->GetRight();

	auto updateNode = [&point](BVHNode* nd)
	{
		NodeData& nodeData = nd->Data();
		if (nodeData.distUpdated)
		{
			// Can never happen
			throw;
		}
		IRes res = nodeData.aabb.CalcShortestDistanceFrom(point);
		nodeData.dist = res.Dist;
		nodeData.distUpdated = true;
	};

	if (leftNode != nullptr)
	{
		updateNode(leftNode);
	}

	if (rightNode != nullptr)
	{
		updateNode(rightNode);
	}

	// Case 1: Both left and right nodes are null
	if (leftNode == nullptr && rightNode == nullptr)
	{
		// Here goes the iteration over all the triangles in the cell
		// to find the least distance		
		minDist = nodeDat.dist;
	}

	// Case 2: Both left and right nodes are not null
	if (leftNode != nullptr && rightNode != nullptr)
	{
		if (leftNode->Data().dist < rightNode->Data().dist)
		{
			if (leftNode->Data().dist > distThreshold)
			{
				minDist = leftNode->Data().dist;
				return;
			}
			else
			{
				// This means the left node is closer than the right node to the point
				UpdateNodeDistDown(leftNode, minDist, point, distThreshold);
			}
		}
		else
		{
			if (rightNode->Data().dist > distThreshold)
			{
				minDist = rightNode->Data().dist;
				return;
			}
			else
			{
				UpdateNodeDistDown(rightNode, minDist, point, distThreshold);
			}			
		}
	}

	// Case 3: Left node is not null, right node is null
	if (leftNode != nullptr && rightNode == nullptr)
	{
		UpdateNodeDistDown(rightNode, minDist, point, distThreshold);
	}

	// Case 4: :Left node is null, right node is not null
	if (leftNode == nullptr && rightNode != nullptr)
	{
		UpdateNodeDistDown(rightNode, minDist, point, distThreshold);
	}
}
