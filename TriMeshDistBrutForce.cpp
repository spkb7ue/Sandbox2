#include "TriMeshDistBrutForce.h"
#include "Vec3.h"
#include "Mesh.h"
namespace rabbit
{

TriMeshBruteForce::TriMeshBruteForce(std::shared_ptr<Mesh<Triangle<Vec3>>> mesh):
        IProximityQueries<Triangle<Vec3>, TriMeshBruteForce>(mesh){}

std::tuple<Vec3,double,bool> TriMeshBruteForce::CalculateClosestPointImpl(const Vec3& point,double distThreshold)
{
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
    Vec3 closestPoint;
    double minDist = distThreshold;
    bool foundPoint = false;
    // Check min distance to each triangle
    for(const Triangle<Vec3>& t : triangles)
    {
        // Note the distance threshold gets updated if a single point is found.
        IntersectionResult<Vec3> res = t.CalcShortestDistanceFrom(point, minDist);
        if(res.Dist < minDist)
        {
            minDist = res.Dist;
            closestPoint = res.Point;
            foundPoint = true;
        }
    }

    return std::make_tuple(closestPoint, minDist, foundPoint);
}

}
