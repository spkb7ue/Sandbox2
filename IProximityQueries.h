#pragma once
#include <memory>
#include "Mesh.h"
#include <boost/noncopyable.hpp>

namespace rabbit
{

template<typename PolygonType,
         typename ProximityQueryMethod>
class IProximityQueries : boost::noncopyable
{
public:
    typedef typename Mesh<PolygonType>::VertType VertType;
    explicit IProximityQueries(std::shared_ptr<Mesh<PolygonType>> mesh):m_mesh(mesh){}

    VertType CalculateClosestPoint(const VertType& point){
        return static_cast<ProximityQueryMethod*>(this)->CalculateClosestPointImpl(point);
    }


protected:
    std::shared_ptr<Mesh<PolygonType>> m_mesh;
};

}
