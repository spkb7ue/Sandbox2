#pragma once

namespace rabbit
{

/** 
* A simple helper struct to keep track of bounding boxes a bit more easily.
*/
struct Bounds
{
    Bounds(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax):
        xMin(xmin),xMax(xmax),yMin(ymin),yMax(ymax),zMin(zmin),zMax(zmax){}

    Bounds():xMin(0.0),xMax(0.0),yMin(0.0),yMax(0.0),zMin(0.0),zMax(0.0){}

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


} // namespace rabbit
