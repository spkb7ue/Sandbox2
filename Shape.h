#pragma once
#include "IsPropertyModifiable.h"
#include "Vec3.h"
#include <algorithm>
namespace rabbit
{

template<typename T>
struct PointSegInterectionResult
{
    PointSegInterectionResult(const T& intersectionPoint, double dist):
        m_point(intersectionPoint),m_dist(dist){}
    T m_point;
    double m_dist;
};

struct BarycentricCoords
{
    BarycentricCoords(double a, double b):u(a),v(b){}
    double u;
    double v;
};

template<typename T, unsigned N, typename AccessIDs>
class Shape
{
public:
    Shape(std::initializer_list<T> verts, std::initializer_list<T> edges){
        std::copy(verts.begin(), verts.end(), m_verts);
        std::copy(edges.begin(), edges.end(), m_edges);
    }

    Shape(){}

    unsigned GetNVerts(){return N;}

    T& GetEdge(AccessIDs id){
        return m_edges[static_cast<unsigned>(id)];
    }

    const T& GetEdge(AccessIDs id)const{
        return m_edges[static_cast<unsigned>(id)];
    }

    T& GetVerts(AccessIDs id){
        return m_verts[static_cast<unsigned>(id)];
    }

    const T& GetVerts(AccessIDs id)const{
        return m_verts[static_cast<unsigned>(id)];
    }

protected:
    T m_verts[N];
    T m_edges[N];

    ~Shape(){}

};

enum ids : unsigned
{
    zero = 0,
    one = 1,
    two = 2
};

template<typename T>
class TriangleV1 : public Shape<T, 3, ids>
{
public:
    TriangleV1(const T& p0, const T& p1, const T& p2):
        Shape<T, 3, ids>({p0, p1, p2}, {p1-p0, p2-p0, p2-p1}),
        m_normal((Vec3::crossProduct(p1-p0,p2-p0).normalise())){}

    TriangleV1(const T& p0, const T& p1, const T& p2,
               const T& e0, const T& e1, const T& e2,
               const T& normal):
        Shape<T, 3, ids>({p0, p1, p2}, {e0, e1, e2}),
        m_normal(normal){}

    T& Normal(){return m_normal;}
    const T& Normal()const{return m_normal;}

    const T& P0()const{return this->m_verts[0];}
    const T& P1()const{return this->m_verts[1];}
    const T& P2()const{return this->m_verts[2];}

    const T& P0P1()const{return this->m_edges[0];}
    const T& P0P2()const{return this->m_edges[1];}
    const T& P1P2()const{return this->m_edges[2];}

    BarycentricCoords CalcBarycentricCoords(const T& point)const;

    bool IsPointWithinShapeExtrudedAlongNormal(const T& point)const;

    PointSegInterectionResult<T> ProjectPointOntoShapePlane(const T& p)const;

    PointSegInterectionResult<T> CalcShortestDistanceFrom(const T& point) const;

    ~TriangleV1(){};
private:
    T m_normal;

};

template<typename T>
PointSegInterectionResult<T> TriangleV1<T>::CalcShortestDistanceFrom(const T& p)const
{
    const auto projectedPointData = ProjectPointOntoShapePlane(p);
    const auto pDist = projectedPointData.Dist;
    const T& projectedPoint = projectedPointData.P;
    if(IsPointWithinShapeExtrudedAlongNormal(projectedPoint))
    {
        // If the point was within the extruded triangle, there is nothing else
        // to do here and we have already found the right triangle.
        return projectedPointData;
    }
    else
    {
        // Lets check intersection with the other edges now.
        const auto P0_P1_Data = CheckPointSegDist(this->m_verts[0], this->m_edges[0],projectedPoint);
        const auto d0 = P0_P1_Data.Dist;

        const auto P0_P2_Data = CheckPointSegDist(this->m_verts[0], this->m_edges[1], projectedPoint);
        const auto d1 = P0_P2_Data.Dist;

        const auto P1_P2_Data = this->CheckPointSegDist(this->m_verts[1],this->m_edges[2],projectedPoint);
        const auto d2 = P1_P2_Data.Dist;
        if(d0 < d1)
        {
            if(d0 < d2)
            {
                auto dist = sqrt(d0*d0 + pDist*pDist);
                return PointSegInterectionResult<T>(P0_P1_Data.P, dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return PointSegInterectionResult<T>(P1_P2_Data.P, dist);
            }
        }
        else
        {
            if(d1 < d2)
            {
                auto dist = sqrt(d1*d1 + pDist*pDist);
                return PointSegInterectionResult<T>(P0_P2_Data.P, dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return PointSegInterectionResult<T>(P1_P2_Data.P, dist);
            }
        }
    }
}

template<typename T>
PointSegInterectionResult<T> TriangleV1<T>::ProjectPointOntoShapePlane(const T& p)const
{
    const auto p_P0 = this->m_verts[0] - p;
    auto signedDist = T::dotProduct(p_P0, this->m_normal);
    return PointSegInterectionResult<T>(T(p + this->m_normal*signedDist), -signedDist);
}

template<typename T>
bool TriangleV1<T>::IsPointWithinShapeExtrudedAlongNormal(const T& point)const
{
    BarycentricCoords coords = CalcBarycentricCoords(point);
    const double tol = 0.000001;
    return ( coords.u >= -tol && coords.v >= -tol && coords.u+coords.v <= 1.0f + tol);
}

template<typename T>
BarycentricCoords TriangleV1<T>::CalcBarycentricCoords(const T& P)const
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
    return BarycentricCoords(u,v);
}
}
