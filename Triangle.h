#pragma once

#include "Shape.h"
namespace rabbit
{

template<typename T>
struct IntersectionResult
{
    IntersectionResult(const T& intersectionPoint, double dist):
        Point(intersectionPoint),Dist(dist){}

    T Point;
    double Dist;
};

template<typename T>
struct PointSegIntersection
{
    PointSegIntersection(const T& intersectionPoint,
                         double dist,
                         bool isWithinExtrudedNormal):
        IsWithinExtrudedNormal(isWithinExtrudedNormal),
        IRes(intersectionPoint, dist){}

    bool IsWithinExtrudedNormal;
    IntersectionResult<T> IRes;
};

struct BarycentricCoords
{
    BarycentricCoords(double a, double b):u(a),v(b){}
    double u;
    double v;
};


enum ids : unsigned
{
    zero = 0,
    one = 1,
    two = 2
};

template<typename T>
class Triangle : public Shape<T, 3, ids>
{
public:
    Triangle(const T& p0, const T& p1, const T& p2):
        Shape<T, 3, ids>({p0, p1, p2}, {p1-p0, p2-p0, p2-p1}),
        m_normal((T::crossProduct(p1-p0,p2-p0).normalise())){}

    Triangle(const T& p0, const T& p1, const T& p2,
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

    IntersectionResult<T> ProjectPointOntoShapePlane(const T& p)const;

    IntersectionResult<T> CalcShortestDistanceFrom(const T& point) const;

    PointSegIntersection<T> CheckPointSegDist(const T& origin,
                                            const T& seg,
                                            const T& point)const;


    IntersectionResult<T> CalcShortestDistanceFromBruteForce(const T& point) const;

    ~Triangle(){};
private:
    T m_normal;

};

template<typename T>
IntersectionResult<T> Triangle<T>::CalcShortestDistanceFromBruteForce(const T& point) const
{

}

template<typename T>
PointSegIntersection<T> Triangle<T>::CheckPointSegDist(const T& origin,const T& seg,const T& P)const
{
    const auto origin_P = P - origin;
    const T unitVecAlongSeg = seg.normalise();

    // Project point onto the seg
    const auto Px = origin + unitVecAlongSeg*T::dotProduct(unitVecAlongSeg,origin_P);

    // Origin to projected point
    const auto origin_Px = Px - origin;
    auto alongSeg = T::dotProduct(unitVecAlongSeg, origin_Px);
    static const double EPSILON= 1.0e-12;
    if(alongSeg >= -EPSILON && alongSeg <= seg.magnitude() + EPSILON)
    {
        return PointSegIntersection<T>(Px, (P-Px).magnitude(), true);
    }
    else
    {
        if(alongSeg < 0.0)
        {
            return PointSegIntersection<T>(origin,origin_P.magnitude(),false);
        }
        else
        {
            auto otherEnd = origin + seg;
            return PointSegIntersection<T>(otherEnd, (otherEnd - P).magnitude(),false);
        }
    }
}

template<typename T>
IntersectionResult<T> Triangle<T>::CalcShortestDistanceFrom(const T& p)const
{
    const auto projectedPointData = ProjectPointOntoShapePlane(p);
    const auto pDist = projectedPointData.Dist;
    const T& projectedPoint = projectedPointData.Point;
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
        const auto d0 = P0_P1_Data.IRes.Dist;

        const auto P0_P2_Data = CheckPointSegDist(this->m_verts[0], this->m_edges[1], projectedPoint);
        const auto d1 = P0_P2_Data.IRes.Dist;

        const auto P1_P2_Data = this->CheckPointSegDist(this->m_verts[1],this->m_edges[2],projectedPoint);
        const auto d2 = P1_P2_Data.IRes.Dist;
        if(d0 < d1)
        {
            if(d0 < d2)
            {
                auto dist = sqrt(d0*d0 + pDist*pDist);
                return IntersectionResult<T>(P0_P1_Data.IRes.Point, dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return IntersectionResult<T>(P1_P2_Data.IRes.Point, dist);
            }
        }
        else
        {
            if(d1 < d2)
            {
                auto dist = sqrt(d1*d1 + pDist*pDist);
                return IntersectionResult<T>(P0_P2_Data.IRes.Point, dist);
            }
            else
            {
                auto dist = sqrt(d2*d2 + pDist*pDist);
                return IntersectionResult<T>(P1_P2_Data.IRes.Point, dist);
            }
        }
    }
}

template<typename T>
IntersectionResult<T> Triangle<T>::ProjectPointOntoShapePlane(const T& p)const
{
    const auto p_P0 = this->m_verts[0] - p;
    auto signedDist = T::dotProduct(p_P0, this->m_normal);
    return IntersectionResult<T>(T(p + this->m_normal*signedDist), std::abs(signedDist));
}

template<typename T>
bool Triangle<T>::IsPointWithinShapeExtrudedAlongNormal(const T& point)const
{
    BarycentricCoords coords = CalcBarycentricCoords(point);
    const double tol = 0.000001;
    return ( coords.u >= -tol && coords.v >= -tol && coords.u+coords.v <= 1.0f + tol);
}

template<typename T>
BarycentricCoords Triangle<T>::CalcBarycentricCoords(const T& P)const
{
    // Assume P = P0 + u*P0_P1 + v*P0_P2, goal is to calculate u and v
    // Project this equation along P0_P1 and P0_P2 and we obtain a set
    // of equations which can then be solved.
    //  |x| = |a, b||u|
    //  |y| = |c, d||v|

    const auto P0_P = P - this->m_verts[0];
    const auto x = T::dotProduct(this->m_edges[0],P0_P);
    const auto y = T::dotProduct(this->m_edges[1],P0_P);

    const auto a = T::dotProduct(this->m_edges[0],this->m_edges[0]);
    const auto b = T::dotProduct(this->m_edges[0],this->m_edges[1]);
    const auto c = T::dotProduct(this->m_edges[1],this->m_edges[0]);
    const auto d = T::dotProduct(this->m_edges[1],this->m_edges[1]);

    const auto inverseDet = 1.0/(a*d - c*b);
    const auto u = inverseDet * (d*x - b*y);
    const auto v = inverseDet * (-c*x + a*y);
    return BarycentricCoords(u,v);
}
}
