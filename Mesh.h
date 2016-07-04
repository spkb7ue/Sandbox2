#pragma once

#include <vector>
#include <string>

#include "TriangleV2.h"
#include "Triangle.h"

namespace rabbit
{
    class Mesh
    {
    public:
        explicit Mesh(std::string fileName);
        std::vector<TriangleV2>& GetTriangles();
        std::vector<TriangleV2> m_triangles;
        std::vector<RigidTriangle> m_tr;
    };
}
