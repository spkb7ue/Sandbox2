#pragma once

#include "AABB.h"
#include "Vec3.h"
#include <vector>

namespace rabbit
{

struct NodeData
{
    explicit NodeData(const AABB<Vec3>& aabb3):
		aabb(aabb3),
		dist(std::numeric_limits<double>::max()),
		distUpdated(false){}

    AABB<Vec3> aabb;
    std::vector<int> indices;
	double dist;
	bool distUpdated;
};

}
