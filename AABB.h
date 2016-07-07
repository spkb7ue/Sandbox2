#pragma once
#include "IntersectionResult.h"
#include <limits>
#include <vector>

namespace rabbit
{

struct Bounds
{
    Bounds(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax):
        xMin(xmin),
        xMax(xmax),
        yMin(ymin),
        yMax(ymax),
        zMin(zmin),
        zMax(zmax){}

    Bounds():
        xMin(0.0),
        xMax(0.0),
        yMin(0.0),
        yMax(0.0),
        zMin(0.0),
        zMax(0.0){}

    template<typename VertType>
    Bounds(const VertType& center, const VertType& halfExtents):
        xMin(center.X() - halfExtents.X()),
        xMax(center.X() + halfExtents.X()),
        yMin(center.Y() - halfExtents.Y()),
        yMax(center.Y() + halfExtents.Y()),
        zMin(center.Z() - halfExtents.Z()),
        zMax(center.Z() + halfExtents.Z()){}

    double xMin;
    double xMax;
    double yMin;
    double yMax;
    double zMin;
    double zMax;
};

template<typename VertType>
class AABB
{
public:
    /**
    * @param center Center of the box from which the dimensions are calculated
    * @param halfExtents half dimensions of the box. Expected to be positive
    */
    AABB(const VertType& center, const VertType& halfExtents):
        m_center(center),
        m_halfExtents(halfExtents),
        m_bounds(center, halfExtents){}

    AABB(const AABB<VertType>&);
    AABB(const Bounds& bounds);

    /**
    * @param point Point of interest
    * @return true if the point is within the AABB.
    * Also return true if the point is on the face or the
    */
    bool IsPointWithinAABB(const VertType& point)const;

    IntersectionResult<VertType> CalcShortestDistanceFrom(const VertType& point,
                                                          double maxDist = std::numeric_limits<double>::max()) const;

    static AABB<VertType>
    CalculateAABB(const std::vector<AABB<VertType>>& aabbList);

    VertType Center() const{return m_center;}
    VertType HalfExtents() const{return m_halfExtents;}
    Bounds GetBounds()const{return m_bounds;}

    std::pair<double, int> GetLargestDim()
    {
        if(m_halfExtents.X() > m_halfExtents.Y())
        {
            if(m_halfExtents.X() > m_halfExtents.Z())
            {
                return std::make_pair(m_halfExtents.X(), 0);
            }
            else
            {
                return std::make_pair(m_halfExtents.Z(), 2);
            }
        }
        else
        {
            if(m_halfExtents.Y() > m_halfExtents.Z())
            {
                return std::make_pair(m_halfExtents.Y(), 1);
            }
            else
            {
                return std::make_pair(m_halfExtents.Z(), 2);
            }
        }
    }

  private:
    VertType m_center;
    VertType m_halfExtents;
    Bounds m_bounds;
};

template<typename VertType>
AABB<VertType>::AABB(const Bounds& b):
    m_center(VertType((b.xMax + b.xMin)*0.5,
                      (b.yMax + b.yMin)*0.5,
                      (b.zMax + b.zMin)*0.5)),
    m_halfExtents(VertType( (b.xMax - b.xMin)*0.5,
                            (b.yMax - b.yMin)*0.5,
                            (b.zMax - b.zMin)*0.5)),
    m_bounds(b)
{

}

template<typename VertType>
AABB<VertType> AABB<VertType>::CalculateAABB(const std::vector<AABB<VertType>>& aabbList)
{
    Bounds bounds;
    bool setInitialBounds = false;
    for(const AABB<VertType>& aabb : aabbList)
    {
        if(!setInitialBounds)
        {
            bounds = aabb.GetBounds();
            setInitialBounds = true;
        }
        else
        {
            // Update the bounds as required.
            const Bounds BOUNDS = aabb.GetBounds();

            bounds.xMin = std::min(bounds.xMin, BOUNDS.xMin);
            bounds.xMax = std::max(bounds.xMax, BOUNDS.xMax);

            bounds.yMin = std::min(bounds.yMin, BOUNDS.yMin);
            bounds.yMax = std::max(bounds.yMax, BOUNDS.yMax);

            bounds.zMin = std::min(bounds.zMin, BOUNDS.zMin);
            bounds.zMax = std::max(bounds.zMax, BOUNDS.zMax);
        }
    }

    return bounds;
}

template<typename VertType>
AABB<VertType>::AABB(const AABB<VertType>& aabb):
    m_center(aabb.Center()),
    m_halfExtents(aabb.HalfExtents()),
    m_bounds(aabb.GetBounds()){}

template<typename VertType>
bool AABB<VertType>::IsPointWithinAABB(const VertType& point)const{
    static const double EPSILON = 0.0000000001;
    return  point.X() >= m_bounds.xMin - EPSILON && point.X() <= m_bounds.xMax + EPSILON &&
            point.Y() >= m_bounds.yMin - EPSILON && point.Y() <= m_bounds.yMax + EPSILON &&
            point.Z() >= m_bounds.zMin - EPSILON && point.Z() <= m_bounds.zMax + EPSILON;
}

template<typename VertType>
IntersectionResult<VertType> AABB<VertType>::CalcShortestDistanceFrom(const VertType& point,
                                                                      double maxDist) const
{
    // labels for convenience
    const double px = point.X();
    const double py = point.Y();
    const double pz = point.Z();

    double x = (px < m_bounds.xMin) ?  m_bounds.xMin : (px > m_bounds.xMax ? m_bounds.xMax : px);
    double y = (py < m_bounds.yMin) ?  m_bounds.yMin : (py > m_bounds.yMax ? m_bounds.yMax : py);
    double z = (pz < m_bounds.zMin) ?  m_bounds.zMin : (pz > m_bounds.zMax ? m_bounds.zMax : pz);

    VertType closestPoint(x,y,z);
    double dist = (point - closestPoint).magnitude();
    return dist <= maxDist ? IntersectionResult<VertType>(closestPoint, dist) :
                             IntersectionResult<VertType>(VertType(), std::numeric_limits<double>::max());
}

} // namespace rabbit
