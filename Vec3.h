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
        std::array<T, 3> m_coords;
    public:

        static constexpr T EPSILON = T(1.0E-12);

        // Default constructor
        Vec3_():m_coords({0, 0, 0}){}

        // Three parameter constructor
        Vec3_(const T& xValue, const T& yValue, const T& zValue)
        :m_coords({xValue, yValue, zValue})
        {

        }

        Vec3_(const Vec3_<T>& vec):
        m_coords({vec.X(), vec.Y(), vec.Z()})
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
                return Vec3_<T>(X()*oneOverMag, Y()*oneOverMag, Z()*oneOverMag);
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
            return X() * vec.X() + Y() * vec.Y() + Z() * vec.Z();
        }

        bool isSameAs(const Vec3_& vec, const T tol = EPSILON)const
        {
            return  fabs(X()-vec.X()) < tol &&
                    fabs(Y()-vec.Y()) < tol &&
                    fabs(Z()-vec.Z()) < tol;
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
            return Vec3_<T>(X() * value, Y() * value, Z() * value);
        }

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator*=(const T &value)
        {
            X() *= value;
            Y() *= value;
            Z() *= value;
        }

        Vec3_<T> operator-(const Vec3_<T>& vec)const
        {
            return Vec3_<T>(X() - vec.X(), Y() - vec.Y(), Z() - vec.Z());
        }

        Vec3_<T> operator+(const Vec3_<T>& vec)const
        {
            return Vec3_<T>(X() + vec.X(), Y() + vec.Y(), Z() + vec.Z());
        }

        Vec3_<T>& operator=(const Vec3_<T>& vec)
        {
            X() = vec.X();
            Y() = vec.Y();
            Z() = vec.Z();
            return *this;
        }

        // Overloaded multiply operator to multiply a vector by a scalar
        Vec3_ operator/(const T &value) const
        {
            return Vec3_<T>(X() / value, Y() / value, Z() / value);
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
