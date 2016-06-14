/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The vector class
*  Use this class for defining vectors and points
*    and for performing operations on them.
-------------------------------------------------------------*/

#ifndef H_VECTOR
#define H_VECTOR

class Vector 
{
public:
    float x, y, z;

    Vector();

    Vector(float a_x, float a_y, float a_z);
 
    const Vector& operator*=(float scale);
 
    Vector operator*(float scale) const;
 
    const Vector& operator/=(float scale);

    Vector operator/(float scale) const;

    const Vector& operator+=(const Vector rhs);

    Vector operator+(const Vector rhs) const;

    const Vector& operator-=(const Vector rhs);
 
    Vector operator-(const Vector rhs) const;

	const bool operator==(const Vector rhs);

	const bool operator!=(const Vector rhs);

	const bool operator<(const Vector rhs);

    void scale(float scale);

    Vector cross(const Vector rhs) const;
 
    float dot(const Vector rhs) const;

    float dist(const Vector rhs) const;

    float length() const;

    void normalise();

};

#endif //!H_VECTOR
