#pragma once

#include <boost/noncopyable.hpp>
#include <vector>

namespace rabbit
{

/**
* @brief An abstract interface to the mesh building policy. Users are expected to provide
* their own implementation to create the mesh in different ways.
* @tparam PolygonType Type of polygon comprising the mesh
*/
template<typename PolygonType>
class IMeshBuildingPolicy : boost::noncopyable
{
public:
	/**
	* @param polygons A reference to the vector of polygons. It is expected
	* that when this function is called, the concrete class will populate
	* the vector with polygons of type PolygoneType
	*/
    virtual void GeneratePolygons(std::vector<PolygonType>& polygons) = 0;

    virtual ~IMeshBuildingPolicy(){}
};

}
