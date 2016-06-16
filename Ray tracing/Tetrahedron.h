/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/
#ifndef H_TETRAHEDRON
#define H_TETRAHEDRON

#include "Vector.h"
#include "Object.h"

class Tetrahedron : public Object
{
private:
    Vector a, b, c;      //The 4 vertices of a quad

public:	
	Tetrahedron(void);
	
    Tetrahedron (Vector pa, Vector pb, Vector pc, Color col)
		: a(pa), b(pb), c(pc)
	{
		color = col;
	};

	bool isInside(Vector pos);
	
	float intersect(Vector pos, Vector dir);
	
	Vector normal(Vector pos);

};
#endif //!H_TETRAHEDRON
