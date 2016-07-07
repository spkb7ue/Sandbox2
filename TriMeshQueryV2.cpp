#incldue "TriMeshQueryV2.h"
#include <algorithm>

using namespace rabbit;
namespace
{
    typedef Triangle<Vec3> Tri;
    typedef IntersectionResult<Vec3> IntRes;
}

TriMeshProxQueryV2::TriMeshProxQueryV2(std::shared_ptr<Mesh<Tri>> mesh):
        IProximityQueries<Tri, TriMeshProxQueryV1>(mesh)
{
    Preprocess();
}


void TriMeshProxQueryV2::Preprocess()
{
    const std::vector<Tri>& triangles = m_mesh->GetPolygons();
    m_aabb.resize(triangles.size());
    unsigned index = 0;
    std::for_each(triangles.begin(), triangles.end(), [&index](const Tri& t)
    {
        m_aabb[index++] = t.CalculateAABB();
    });
}

std::tuple<Vec3,double,bool> CalculateClosestPointImpl(const Vec3& point,double distThreshold)
{
    const std::vector<Triangle<Vec3>>& triangles = m_mesh->GetPolygons();
    Vec3 closestPoint;

    double minDist = distThreshold;
    bool foundPoint = false;

    for(unsigned i = 0; i < m_aabb.size(); ++i)
    {
        IntRes resAABB = m_aabb.CalcShortestDistanceFrom(point, minDist);
        if(resAABB.Dist < minDist)
        {
            // We only bother looking at the triangle if the distance to
            // AABB is less than the threshold

            minDist = resAABB.Dist;
            IntersectionResult<Vec3> resTri = triangles[i].CalcShortestDistanceFrom(point, minDist);
            if(resTri.Dist < minDist)
            {
                minDist = res.Dist;
                closestPoint = res.Point;
                foundPoint = true;
            }
        }
    }
}
