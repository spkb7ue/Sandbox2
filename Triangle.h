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

template<typename T, bool isTriangleDeformable = false>
class Tri
{
public:
    typedef typename IsPropertyModifiable<T,isTriangleDeformable>::type VertType;
    typedef typename IsPropertyModifiable<T,isTriangleDeformable>::type VecType;

    enum Edge
    {
        eP0P1 = 0,  ///< Vector from P0 to P1
        eP0P2 = 1,  ///< Vector from P0 to P2
        eP1P2 = 2   ///< Vector from P1 to P2
    };

    enum Vert
    {
        eP0 = 0,    ///< Refers to Vertex P0
        eP1 = 1,    ///< Refers to Vertex P1
        eP2 = 2     ///< Refers to Vertex P2
    };

    Tri(const T& v0, const T& v1, const T& v2):
        m_verts({v0, v1, v2}),
        m_edges({m_verts[1]-m_verts[0],
                 m_verts[2]-m_verts[0],
                 m_verts[2]-m_verts[1]}),
        m_normal(Vec3::crossProduct(m_edges[eP0P1],m_edges[eP0P2]).normalise()){}

    VertType& Vert(Vert vertID){
        return m_verts[static_cast<short>(vertID)];
    }

    VecType& Edge(Edge edgeID){
        return m_edges[static_cast<short>(edgeID)];
    }

    static const short NUM_VERTS = 3;
    VertType m_verts[NUM_VERTS];
    VecType m_edges[NUM_VERTS];
    VecType m_normal;

};

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

    std::pair<Point, double> CalcInPlaneDistanceToTriangle(const Point& p)const;

    std::pair<Point, double> CalcShortestDistanceFrom(const Point&)const;

    Point CalcPointFromBarycentricCoords(const double u, const double v) const;
    Point CalcPointFromBarycentricCoords(std::pair<double,double> coords)const;

    std::tuple<Point, double, bool> CheckPointSegDist(const Vec3& origin,
                                                      const Vec3& seg,
                                                      const Vec3& P)const;

    Triangle(const Point& p0, const Point& p1, const Point& p2);
};
}
