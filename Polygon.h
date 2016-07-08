#pragma once
#include <algorithm>
namespace rabbit
{

/**
* @brief This class represents a closed polygon and assumes that the number
* of vertices and edges are the same.
* @tparam T class representing the vertices/edges of the polygon.
* @tparam N Number of vertices/edges of this polygon
* @tparam AccessIDs Any user defined type that can be static_cast into an unsigned int.
* Helps in compile time bounds checking for accessing edges and vertices.
*/
template<typename T, unsigned N, typename AccessIDs>
class Polygon
{
public:

	/**
	* @param verts vertices of this polygon
	* @param edges of this polygon
	* @remark There are no bound checks in the constructor
	*/
	Polygon(std::initializer_list<T> verts, std::initializer_list<T> edges);

	Polygon(){}

	// Access for the edges and verts is only possible through access ids.
	T& GetEdge(AccessIDs id){return m_edges[static_cast<unsigned>(id)];}
    const T& GetEdge(AccessIDs id)const{return m_edges[static_cast<unsigned>(id)];}
    T& GetVerts(AccessIDs id){return m_verts[static_cast<unsigned>(id)];}
    const T& GetVerts(AccessIDs id)const{return m_verts[static_cast<unsigned>(id)];}

protected:
    T m_verts[N]; ///< Vertices of the polygon
    T m_edges[N]; ///< Edges of the polygon
};

template<typename T, unsigned N, typename AccessIDs>
Polygon<T,N,AccessIDs>::Polygon(std::initializer_list<T> verts, std::initializer_list<T> edges)
{
	std::copy(verts.begin(), verts.end(), m_verts);
	std::copy(edges.begin(), edges.end(), m_edges);
}

}
