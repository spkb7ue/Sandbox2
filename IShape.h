#pragma once

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

    VecType& Normal(){
        return m_normal;
    }

    VecType& Normal()const{
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
                                                                   const T& P) const;

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
