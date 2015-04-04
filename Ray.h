#ifndef RAY_H
#define RAY_H

#include "Vect.h"

class Ray
{
	int id;
	int remainingIntersections;	
	Vect origin;
	Vect direction;
	double refractiveIndex;

public:
	Ray();

	Ray(Vect, Vect);

	Ray(Vect, Vect, double, int);

	Vect GetRayOrigin() 
	{ 
		return origin; 
	}

	Vect GetRayDirection() 
	{ 
		return direction; 
	}

	double GetRefractiveIndex()
	{
		return refractiveIndex;
	}

	int GetID()
	{
		return id;
	}

	int GetRemainingIntersections()
	{
		return remainingIntersections;
	}

	void SetRefractiveIndex(double r)
	{
		refractiveIndex = r;
	}

	void SubstractIntersection()
	{
		if (remainingIntersections > 0)
		{
			remainingIntersections--;
		}
	}
};


Ray::Ray()
{
	origin          = Vect(0, 0, 0);
	direction       = Vect(1, 0, 0);
	refractiveIndex = 1.000293;
	id = 0;
}

Ray::Ray(Vect o, Vect d)
{
	origin          = o;
	direction       = d;
	refractiveIndex = 1.000293;
	id = 0;
}

Ray::Ray(Vect o, Vect d, double r, int i)
{
	origin          = o;
	direction       = d;
	refractiveIndex = r;
	id = i;
}

#endif // RAY_H

