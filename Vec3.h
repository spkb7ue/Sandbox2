#pragma once
#include <iostream>
#include <math.h>
#include <system_error>

namespace rabbit
{

template <class T> class Vec3_
{
    private:
        T x, y, z;
    public:

        static constexpr T EPSILON = T(1.0E-12);

        // Default constructor
        Vec3_():x(T(0)),y(T(0)),z(T(0)){}

        // Three parameter constructor
        Vec3_(T xValue, T yValue, T zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        Vec3_(const Vec3_<T>& vec):
        x(vec.X()),y(vec.Y()),z(vec.Z())
        {}

        T& X() { return x; }
        const T& X() const {return x;}

        T& Y() { return y;}
        const T& Y() const  { return y; }

        T& Z() { return z; }
        const T& Z() const  { return z; }

        // Method to normalise a vector
        Vec3_<T> normalise()const
        {
            // Calculate the magnitude of our vector
            T magnitude = sqrt((x * x) + (y * y) + (z * z));

            if (magnitude > EPSILON)
            {
                T oneOverMag = 1.0/magnitude;
                return Vec3_<T>(x*oneOverMag, y*oneOverMag, z*oneOverMag);
            }
            else
            {
                throw std::runtime_error("Attempted to normalise zero vector");
            }
        }

        T magnitude()const
        {
            return sqrt((x * x) + (y * y) + (z * z));
        }

        static T dotProduct(const Vec3_ &vec1, const Vec3_ &vec2)
        {
            return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
        }

        T dotProduct(const Vec3_ &vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        bool isSameAs(const Vec3_& vec, const T tol = EPSILON)const
        {
            return  fabs(x-vec.X()) < tol &&
                    fabs(y-vec.Y()) < tol &&
                    fabs(z-vec.Z()) < tol;
        }


        // Usage example: Vec3<double> crossVect = Vec3<double>::crossProduct(vectorA, vectorB);
        static Vec3_ crossProduct(const Vec3_ &vec1, const Vec3_ &vec2)
        {
            return Vec3_(vec1.y * vec2.z - vec1.z * vec2.y,
                        vec1.z * vec2.x - vec1.x * vec2.z,
                        vec1.x * vec2.y - vec1.y * vec2.x);
        }

        // Overloaded multiply operator to multiply a vector by a scalar
        Vec3_ operator*(const T &value) const
        {
            return Vec3_<T>(x * value, y * value, z * value);
        }

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator*=(const T &value)
        {
            x *= value;
            y *= value;
            z *= value;
        }

        Vec3_<T> operator-(const Vec3_<T>& vec)const
        {
            return Vec3_<T>(x - vec.X(), y - vec.Y(), z - vec.Z());
        }

        Vec3_<T> operator+(const Vec3_<T>& vec)const
        {
            return Vec3_<T>(x + vec.X(), y + vec.Y(), z + vec.Z());
        }

        Vec3_<T>& operator=(const Vec3_<T>& vec)
        {
            x = vec.X();
            y = vec.Y();
            z = vec.Z();
            return *this;
        }

        // Overloaded multiply operator to multiply a vector by a scalar
        Vec3_ operator/(const T &value) const
        {
            return Vec3_<T>(x / value, y / value, z / value);
        }
};

template<typename T>
std::ostream& operator << (std::ostream& os, const Vec3_<T>& v)
{
    os << "["; os << v.X()<<", "<<v.Y()<<", "<<v.Z();
    os << "]";
    return os;
}

typedef Vec3_<double> Point;
typedef Vec3_<double> Vec3;

}
