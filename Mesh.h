#pragma once

#include <vector>
#include <string>

#include "Triangle.h"

namespace rabbit
{
    class Mesh
    {
    public:
        explicit Mesh(std::string fileName);
        std::vector<Triangle>& GetTriangles();
        std::vector<Triangle> m_triangles;
        std::vector<RigidTriangle> m_tr;
    };
}
