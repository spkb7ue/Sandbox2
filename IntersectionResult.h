#pragma once

namespace rabbit
{

/**
* @tparam T struct used for representing a point.
* @brief IntersectionResult is a helper struct, could have gotten
* away with an std::pair<T,double> instead.
*/
template<typename T>
struct IntersectionResult
{
    IntersectionResult(const T& intersectionPoint, double dist):
        Point(intersectionPoint),Dist(dist){}

    T Point;
    double Dist;
};

/**
* @tparam T struct used for representing a point.
*/
template<typename T>
struct PointSegIntersection
{
    PointSegIntersection(const T& intersectionPoint,
                         double dist,
                         bool isWithinExtrudedNormal):
        IsWithinExtrudedNormal(isWithinExtrudedNormal),
        IRes(intersectionPoint, dist){}

    bool IsWithinExtrudedNormal; ///< true if point of interest lies within the cylindrical volume
								 ///< formed by the segment as an axis and the largest normal to it.

    IntersectionResult<T> IRes; ///< Intersection result. See above
};

}
