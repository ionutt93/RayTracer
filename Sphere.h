#ifndef SPHERE_H
#define SPHERE_H

#include "math.h"
#include "Vect.h"
#include "Color.h"
#include "Object.h"

class Sphere : public Object
{
	Vect center;
	double radius;
	Color color;

public:
	Sphere();

	Sphere(Vect, double, Color);
	
	Vect GetSphereCenter()   { return center; }
	double GetSphereRadius() { return radius; }
	Color GetSphereColor()   { return color;  }
};

Sphere::Sphere()
{
	center = Vect(0, 0, 0);
	radius = 1.0;
	color  = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect c, double r, Color co)
{
	center = c;
	radius = r;
	color  = co;
}

#endif // SPHERE_H
