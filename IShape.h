#pragma once
#include "IsPropertyModifiable.h"
namespace rabbit
{

/**
* IShape is an interface for a closed planar shape with number of edges/verts and other properties decided
* by the implementing class
* @tparam T This is the type of vertex/edge comprising the shape
* @tparam isDeformable Determines if T can ever be modified after construction. If true, vertex/edge type is const T
* @tparam Derived User defined concrete Shape.
* @tparam ShapeProps User defined ShapeProperties
*/
template<typename T,
         bool isDeformable,
         typename Derived,
         typename ShapeProps>
class IShape
{
public:
    typedef typename IsPropertyModifiable<T,isDeformable>::type VertType; ///< Vertex type
    typedef typename IsPropertyModifiable<T,isDeformable>::type VecType;  ///< EdgeType type
    typedef typename ShapeProps::VertIndices VertIndices;
    typedef typename ShapeProps::EdgeIndices EdgeIndices;

    /**
    * Create a shape with 3 verts of type T. If a shape has a different
    * number of vertices, a new constructor will have to be added.
    * Note: Ideally, we should use variadic templates here. Also not supported
    * by all the compilers yet (Visual Studio certainly does not support it yet)
    * Vertices are numbered from 0.
    * @param v0 Vertex 0
    * @param v1 Vertex 1
    * @param v2 Vertex 2
    */
    IShape(const T& v0, const T& v1, const T& v2);

    VertType& Vert(VertIndices index){
        return m_verts[static_cast<short>(index)];
    }

    VertType& Vert(VertIndices index)const{
        return m_verts[static_cast<short>(index)];
    }

    VertType* VertPtr()const{
        return m_verts;
    }

    VecType& Edge(EdgeIndices index){
        return m_edges[static_cast<short>(index)];
    }

    VecType& Edge(EdgeIndices index)const{
        return m_edges[static_cast<short>(index)];
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
                                                                   const T& P) const;
protected:
    VertType m_verts[ShapeProps::NUM_VERTICES];
    VecType m_edges[ShapeProps::NUM_VERTICES];
};

template<typename T,
         bool isDeformable,
         typename Derived,
         typename ShapeProps>
IShape<T,isDeformable,Derived,ShapeProps>::IShape(const T& v0, const T& v1, const T& v2):
        m_verts({v0, v1, v2}),
        m_edges({v1-v0, v2-v0, v2-v1}){
    }

template<typename T,
         bool isDeformable,
         typename Derived,
         typename ShapeProps>
typename ShapeProps::PointIntersectionResult
IShape<T,isDeformable,Derived,ShapeProps>::CheckPointSegDist(const T& origin,
                                                             const T& seg,
                                                             const T& P) const
{
    const auto origin_P = P - origin;
    const T unitVecAlongSeg = seg.normalise();

    const auto Px = origin + unitVecAlongSeg*T::dotProduct(unitVecAlongSeg,origin_P);
    const auto origin_Px = Px - origin;
    auto alongSeg = T::dotProduct(unitVecAlongSeg, origin_Px);
    static const double EPSILON= 1.0e-12;
    if(alongSeg >= -EPSILON && alongSeg <= seg.magnitude() + EPSILON)
    {
        return typename ShapeProps::PointIntersectionResult(Px, (P-Px).magnitude());
    }
    else
    {
        if(alongSeg < 0.0)
        {
            return typename ShapeProps::PointIntersectionResult(origin,origin_P.magnitude());
        }
        else
        {
            auto otherEnd = origin + seg;
            return typename ShapeProps::PointIntersectionResult(otherEnd, (otherEnd - P).magnitude());
        }
    }
}
}
