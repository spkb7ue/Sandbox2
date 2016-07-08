#pragma once
#include <memory>

namespace rabbit
{

template<typename PolygonType>
class Mesh;

/**
* @brief A simple interface for proximity queries methods. All proximity
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

	/**
	* Calculates the least distance between point and a polygon within the specified threshold
	* @param  point Point of interest
	* @param distThreshold Distances between the polygon and the point larger than this value are ingored.
	* @return tuple::bool is a flag which is set to true if a point was found within the threshold, false otherwise
	*		  tuple::VertType point on polygon closet to the input point. Legal only if bool is set to true
			  tuple::double minimum distance between point and the polygon
	*/
    template <typename VertType>
	std::tuple<VertType, double, bool> CalculateClosestPoint(const VertType& point, double distThreshold);

protected:
	// The destructor is protected to disallow a user holding a handle onto a pointer to this object
    ~IProximityQueries(){}

    std::shared_ptr<Mesh<PolygonType>> m_mesh; ///< Polygonal mesh of interest
};

template<typename PolygonType, typename ProximityQueryMethod>
template <typename VertType> std::tuple<VertType, double, bool>
IProximityQueries<PolygonType, ProximityQueryMethod>::CalculateClosestPoint(const VertType& point, double distThreshold)
{
	return static_cast<ProximityQueryMethod*>(this)->CalculateClosestPointImpl(point, distThreshold);
}

}
