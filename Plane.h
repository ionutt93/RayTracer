#ifndef PLANE_H
#define PLANE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object
{
private:
	Vect normal;
	double distance;
	Color color;
	Material material;

public:
	Plane();

	Plane(Vect n, double d, Color c, Material m);
	
	Vect GetPlaneNormal() 
	{ 
		return normal;
	}

	double GetPlaneDistance() 
	{ 
		return distance; 
	}

	virtual Color GetColor()
	{ 
		return color; 
	}

	virtual Material GetMaterial()
	{
		return material;
	}

	virtual Vect GetNormalAt(Vect point);

	virtual	double FindIntersection(Ray ray);
};

Plane::Plane() 
{
	normal   = Vect(1.0, 0.0, 0.0);
	distance = 0.0;
	color    = Color(0.5, 0.5, 0.5);
	material = Material();
}

Plane::Plane(Vect n, double d, Color c, Material m)
{
	normal   = n;
	distance = d;
	color    = c;
	material = m;
}

Vect Plane::GetNormalAt(Vect point)
{
	return normal;
}

double Plane::FindIntersection(Ray ray)
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
		// N . (RO - N * d)  
		double b = normal.DotProduct(ray.GetRayOrigin().VectAdd(normal.VectMult(distance).Negative()));
		// double b = ray.GetRayOrigin().DotProduct(normal) + distance;
		double result = -1 * b / a;
		if (result > 0.000001)
			return result;
		else
			return -1;
		// return -1 * b / a - 0.000001;
	}
}
#endif // PLANE_H

