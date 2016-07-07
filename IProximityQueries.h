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
    explicit IProximityQueries(std::shared_ptr<Mesh<PolygonType>> mesh):m_mesh(mesh){}

    template <typename VertType>
    VertType CalculateClosestPoint(const VertType& point)
    {
        return static_cast<ProximityQueryMethod*>(this)->CalculateClosestPointImpl(point);
    }

protected:
    std::shared_ptr<Mesh<PolygonType>> m_mesh;
};

}
