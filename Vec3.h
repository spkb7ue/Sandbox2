#pragma once
#include <iostream>
#include <math.h>

namespace rabbit
{

template <class T> class Vec3_
{
    private:
        // A Vec3 simply has three properties called x, y and z
        T x, y, z;

    public:
        // ------------ Constructors ------------

        // Default constructor
        Vec3_() { x = y = z = 0; };

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

        // ------------ Getters and setters ------------

        void set(const T &xValue, const T &yValue, const T &zValue)
        {
            x = xValue;
            y = yValue;
            z = zValue;
        }

        T& X() { return x; }
        const T& X() const {return x;}

        T& Y() { return y;}
        const T& Y() const  { return y; }

        T& Z() { return z; }
        const T& Z() const  { return z; }

        // ------------ Helper methods ------------

        // Method to reset a vector to zero
        void zero()
        {
            x = y = z = 0;
        }

        // Method to normalise a vector
        Vec3_<T>& normalise()
        {
            // Calculate the magnitude of our vector
            T magnitude = sqrt((x * x) + (y * y) + (z * z));

            // As long as the magnitude isn't zero, divide each element by the magnitude
            // to get the normalised value between -1 and +1
            if (magnitude != 0)
            {
                x /= magnitude;
                y /= magnitude;
                z /= magnitude;
            }
            return *this;
        }

        // Static method to calculate and return the scalar dot product of two vectors
        //
        // Note: The dot product of two vectors tell us things about the angle between
        // the vectors. That is, it tells us if they are pointing in the same direction
        // (i.e. are they parallel? If so, the dot product will be 1), or if they're
        // perpendicular (i.e. at 90 degrees to each other) the dot product will be 0,
        // or if they're pointing in opposite directions then the dot product will be -1.
        //
        // Usage example: double foo = Vec3<double>::dotProduct(vectorA, vectorB);
        static T dotProduct(const Vec3_ &vec1, const Vec3_ &vec2)
        {
            return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
        }

        // Non-static method to calculate and return the scalar dot product of this vector and another vector
        //
        // Usage example: double foo = vectorA.dotProduct(vectorB);
        T dotProduct(const Vec3_ &vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
        
        // Usage example: Vec3<double> crossVect = Vec3<double>::crossProduct(vectorA, vectorB);
        static Vec3_ crossProduct(const Vec3_ &vec1, const Vec3_ &vec2)
        {
            return Vec3_(vec1.y * vec2.z - vec1.z * vec2.y,
                        vec1.z * vec2.x - vec1.x * vec2.z,
                        vec1.x * vec2.y - vec1.y * vec2.x);
        }

        // Easy adders
        void addX(T value) { x += value; }
        void addY(T value) { y += value; }
        void addZ(T value) { z += value; }

        // Method to return the distance between two vectors in 3D space
        static T getDistance(const Vec3_ &v1, const Vec3_ &v2)
        {
            T dx = v2.x - v1.x;
            T dy = v2.y - v1.y;
            T dz = v2.z - v1.z;

            return sqrt(dx * dx + dy * dy + dz * dz);
        }

        // Method to display the vector so you can easily check the values
        void display()
        {
            std::cout << "X: " << x << "\t Y: " << y << "\t Z: " << z << std::endl;
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

        // Overloaded multiply and assign operator to multiply a vector by a scalar
        void operator/=(const T &value)
        {
            x /= value;
            y /= value;
            z /= value;
        }
};

typedef Vec3_<float> Point;
typedef Vec3_<float> Vec3;

}
