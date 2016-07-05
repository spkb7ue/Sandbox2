#pragma once

#include "IMeshBuildingPolicy.h"
#include "Triangle.h"
#include "Vec3.h"

namespace rabbit
{
    /**
    * This builder can be used to extract triangle coordinates from a plain text file.
    * Look at the file rabbit.triangles for the expected format.
    */
    class TriangularMeshBuilingPolicy : public IMeshBuildingPolicy<Triangle<Vec3>>
    {
    public:

        /**
        * Create a triangular mesh builder. The ctor will throw if file does not exist
        * @param fileName Name of the file
        */
        TriangularMeshBuilingPolicy(std::string fileName);

        /**
        * Interface implementation for the base class
        * @param triangles The triangle vector which are to be populated
        */
        virtual void GeneratePolygons(std::vector<Triangle<Vec3>>& triangles) override;
    private:

        std::string m_fileName; ///< Name of the file containing the triangle coordinates comprising the mesh
    };
}
