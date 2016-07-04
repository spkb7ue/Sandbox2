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
        std::vector<RigidTriangle> m_triangles;
        std::vector<RigidTriangle>& GetTriangles();
    };


}
