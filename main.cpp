#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "Mesh.h"
#include "Triangle.h"
#include <type_traits>
using namespace std;
using namespace rabbit;
namespace
{
const std::string fileName = "rabbit.triangles";

template<typename T, bool deformable>
struct IsPropertyModifiable{};

template<typename T>
struct IsPropertyModifiable<T,true>
{
    typedef T type;
};

template<typename T>
struct IsPropertyModifiable<T,false>
{
    typedef const T type;
};

template<typename T,
         bool isDeformable,
         typename Derived,
         typename Indices>
class IShape
{
public:
    typedef typename IsPropertyModifiable<T,isDeformable>::type VertType;
    typedef typename IsPropertyModifiable<T,isDeformable>::type VecType;
    typedef typename Indices::VertIndices VertIndices;

    IShape(std::initializer_list<T> verts){
        std::copy(verts.begin(), verts.end(), const_cast<T*>(m_verts));
    }


    VertType& Vert(VertIndices index){
        return m_verts[index];
    }

protected:
    VertType m_verts[Indices::NUM_VERTICES];
};

struct TriangleIndices
{
    enum VertIndices : unsigned
    {
        eP0 = 0,    ///< Refers to Vertex P0
        eP1 = 1,    ///< Refers to Vertex P1
        eP2 = 2     ///< Refers to Vertex P2
    };

    static const unsigned NUM_VERTICES = 3;
};

template<bool isDeformable = false>
class TriangleV1:
    public IShape<  Vec3,
                    isDeformable,
                    TriangleV1<isDeformable>,
                    TriangleIndices>
{
public:
    TriangleV1():IShape<Vec3,
                        isDeformable,
                        TriangleV1<isDeformable>,
                        TriangleIndices>({Vec3(), Vec3(), Vec3()}){}
};

template<typename T, bool isTriangleDeformable = false>
class Tri
{
public:
    typedef typename IsPropertyModifiable<T,isTriangleDeformable>::type VertType;
    typedef typename IsPropertyModifiable<T,isTriangleDeformable>::type VecType;

    enum Edge
    {
        eP0P1 = 0,  ///< Vector from P0 to P1
        eP0P2 = 1,  ///< Vector from P0 to P2
        eP1P2 = 2   ///< Vector from P1 to P2
    };

    enum Vert
    {
        eP0 = 0,    ///< Refers to Vertex P0
        eP1 = 1,    ///< Refers to Vertex P1
        eP2 = 2     ///< Refers to Vertex P2
    };

    Tri(const T& v0, const T& v1, const T& v2):
        m_verts({v0, v1, v2}),
        m_edges({m_verts[1]-m_verts[0],
                 m_verts[2]-m_verts[0],
                 m_verts[2]-m_verts[1]}),
        m_normal(Vec3::crossProduct(m_edges[eP0P1],m_edges[eP0P2]).normalise()){}

    VertType& Vert(Vert vertID){
        return m_verts[static_cast<short>(vertID)];
    }

    VecType& Edge(Edge edgeID){
        return m_edges[static_cast<short>(edgeID)];
    }

    static const short NUM_VERTS = 3;
    VertType m_verts[NUM_VERTS];
    VecType m_edges[NUM_VERTS];
    VecType m_normal;

};
}
int main()
{
    Point p;
    Point q; q.Y() = 10.0f;
    TriangleV1<false> t;
    t.Vert(TriangleIndices::VertIndices::eP0);

}
