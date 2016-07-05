#pragma once

#include "IMeshBuilder.h"
#include "Triangle.h"

namespace rabbit
{
    class TriangularMeshBuilingPolicy : IMeshBuildingPolicy<RigidTriangle>
    {
    public:
        TriangularMeshBuilingPolicy(std::string fileName);
        virtual void GeneratePolygons(std::vector<RigidTriangle>& triangles);
    private:
        std::string m_fileName;

    };
}
