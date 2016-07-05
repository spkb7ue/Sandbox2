#pragma once

#include <memory>
#include <type_traits>

namespace rabbit
{
    template<typename Polygon, typename MeshBuilder>
    class Mesh2
    {
    public:
        explicit Mesh2(MeshBuilder& builder);
        std::vector<Polygon> m_polygons;
    };

    template<typename Polygon, typename MeshBuilder>
    Mesh2<Polygon, MeshBuilder>::Mesh2(MeshBuilder& builder)
    {
        builder.GeneratePolygons(m_polygons);
    }
}
