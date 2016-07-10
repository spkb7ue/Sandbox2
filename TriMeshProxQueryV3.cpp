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
}

void TriMeshProxQueryV3::PrintNodes(const Vec3& point, BVHNode* node)
{
	if (node == nullptr)
	{
		return;
	}
	if (node->GetLeft() == nullptr && node->GetRight() == nullptr)
	{
		auto it = std::find(node->Data().indices.begin(), node->Data().indices.end(), 2063);
		if (it != node->Data().indices.end())
		{
			BVHNode *current = node;
			while (current != nullptr)
			{
				// found triangle of interest, recurse back to the root
				IRes res = current->Data().aabb.CalcShortestDistanceFrom(point, 10000.0);
				cout << "Node ID: " << current->m_nodeID << "\t AABB: " << res.Dist << endl;
				current = current->GetParent();
			}			
			IRes res = m_mesh->GetPolygons()[2063].CalcShortestDistanceFrom(point, 10000.0);
			cout << "V3Dist: " << res.Dist;
			cout << "\n----------" << "Done printing nodes\n";
		}
	}
	else
	{
		PrintNodes(point, node->GetLeft());
		PrintNodes(point, node->GetRight());
	}	
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

	if (triangleIndicesChild1.size() != 0 && triangleIndicesChild1.size() != nodeDat.indices.size())
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
	PrintNodes(point, m_bvhTreeNodes[0]);
	cin.get();
	PrintPathToRoot(m_bvhTreeNodes[491], m_bvhTreeNodes[0]);
	//Verify(point, m_bvhTreeNodes[0]);
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
	
	double minDist;
	UpdateNodeDistDown(m_bvhTreeNodes[0], minDist, point, std::numeric_limits<double>::max(), m_bvhTreeNodes[0]);
	Flush(m_bvhTreeNodes[0]);
	return std::make_tuple(point, minDist, true);
}

void TriMeshProxQueryV3::UpdateNodeDistDown(BVHNode* node, 
											double& minDist, 
											const Vec3& point, 
											double distThreshold,
											BVHNode* terminalNode)
{
	if (node == nullptr){return;}	
	NodeData& nodeDat = node->Data();
	if (!nodeDat.distUpdated)
	{
		IRes res = nodeDat.aabb.CalcShortestDistanceFrom(point);
		nodeDat.dist = res.Dist;
		nodeDat.distUpdated = true;		
	}
	if (nodeDat.dist > distThreshold)
	{
		minDist = nodeDat.dist;
		// nothing else to do in this tree.
		return;
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
		PrintPathToRoot(node, terminalNode);
		const std::vector<Tri3>& triangles = m_mesh->GetPolygons();
		double tmp = std::numeric_limits<double>::max();
		//cout << "\nTriangleIndices: ";
		for (unsigned i = 0; i < nodeDat.indices.size(); ++i)
		{
			//cout << nodeDat.indices[i] << ", ";
			IRes res = triangles[nodeDat.indices[i]].CalcShortestDistanceFrom(point, std::numeric_limits<double>::max());
			tmp = std::min(tmp, res.Dist);
		}
		//cout << endl;
		//cin.get();
		// Here goes the iteration over all the triangles in the cell
		// to find the least distance		
		minDist = tmp;
		nodeDat.dist = tmp;
		/*if (minDist>distThreshold)
		{
			return;
		}*/

		// Now that we have calculated min dist, its time to recurse back to the root node and see if
		// we have violated any constraints
		BVHNode* parent = node->GetParent();
		BVHNode* current = node;
		if (terminalNode->GetParent() != nullptr)
		{
			cout << "Terminal id: " << terminalNode->GetParent()->m_nodeID << endl;
		}
		else
		{
			cout << "Terminal id: nullptr" << endl;
		}
		while (parent != terminalNode->GetParent())
		{	
			BVHNode * sisterNode = parent->GetLeft() == current ? parent->GetRight() : parent->GetLeft();
			if (parent == m_bvhTreeNodes[0])
			{
				cin.get();
			}
			
			if (sisterNode->Data().dist < minDist)
			{
				double updatedSisterNodeDist;
				UpdateNodeDistDown(sisterNode, updatedSisterNodeDist, point, minDist, current);
				sisterNode->Data().dist = updatedSisterNodeDist;
				minDist = std::min(updatedSisterNodeDist, minDist);
			}
			
			parent->Data().dist = minDist;

			current = parent;
			parent = current->GetParent();
		}
	}

	// Case 2: Both left and right nodes are not null
	if (leftNode != nullptr && rightNode != nullptr)
	{
		if (leftNode->Data().dist < rightNode->Data().dist)
		{
			if (leftNode->Data().dist > distThreshold)
			{
				return;
			}
			else
			{
				// This means the left node is closer than the right node to the point
				UpdateNodeDistDown(leftNode, minDist, point, distThreshold, terminalNode);
				return;
			}
		}
		else
		{
			if (rightNode->Data().dist > distThreshold)
			{
				return;
			}
			else
			{
				UpdateNodeDistDown(rightNode, minDist, point, distThreshold, terminalNode);
				return;
			}			
		}
	}

	// Case 3: Left node is not null, right node is null
	if (leftNode != nullptr && rightNode == nullptr)
	{
		if (leftNode->Data().dist > distThreshold)
		{
			return;
		}
		else
		{
			// This means the left node is closer than the right node to the point
			UpdateNodeDistDown(leftNode, minDist, point, distThreshold, terminalNode);
		}
	}

	// Case 4: :Left node is null, right node is not null
	if (leftNode == nullptr && rightNode != nullptr)
	{
		if (rightNode->Data().dist > distThreshold)
		{
			minDist = rightNode->Data().dist;
			return;
		}
		else
		{
			UpdateNodeDistDown(rightNode, minDist, point, distThreshold, terminalNode);
		}
	}
}

void TriMeshProxQueryV3::Flush(BVHNode *node)
{
	if (node == nullptr)
	{
		return;
	}

	node->Data().distUpdated = false;
	node->Data().dist = std::numeric_limits<double>::max();
	Flush(node->GetLeft());
	Flush(node->GetRight());
}

void TriMeshProxQueryV3::Verify(const Vec3& point, BVHNode* node)
{
	double distNode, distLeft, distRight;
	if (node != nullptr)
	{
		IRes res = node->Data().aabb.CalcShortestDistanceFrom(point, std::numeric_limits<double>::max());
		distNode = res.Dist;
	}
	else
	{
		return;
	}

	if (node->GetLeft() != nullptr)
	{
		IRes res = node->GetLeft()->Data().aabb.CalcShortestDistanceFrom(point, std::numeric_limits<double>::max());
		distLeft = res.Dist;
		if (distNode > distLeft)
		{
			throw;
		}
	}

	if (node->GetRight() != nullptr)
	{
		IRes res = node->GetRight()->Data().aabb.CalcShortestDistanceFrom(point, std::numeric_limits<double>::max());
		distRight = res.Dist;
		if (distNode > distRight)
		{
			throw;
		}
	}

	Verify(point, node->GetLeft());
	Verify(point, node->GetRight());
}

void TriMeshProxQueryV3::PrintPathToRoot(BVHNode* node, BVHNode* terminal)
{
	auto current = node;
	while (current != terminal->GetParent())
	{
		cout << current->m_nodeID << "-";
		current = current->GetParent();
	}
	cout << endl;
}