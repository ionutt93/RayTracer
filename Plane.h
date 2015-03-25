#ifndef PLANE_H
#define PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object
{
	Vect normal;
	double distance;
	Color color;

public:
	Plane();

	Plane(Vect, double, Color);
	
	Vect GetPlaneNormal()     { return normal;   }
	double GetPlaneDistance() { return distance; }
	Color GetPlaneColor()     { return color;    }

	Vect GetNormalAt(Vect point)
	{
		return normal;
	}

	double FindIntersection(Ray ray)
	{
		Vect rayDirection = ray.GetRayDirection();

		double a = rayDirection.DotProduct(normal);
		if (a == 0)
		{
			// ray is parallel to our plane:w
			return -1;
		}
		else
		{
			double b = normal.DotProduct(ray.GetRayOrigin().VectAdd(normal.VectMult(distance).Negative()));
		}
	}
};

Plane::Plane()
{
	normal   = Vect(1.0, 0.0, 0.0);
	distance = 0.0;
	color    = Color(0.5, 0.5, 0.5, 0.0);
}


Plane::Plane(Vect n, double d, Color c)
{
	normal   = n;
	distance = d;
	color    = c;
}
#endif // PLANE_H

