#pragma once

#include "AABB.h"
#include "Vec3.h"
#include <vector>

namespace rabbit
{

struct NodeData
{
    NodeData(const AABB<Vec3>& aabb3):
        aabb(aabb3){}
    AABB<Vec3> aabb;
    std::vector<int> triIndices;
};

}
