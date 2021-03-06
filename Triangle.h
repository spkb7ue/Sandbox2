#pragma once

#include "Polygon.h"
#include "IntersectionResult.h"
#include "AABB.h"
#include <math.h>

namespace rabbit
{

/**
* @brief struct used to represent a point inside the triangle
* including all the three edges.
*/
struct BarycentricCoords
{
    BarycentricCoords(double a, double b):u(a),v(b){}

	// If a triangle has 3 vertices P0,P1 and P2
	// any point inside the Triangle can be written as
	// P = P0 + u*(P1-P0) + v*(P2-P0) with the condition
	// 0 <= u,v <= 1 && (u + v <= 1)
    double u;
    double v;
};

/**
* @brief Ids that can be used to access vertices/edges of the triangle.
* Attempting to access vert/edge using an int will result in compile time
* error. This ensures automatic bounds checking for the underlying arrays
*/
enum ids : unsigned
{
    zero = 0,
    one = 1,
    two = 2
};

/**
* @brief A triangle class. The following notation is used throughout the
* triangle related code. The triangle consists of 3 vertices
* represented as P0, P1 and P2. The corresponding edges are
* P0P1, P0P2, P1P2 which completes the triangle description
*/
template<typename T>
class Triangle : public Polygon<T, 3, ids>
{
public:

	using Polygon<T, 3, ids>::m_verts;
	using Polygon<T, 3, ids>::m_edges;

    Triangle(const T& p0, const T& p1, const T& p2):
        Polygon<T, 3, ids>({p0, p1, p2}, {p1-p0, p2-p0, p2-p1}),
        m_normal((T::crossProduct(p1-p0,p2-p0).normalise())){}

    Triangle(const T& p0, const T& p1, const T& p2,
               const T& e0, const T& e1, const T& e2,
               const T& normal):
        Polygon<T, 3, ids>({p0, p1, p2}, {e0, e1, e2}),
        m_normal(normal){}

    T& Normal(){return m_normal;}
    const T& Normal()const{return m_normal;}

	// Access vertices
    const T& P0()const{return m_verts[0];}
    const T& P1()const{return m_verts[1];}
    const T& P2()const{return m_verts[2];}

	// Access edges
    const T& P0P1()const{return m_edges[0];}
    const T& P0P2()const{return m_edges[1];}
    const T& P1P2()const{return m_edges[2];}

	/**
	* @param point, point whose bc coords are to be calculated.
	* @return Returns the barycentric coords of the input point
	*/
	BarycentricCoords CalcBarycentricCoords(const T& point)const;

	/**
	* @return true if the input points lies within the volume generated by
	* extruding the triangle along its normal in either directions
	*/
    bool IsPointWithinShapeExtrudedAlongNormal(const T& point)const;

	/**
	* @return The intersection of the input point projected onto the
	* plane of the triangle.
	*/
    IntersectionResult<T> ProjectPointOntoShapePlane(const T& p)const;

	/**
	* Calculate the least distance between point and a triangle
	* @param maxDist if closest point to the triangle is at a distance
	* greater than maxDist, then IntersectionResult reports infinity
	*/
    IntersectionResult<T> CalcShortestDistanceFrom(const T& point,
                                                   double maxDist) const;

	/**
	* Calculates the least distance between point and a segment. See the
	* documentation for PointSegIntersection for more details
	*/
    PointSegIntersection<T> CheckPointSegDist(const T& origin,
                                              const T& seg,
                                              const T& point)const;


    AABB<T> CalculateAABB() const;

    ~Triangle(){};
private:
    T m_normal;

};

template<typename T>
AABB<T> Triangle<T>::CalculateAABB()const
{
    double xmin = std::min(std::min(m_verts[0].X(), m_verts[1].X()),m_verts[2].X());
    double xmax = std::max(std::max(m_verts[0].X(), m_verts[1].X()),m_verts[2].X());

    double ymin = std::min(std::min(m_verts[0].Y(), m_verts[1].Y()),m_verts[2].Y());
    double ymax = std::max(std::max(m_verts[0].Y(), m_verts[1].Y()),m_verts[2].Y());

    double zmin = std::min(std::min(m_verts[0].Z(), m_verts[1].Z()),m_verts[2].Z());
    double zmax = std::max(std::max(m_verts[0].Z(), m_verts[1].Z()),m_verts[2].Z());

    T center((xmax + xmin)*0.5, (ymax + ymin)*0.5, (zmax + zmin)*0.5);
    T halfExtents((xmax - xmin)*0.5, (ymax - ymin)*0.5, (zmax - zmin)*0.5);
    return AABB<T>(center, halfExtents);
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
IntersectionResult<T> Triangle<T>::CalcShortestDistanceFrom(const T& p, double maxDist)const
{
    const auto projectedPointData = ProjectPointOntoShapePlane(p);
    const auto pDist = projectedPointData.Dist;
    if(pDist > maxDist)
    {
        // If the perpendicular distance from point to the triangular plane
        // is greater than the specified threshold, we don't do any other checks
        // The distance is set to infinity so that no other calculations are performed
        return IntersectionResult<T>(projectedPointData.Point, std::numeric_limits<double>::max());
    }

    const T& projectedPoint = projectedPointData.Point;
    if(IsPointWithinShapeExtrudedAlongNormal(projectedPoint))
    {
        // If the point was within the extruded triangle, there is nothing else
        // to do here and we have already found the closest point.
        return projectedPointData;
    }
    else
    {
        // Lets check intersection with the other edges now.
        const auto P0_P1_Data = CheckPointSegDist(m_verts[0], m_edges[0],projectedPoint);
        const auto d0 = P0_P1_Data.IRes.Dist;

        const auto P0_P2_Data = CheckPointSegDist(m_verts[0], m_edges[1], projectedPoint);
        const auto d1 = P0_P2_Data.IRes.Dist;

        const auto P1_P2_Data = CheckPointSegDist(m_verts[1],m_edges[2],projectedPoint);
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
    const auto p_P0 = m_verts[0] - p;
    auto signedDist = T::dotProduct(p_P0, m_normal);
    return IntersectionResult<T>(T(p + m_normal*signedDist), std::abs(signedDist));
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

    const auto P0_P = P - m_verts[0];
    const auto x = T::dotProduct(m_edges[0],P0_P);
    const auto y = T::dotProduct(m_edges[1],P0_P);

    const auto a = T::dotProduct(m_edges[0],m_edges[0]);
    const auto b = T::dotProduct(m_edges[0],m_edges[1]);
    const auto c = T::dotProduct(m_edges[1],m_edges[0]);
    const auto d = T::dotProduct(m_edges[1],m_edges[1]);

    const auto inverseDet = 1.0/(a*d - c*b);
    const auto u = inverseDet * (d*x - b*y);
    const auto v = inverseDet * (-c*x + a*y);
    return BarycentricCoords(u,v);
}
}
