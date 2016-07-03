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
    t(p,q,p);
    p = q*4.0;
    t.Vert(TriangleProps3D::VertIndices::eP0) = q*4.0;

}
