#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <vector>
#include "IMeshBuildingPolicy.h"

namespace rabbit
{

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
MeshBuilder<PolygonType>::MeshBuilder(std::shared_ptr<IMeshBuildingPolicy<PolygonType>> policy):
    m_meshBuildingPolicy(policy){}


template<typename PolygonType>
void MeshBuilder<PolygonType>::GeneratePolygons(std::vector<PolygonType>& polygons)
{
    m_meshBuildingPolicy->GeneratePolygons(polygons);
}

}
