#pragma once
#include <algorithm>
namespace rabbit
{

template<typename T, unsigned N, typename AccessIDs>
class Shape
{
public:
    Shape(std::initializer_list<T> verts,
          std::initializer_list<T> edges)
    {
        std::copy(verts.begin(), verts.end(), m_verts);
        std::copy(edges.begin(), edges.end(), m_edges);
    }

    Shape(){}

    unsigned GetNVerts(){return N;}

    T& GetEdge(AccessIDs id){
        return m_edges[static_cast<unsigned>(id)];
    }

    const T& GetEdge(AccessIDs id)const{
        return m_edges[static_cast<unsigned>(id)];
    }

    T& GetVerts(AccessIDs id){
        return m_verts[static_cast<unsigned>(id)];
    }

    const T& GetVerts(AccessIDs id)const{
        return m_verts[static_cast<unsigned>(id)];
    }

protected:
    T m_verts[N];
    T m_edges[N];
};

}
