/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The vector class
*  Use this class for defining vectors and points
*    and for performing operations on them.
-------------------------------------------------------------*/

#include <math.h>
#include "Vector.h"

// Default constructor, initialises all values to 0. 
Vector::Vector() : x(0), y(0), z(0) {}

// Constructor that takes initial x, y, z  values.
Vector::Vector(float a_x, float a_y, float a_z)
: x(a_x), y(a_y), z(a_z) {}


// Scales the input vector by scale 
const Vector& Vector::operator*=(float scale)
{
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
}

// Returns a new vector, containing this vector scaled by the input scale factor
Vector Vector::operator*(float scale) const 
{
    return Vector(*this) *= scale;
}

// Divides this vector by the input scale
const Vector& Vector::operator/=(float scale) 
{
    return operator*=(1.0f / scale);
}

// Returns a new vector containing the value of this vector divide by the given scale
Vector Vector::operator/(float scale) const 
{
    return Vector(*this) *= 1.0f / scale;
}

// Add the given vector to this one.
const Vector& Vector::operator+=(const Vector rhs) 
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// Returns a new vector containing the result of adding the given vector to this one.
Vector Vector::operator+(const Vector rhs) const 
{
    return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}

// Subtracts the given vector from this one.
const Vector& Vector::operator-=(const Vector rhs) 
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

// Returns a new vector containing the result of subtract rhs from this one.
Vector Vector::operator-(const Vector rhs) const 
{
    return Vector(x -rhs.x, y - rhs.y, z - rhs.z);
}

// Compares two vectors
const bool Vector::operator<(const Vector rhs) 
{
    float l1 = length();
    float l2 = rhs.length();
    return (l1 < l2);
}

// Scales the current vector by the given scale factor.
void Vector::scale(float scale) 
{
    operator*=(scale);
}

// Returns the cross product of this vector and rhs.
Vector Vector::cross(const Vector rhs) const 
{
    float tx = y * rhs.z - z * rhs.y;
    float ty = z * rhs.x - x * rhs.z;
    float tz = x * rhs.y - y * rhs.x;
    return Vector(tx, ty, tz);
}

// Gives the dot product of this vector and rhs
float Vector::dot(const Vector rhs) const 
{
    return x*rhs.x + y*rhs.y + z*rhs.z;
}

// Gives the distance between this point and rhs
float Vector::dist(const Vector rhs) const 
{
    Vector vdif = (*this)-rhs;
    return vdif.length(); 
}

// Returns the length of this vector
float Vector::length() const 
{
    return sqrt(dot(*this));
}

//Normalises this vector.
void Vector::normalise() 
{
    scale(1.0f / length());
}

