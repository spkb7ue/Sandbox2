#pragma once
#include <memory>
#include <boost/noncopyable.hpp>

namespace rabbit
{

template<typename PolygonType>
class Mesh;

template<typename PolygonType,
         typename ProximityQueryMethod>
class IProximityQueries : boost::noncopyable
{
public:
    explicit IProximityQueries(std::shared_ptr<Mesh<PolygonType>> mesh):m_mesh(mesh){}

    template <typename VertType>
    std::tuple<VertType,double, bool> CalculateClosestPoint(const VertType& point, double distThreshold)
    {
        return static_cast<ProximityQueryMethod*>(this)->CalculateClosestPointImpl(point,distThreshold);
    }

protected:
    std::shared_ptr<Mesh<PolygonType>> m_mesh;
};

}
