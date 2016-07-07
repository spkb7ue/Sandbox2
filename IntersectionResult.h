#pragma once

namespace rabbit
{

template<typename T>
struct IntersectionResult
{
    IntersectionResult(const T& intersectionPoint, double dist):
        Point(intersectionPoint),Dist(dist){}

    T Point;
    double Dist;
};

template<typename T>
struct PointSegIntersection
{
    PointSegIntersection(const T& intersectionPoint,
                         double dist,
                         bool isWithinExtrudedNormal):
        IsWithinExtrudedNormal(isWithinExtrudedNormal),
        IRes(intersectionPoint, dist){}

    bool IsWithinExtrudedNormal;
    IntersectionResult<T> IRes;
};

}
