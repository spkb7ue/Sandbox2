#pragma once
#include <memory>

namespace rabbit
{

template<typename PolygonType>
class Mesh;

/**
* A simple interface for proximity queries methods. All proximity
* query methods are expected to derive from this base class and provide
* the implementation for closest point query.
* @tparam PolygonType Type of polygon comprising the mesh.
* @tparam ProximityQueryMethod Several different algorithms are possible for a given polygon type
*/
template<typename PolygonType,
         typename ProximityQueryMethod> 
class IProximityQueries
{
public:

    explicit IProximityQueries(std::shared_ptr<Mesh<PolygonType>> mesh):m_mesh(mesh){}

    template <typename VertType>
	std::tuple<VertType, double, bool> CalculateClosestPoint(const VertType& point, double distThreshold);

protected:
    ~IProximityQueries(){}
    std::shared_ptr<Mesh<PolygonType>> m_mesh;
};

template<typename PolygonType, typename ProximityQueryMethod>
template <typename VertType> std::tuple<VertType, double, bool> 
IProximityQueries<PolygonType, ProximityQueryMethod>::CalculateClosestPoint(const VertType& point, double distThreshold)
{
	return static_cast<ProximityQueryMethod*>(this)->CalculateClosestPointImpl(point, distThreshold);
}

}
