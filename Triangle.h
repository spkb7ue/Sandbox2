#pragma once

#include "Vec3.h"

namespace rabbit
{

template<typename T, bool deformable>
struct IsPropertyModifiable{};

template<typename T>
struct IsPropertyModifiable<T,true>
{
    typedef T type;
};

template<typename T>
struct IsPropertyModifiable<T,false>
{
    typedef const T type;
};

template<typename T,
         bool isDeformable,
         typename Derived,
         typename ShapeProps>
class IShape
{
public:
    typedef typename IsPropertyModifiable<T,isDeformable>::type VertType;
    typedef typename IsPropertyModifiable<T,isDeformable>::type VecType;
    typedef typename ShapeProps::VertIndices VertIndices;
    typedef typename ShapeProps::EdgeIndices EdgeIndices;

    IShape(const T& v0, const T& v1, const T& v2):
        m_verts({v0, v1, v2}),
        m_edges({v1-v0, v2-v0, v2-v1}),
        m_normal((T::crossProduct(m_edges[0],m_edges[1])).normalise()){
    }

    VertType& Vert(VertIndices index){
        return m_verts[static_cast<short>(index)];
    }

    VecType& Edge(EdgeIndices index){
        return m_edges[static_cast<short>(index)];
    }

    VecType& Normal(){
        return m_normal;
    }

    bool IsPointWithinShapeExtrudedAlongNormal(const T& point)const{
        return static_cast<Derived*>(this)->IsPointWithinShapeExtrudedAlongNormal(point);
    }

    typename ShapeProps::PointIntersectionResult ProjectPointOntoShapePlane(const T& point) const{
        return static_cast<Derived*>(this)->IsPointWithinShapeExtrudedAlongNormal(point);
    }

    typename ShapeProps::PointIntersectionResult CalcShortestDistanceFrom(const T& point) const{
        return static_cast<Derived*>(this)->CalcShortestDistanceFrom(point);
    }

    typename ShapeProps::PointIntersectionResult CheckPointSegDist(const T& origin,
                                                                   const T& seg,
                                                                   const T& P);

    VertType m_verts[ShapeProps::NUM_VERTICES];
    VecType m_edges[ShapeProps::NUM_VERTICES];
    VecType m_normal;
};

template<typename T,
         bool isDeformable,
         typename Derived,
         typename ShapeProps>
typename ShapeProps::PointIntersectionResult
IShape<T,isDeformable,Derived,ShapeProps>::CheckPointSegDist(const T& origin,
                                                             const T& seg,
                                                             const T& P)
{
    const auto origin_P = P - origin;
    const T unitVecAlongSeg = seg.normalise();

    const auto Px = origin + unitVecAlongSeg*T::dotProduct(unitVecAlongSeg,origin_P);
    const auto origin_Px = Px - origin;
    auto alongSeg = T::dotProduct(unitVecAlongSeg, origin_Px);
    static const T EPSILON = T(1.0e-12);
    if(alongSeg >= -EPSILON && alongSeg <= seg.magnitude() + EPSILON)
    {
        return ShapeProps::PointIntersectionResult(Px, (P-Px).magnitude());
    }
    else
    {
        if(alongSeg < 0.0)
        {
            return ShapeProps::PointIntersectionResult(origin,origin_P.magnitude());
        }
        else
        {
            auto otherEnd = origin + seg;
            return ShapeProps::PointIntersectionResult(otherEnd, (otherEnd - P).magnitude());
        }
    }
}

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

template<bool isDeformable = false>
class TriangleV1:
    public IShape<  Vec3,
                    isDeformable,
                    TriangleV1<isDeformable>,
                    TriangleProps3D>
{
public:
    TriangleV1(const Vec3& p0, const Vec3& p1, const Vec3& p2):
         IShape<Vec3,
                isDeformable,
                TriangleV1<isDeformable>,
                TriangleProps3D>(p0, p1, p2){}
    bool IsPointWithinShapeExtrudedAlongNormal(const Point& point)const;

    TriangleProps3D::BarycentricCoords CalcBarycentricCoords(const Point& P)const;

    TriangleProps3D::PointIntersectionResult ProjectPointOntoShapePlane(const Point& p)const;

    TriangleProps3D::PointIntersectionResult CalcShortestDistanceFrom(const Point& point) const;
};

template<bool isDeformable>
TriangleProps3D::PointIntersectionResult TriangleV1<isDeformable>::CalcShortestDistanceFrom(const Point& p) const
{
    const auto projectedPointData = ProjectPointOntoShapePlane(p);
    const auto pDist = projectedPointData.Dist;
    const Point& projectedPoint = projectedPointData.P;
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

        const auto P0_P2_Data = CheckPointSegDist(this->m_verts[0], this->m_edges[1], projectedPoint);
        const auto d1 = P0_P2_Data.Dist;

        const auto P1_P2_Data = CheckPointSegDist(this->m_verts[1],this->m_edges[2],projectedPoint);
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
TriangleProps3D::PointIntersectionResult TriangleV1<isDeformable>::ProjectPointOntoShapePlane(const Point& p)const
{
    const auto p_P0 = this->m_verts[0] - p;
    auto signedDist = Vec3::dotProduct(p_P0, this->m_normal);
    return TriangleProps3D::PointIntersectionResult(Point(p + this->m_normal*signedDist), -signedDist);
}

template<bool isDeformable>
TriangleProps3D::BarycentricCoords TriangleV1<isDeformable>::CalcBarycentricCoords(const Point& P)const
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
bool TriangleV1<isDeformable>::IsPointWithinShapeExtrudedAlongNormal(const Vec3& point)const
{
    TriangleProps3D::BarycentricCoords coords = CalcBarycentricCoords(point);
    const double tol = 0.000001;
    return ( coords.u >= -tol && coords.v >= -tol && coords.u+coords.v <= 1.0f + tol);
}

typedef TriangleV1<false> RigidTriangle;
typedef TriangleV1<true> FlexibleTriangle;

struct Triangle
{
    Point P0;
    Point P1;
    Point P2;
    Vec3 P0_P1; ///< P0 to P1 Vector
    Vec3 P0_P2; ///< P0 to P2 Vector;
    Vec3 P1_P2; ///< P1 to P2 Vector
    Vec3 Normal; ///< Normal to the Triangle's plane

    bool IsPointWithinExtrudedTriangle(const Point& p)const;

    std::pair<Point,double> ProjectPointOntoTrianglePlane(const Point& p) const;

    std::pair<double,double> CalcBarycentricCoords(const Point& p)const;

    std::pair<Point, double> CalcShortestDistanceFrom(const Point&)const;

    // Debug functions
    Point CalcPointFromBarycentricCoords(const double u, const double v) const;
    Point CalcPointFromBarycentricCoords(std::pair<double,double> coords)const;
    std::tuple<Point, double, bool> CheckPointSegDist(const Vec3& origin,
                                                      const Vec3& seg,
                                                      const Vec3& P)const;

    Triangle(const Point& p0, const Point& p1, const Point& p2);
};
}
