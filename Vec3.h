#pragma once
#include <iostream>
#include <math.h>
#include <system_error>
#include <array>
namespace rabbit
{

template <class T>
class Vec3_
{
    private:
        T x, y, z;
        std::array<T, 3> m_coords;
    public:

        static constexpr T EPSILON = T(1.0E-12);

        // Default constructor
        Vec3_():x(T(0)),y(T(0)),z(T(0)),m_coords({0, 0, 0}){}

        // Three parameter constructor
        Vec3_(const T& xValue, const T& yValue, const T& zValue)
        :m_coords({xValue, yValue, zValue})
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        Vec3_(const Vec3_<T>& vec):
        x(vec.X()),y(vec.Y()),z(vec.Z()),m_coords({vec.X(), vec.Y(), vec.Z()})
        {}

        T& X() { return m_coords[0]; }
        const T& X() const {return m_coords[0];}

        T& Y() { return m_coords[1];}
        const T& Y() const  { return m_coords[1]; }

        T& Z() { return m_coords[2]; }
        const T& Z() const  { return m_coords[2]; }

        // Method to normalise a vector
        Vec3_<T> normalise()const
        {
            T mag = magnitude();

            if (mag > EPSILON)
            {
                T oneOverMag = 1.0/mag;
                return Vec3_<T>(x*oneOverMag, y*oneOverMag, z*oneOverMag);
            }
            else
            {
                throw std::runtime_error("Attempted to normalise zero vector");
            }
        }

        T magnitude()const
        {
            return sqrt((X() * X()) + (Y() * Y()) + (Z() * Z()));
        }

        static T dotProduct(const Vec3_ &vec1, const Vec3_ &vec2)
        {
            return vec1.X() * vec2.X() + vec1.Y() * vec2.Y() + vec1.Z() * vec2.Z();
        }

        T dotProduct(const Vec3_ &vec) const
        {
            return x * vec.X + y * vec.Y + z * vec.Z;
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
            return Vec3_(vec1.Y() * vec2.Z() - vec1.Z() * vec2.Y(),
                        vec1.Z() * vec2.X() - vec1.X() * vec2.Z(),
                        vec1.X() * vec2.Y() - vec1.Y() * vec2.X());
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
