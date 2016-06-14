/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_SPHERE
#define H_SPHERE

#include "Object.h"

/**
 * Defines a simple Sphere located at 'center' 
 * with the specified radius
 */
class Sphere : public Object
{

private:
    Vector center;
    float radius;

public:	
	Sphere()
		: center(Vector()), radius(1)  //Default constructor creates a unit sphere
	{
		color = Color::WHITE;
	};
	
    Sphere(Vector c, float r, Color col)
		: center(c), radius(r)
	{
		color = col;
	};

	float intersect(Vector pos, Vector dir);

	Vector normal(Vector p);

};

#endif //!H_SPHERE
