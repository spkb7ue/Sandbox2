#pragma once

#include <boost/noncopyable.hpp>
#include <memory>

namespace rabbit
{

template<typename PolygonType>
class IMeshBuildingPolicy : boost::noncopyable
{
public:

    virtual void GeneratePolygons(std::vector<PolygonType>& polygons) = 0;

    virtual ~IMeshBuildingPolicy(){}
};


template<typename PolygonType>
class MeshBuilder : boost::noncopyable
{
public:

    explicit MeshBuilder(std::shared_ptr<IMeshBuildingPolicy<PolygonType>> meshBuildingPolicy);

    void GeneratePolygons(std::vector<PolygonType>& polygons);
private:
    std::shared_ptr<IMeshBuildingPolicy<PolygonType>> m_meshBuildingPolicy;
};



template<typename PolygonType>
void MeshBuilder<PolygonType>::GeneratePolygons(std::vector<PolygonType>& polygons)
{
    m_meshBuildingPolicy->GeneratePolygons(polygons);
}

}
