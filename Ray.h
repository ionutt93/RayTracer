#ifndef RAY_H
#define RAY_H

#include "Vect.h"

class Ray
{
	Vect origin;
	Vect direction;
public:
	Ray();

	Ray(Vect, Vect);

	Vect GetRayOrigin() { return origin; }
	Vect GetRayDirection() { return direction; }
};


Ray::Ray()
{
	origin    = Vect(0, 0, 0);
	direction = Vect(1, 0, 0);
}

Ray::Ray(Vect o, Vect d)
{
	origin    = o;
	direction = d;
}

#endif // RAY_H

