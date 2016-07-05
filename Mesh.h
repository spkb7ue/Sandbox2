#pragma once

#include <memory>
#include <vector>

namespace rabbit
{

    template<typename Polygon>
    class IMeshBuildingPolicy;

    template<typename PolygonType>
    class Mesh
    {
    public:
        Mesh(std::shared_ptr<IMeshBuildingPolicy<PolygonType>> buildingPolicy);

        std::vector<PolygonType>& GetPolygons(){return m_polygons;}

        std::vector<PolygonType> m_polygons;
    };

    template<typename PolygonType>
    Mesh<PolygonType>::Mesh(std::shared_ptr<IMeshBuildingPolicy<PolygonType>> buildingPolicy)
    {
        buildingPolicy->GeneratePolygons(m_polygons);
    }
}
