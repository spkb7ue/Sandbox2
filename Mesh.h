#pragma once

#include <vector>
#include <string>
#include <boost/noncopyable.hpp>

#include "Triangle.h"

namespace rabbit
{
    class Mesh : boost::noncopyable
    {
    public:

        explicit Mesh(std::string fileName);


        std::vector<Triangle>& GetTriangles();
    private:
        std::vector<Triangle> m_triangles;
    };
}
