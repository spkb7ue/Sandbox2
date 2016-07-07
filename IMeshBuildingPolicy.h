#pragma once

//#include <boost/noncopyable.hpp>
#include <vector>

namespace rabbit
{

template<typename PolygonType>
class IMeshBuildingPolicy// : boost::noncopyable
{
public:

    virtual void GeneratePolygons(std::vector<PolygonType>& polygons) = 0;

    virtual ~IMeshBuildingPolicy(){}
};

}
