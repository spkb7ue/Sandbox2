#pragma once

#include "Vec3.h"

namespace rabbit
{

template<typename T>
class Tri
{
public:
    Tri(const T& v0, const T& v1, const T& v2):
        m_verts({v0, v1, v2}){}

    T& Vert(unsigned index){return m_verts[index];}
    const T& Vert(unsigned index)const {return m_verts[index];}

    static const short NUM_VERTS = 3;
    T m_verts[NUM_VERTS];
};

typedef Tri<Vec3> Triangle3D;

template<typename T>
class TriangleProps{};

template<>
class TriangleProps<Triangle3D>
{
public:
    TriangleProps():m_data(Vec3(), Vec3(),Vec3()){}
    Triangle3D m_data;
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
