#pragma once

#include "Vec3.h"
#include "IShape.h"

namespace rabbit
{

struct TriangleProps3D
{
    enum VertIndices : unsigned
    {
        eP0 = 0,    ///< Refers to Vertex P0
        eP1 = 1,    ///< Refers to Vertex P1
        eP2 = 2     ///< Refers to Vertex P2
    };

    enum EdgeIndices : unsigned
    {
        eP0P1 = 0,    ///< Refers to Edge P0 to P1
        eP0P2 = 1,    ///< Refers to Edge P0 to P2
        eP1P2 = 2     ///< Refers to Edge P1 to P2
    };

    struct BarycentricCoords
    {
        BarycentricCoords(double a, double b):u(a),v(b){}
        double u;
        double v;
    };

    struct PointIntersectionResult
    {
        PointIntersectionResult(const Point& p, double d):P(p),Dist(d){}
        Point P; ///< Point of intersection or the closest point
        double Dist; ///< The distance to point P
    };

    static const unsigned NUM_VERTICES = 3;
};

template<bool isDeformable>
class Triangle:
    public IShape<  Vec3,
                    isDeformable,
                    Triangle<isDeformable>,
                    TriangleProps3D>
{
public:
    Triangle(const Vec3& p0, const Vec3& p1, const Vec3& p2);

    bool IsPointWithinShapeExtrudedAlongNormal(const Point& point)const;

    TriangleProps3D::BarycentricCoords CalcBarycentricCoords(const Point& P)const;

    TriangleProps3D::PointIntersectionResult ProjectPointOntoShapePlane(const Point& p)const;

    TriangleProps3D::PointIntersectionResult CalcShortestDistanceFrom(const Point& point) const;

    const Point& P0()const{
        return this->m_verts[0];
    };

    const Point& P1()const{
        return this->m_verts[1];
    };

    const Point& P2()const{
        return this->m_verts[2];
    }

    const Vec3& P0P1()const{
        return this->m_edges[0];
    }

    const Vec3& P0P2()const{
        return this->m_edges[1];
    }

    const Vec3& P1P2()const{
        return this->m_edges[2];
    }
};

template<bool isDeformable>
Triangle<isDeformable>::Triangle(const Vec3& p0, const Vec3& p1, const Vec3& p2):
         IShape<Vec3,
                isDeformable,
                Triangle<isDeformable>,
                TriangleProps3D>(p0, p1, p2){}

template<bool isDeformable>
TriangleProps3D::PointIntersectionResult Triangle<isDeformable>::CalcShortestDistanceFrom(const Point& p) const
{
    const auto projectedPointData = ProjectPointOntoShapePlane(p);
    const auto pDist = projectedPointData.Dist;
    const Vec3& projectedPoint = projectedPointData.P;
    if(IsPointWithinShapeExtrudedAlongNormal(projectedPoint))
    {
        // If the point was within the extruded triangle, there is nothing else
        // to do here and we have already found the right triangle.
        return projectedPointData;
    }
    else
    {
        // Lets check intersection with the other edges now.
        const auto P0_P1_Data = this->CheckPointSegDist(this->m_verts[0], this->m_edges[0],projectedPoint);
        const auto d0 = P0_P1_Data.Dist;

        const auto P0_P2_Data = this->CheckPointSegDist(this->m_verts[0], this->m_edges[1], projectedPoint);
        const auto d1 = P0_P2_Data.Dist;

        const auto P1_P2_Data = this->CheckPointSegDist(this->m_verts[1],this->m_edges[2],projectedPoint);
        const auto d2 = P1_P2_Data.Dist;
        if(d0 < d1)
        {
            if(d0 < d2)
            {
                auto dist = sqrt(d0*d0 + pDist*pDist);
                return TriangleProps3D::PointIntersectionResult(P0_P1_Data.P, dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return TriangleProps3D::PointIntersectionResult(P1_P2_Data.P, dist);
            }
        }
        else
        {
            if(d1 < d2)
            {
                auto dist = sqrt(d1*d1 + pDist*pDist);
                return TriangleProps3D::PointIntersectionResult(P0_P2_Data.P, dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return TriangleProps3D::PointIntersectionResult(P1_P2_Data.P, dist);
            }
        }
    }
}

template<bool isDeformable>
TriangleProps3D::PointIntersectionResult Triangle<isDeformable>::ProjectPointOntoShapePlane(const Point& p)const
{
    const auto p_P0 = this->m_verts[0] - p;
    auto signedDist = Vec3::dotProduct(p_P0, this->m_normal);
    return TriangleProps3D::PointIntersectionResult(Point(p + this->m_normal*signedDist), -signedDist);
}

template<bool isDeformable>
TriangleProps3D::BarycentricCoords Triangle<isDeformable>::CalcBarycentricCoords(const Point& P)const
{
    // Assume P = P0 + u*P0_P1 + v*P0_P2, goal is to calculate u and v
    // Project this equation along P0_P1 and P0_P2 and we obtain a set
    // of equations which can then be solved.
    //  |x| = |a, b||u|
    //  |y| = |c, d||v|

    const auto P0_P = P - this->m_verts[0];
    const auto x = Vec3::dotProduct(this->m_edges[0],P0_P);
    const auto y = Vec3::dotProduct(this->m_edges[1],P0_P);

    const auto a = Vec3::dotProduct(this->m_edges[0],this->m_edges[0]);
    const auto b = Vec3::dotProduct(this->m_edges[0],this->m_edges[1]);
    const auto c = Vec3::dotProduct(this->m_edges[1],this->m_edges[0]);
    const auto d = Vec3::dotProduct(this->m_edges[1],this->m_edges[1]);

    const auto inverseDet = 1.0/(a*d - c*b);
    const auto u = inverseDet * (d*x - b*y);
    const auto v = inverseDet * (-c*x + a*y);
    return TriangleProps3D::BarycentricCoords(u,v);
}

template<bool isDeformable>
bool Triangle<isDeformable>::IsPointWithinShapeExtrudedAlongNormal(const Vec3& point)const
{
    TriangleProps3D::BarycentricCoords coords = CalcBarycentricCoords(point);
    const double tol = 0.000001;
    return ( coords.u >= -tol && coords.v >= -tol && coords.u+coords.v <= 1.0f + tol);
}

typedef Triangle<false> RigidTriangle;
typedef Triangle<true> FlexibleTriangle;
}
