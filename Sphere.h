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
	virtual Color GetColor()   { return color;  }

	virtual Vect GetNormalAt(Vect point)
	{
		Vect normalVect = point.VectAdd(center.Negative()).Normalize();
		return normalVect;
	}

	virtual	double FindIntersection(Ray ray)
	{
		Vect rayOrigin = ray.GetRayOrigin();
		double rayOriginX = rayOrigin.getVectX(); 
		double rayOriginY = rayOrigin.getVectY(); 
		double rayOriginZ = rayOrigin.getVectZ(); 

		Vect rayDirection = ray.GetRayDirection();
		double rayDirectionX = rayDirection.getVectX(); 
		double rayDirectionY = rayDirection.getVectY(); 
		double rayDirectionZ = rayDirection.getVectZ(); 

		Vect sphereCenter = center;
		double sphereCenterX = sphereCenter.getVectX(); 
		double sphereCenterY = sphereCenter.getVectY(); 
		double sphereCenterZ = sphereCenter.getVectZ(); 

		double a = 1;
		double b = (2 * (rayOriginX - sphereCenterX) * rayDirectionX) + 
				   (2 * (rayOriginY - sphereCenterY) * rayDirectionY) + 
				   (2 * (rayOriginZ - sphereCenterZ) * rayDirectionZ);
		double c = pow((rayOriginX - sphereCenterX), 2) + 
				   pow((rayOriginY - sphereCenterY), 2) + 
				   pow((rayOriginZ - sphereCenterZ), 2) -
				   (radius * radius);
		double discriminant = b * b - 4 * c;

		if (discriminant > 0)
		{
			// the ray intersects the sphere
			// the first root
			double root1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

			if (root1 > 0)
			{
				// the first root is the smallest positive root
				return root1;
			}
			else
			{
				// the second root is the smallest positive root
				double root2 = ((sqrt(discriminant) - b) / 2) - 0.000001;	
				return root2;
			}
		}
		else
		{
			// the ray missed the sphere:w
			return -1;
		}
	}
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
