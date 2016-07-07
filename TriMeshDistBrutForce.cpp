#include "TriMeshDistBrutForce.h"
#include "Triangle.h"
#include "Vec3.h"
namespace rabbit
{

TriMeshBruteForce::TriMeshBruteForce(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh):
        IProximityQueries<Triangle<Vec3>, TriMeshBruteForce>(mesh){}

Vec3 TriMeshBruteForce::CalculateClosestPointImpl(const Vec3& point)
{
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
    for(const Triangle<Vec3>& t : triangles)
    {
        IntersectionResult<Vec3> res = t.CalcShortestDistanceFrom(point);
    }

}

}
