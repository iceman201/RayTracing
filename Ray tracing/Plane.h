/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_PLANE
#define H_PLANE

#include "Vector.h"
#include "Object.h"

class Plane : public Object
{
private:
    Vector a, b, c, d;      //The 4 vertices of a quad

public:	
	Plane(void);
	
    Plane(Vector pa, Vector pb, Vector pc, Vector pd, Color col)
		: a(pa), b(pb), c(pc), d(pd)
	{
		color = col;
	};

	bool isInside(Vector pos);
	
	float intersect(Vector pos, Vector dir);
	
	Vector normal(Vector pos);

};

#endif //!H_PLANE
