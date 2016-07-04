#pragma once

namespace rabbit
{

template<typename PolygonType,
         typename MeshBuilder>
class IMeshBuilder
{
    void GeneratePolygons(std::vector<PolygonType>& polygons);
};

template<typename PolygonType,typename MeshBuilder>
void IMeshBuilder<PolygonType, MeshBuilder>::GeneratePolygons(std::vector<PolygonType>& polygons)
{
    static_cast<MeshBuilder*>(this)->GeneratePolygons(polygons);
}

}
