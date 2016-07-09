#pragma once

#include <memory>
#include <vector>

namespace rabbit
{

template<typename Polygon> class IMeshBuildingPolicy;

/**
* @brief A uniform mesh consisting of a vector of polgons. Assumption is made that
* the mesh has all polygons of the same type. Use of templates imply we can easily
* change the underlying shape comprising the mesh.
* @tparam PolygonType The type polygon making up the mesh
*/

template<typename PolygonType>
class Mesh
{
public:
	/**
	* @param buildingPolicy The mesh only gets an interface to the MeshBuilder.
	* Users can easily create their own policy to create an object of this type
	*/
    Mesh(std::shared_ptr<IMeshBuildingPolicy<PolygonType>> buildingPolicy);

	// Access the vector of underlying polygons
	std::vector<PolygonType>& GetPolygons(){return m_polygons;}
    const std::vector<PolygonType>& GetPolygons()const{return m_polygons;}

private:
    std::vector<PolygonType> m_polygons;
};

template<typename PolygonType>
Mesh<PolygonType>::Mesh(std::shared_ptr<IMeshBuildingPolicy<PolygonType>> buildingPolicy)
{
    buildingPolicy->GeneratePolygons(m_polygons);
}

}
