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
	Material material;

public:
	Sphere();

	Sphere(Vect, double, Color, Material);
	
	Vect GetSphereCenter()   
	{ 
		return center; 
	}

	double GetSphereRadius() 
	{ 
		return radius; 
	}

	virtual Color GetColor()   
	{ 
		return color;  
	}

	virtual Material GetMaterial()
	{
		return material;
	}

	virtual Vect GetNormalAt(Vect point)
	{
		Vect normalVect = point.VectAdd(center.Negative()).Normalize();
		return normalVect;
	}

	virtual	double FindIntersection(Ray ray)
	{
		Vect rayOrigin = ray.GetRayOrigin(); 
		Vect rayDirection = ray.GetRayDirection();
		Vect sphereCenter = center;

		double a = rayDirection.DotProduct(rayDirection);
		double b = rayOrigin.VectAdd(sphereCenter.Negative()).VectMult(2).DotProduct(rayDirection);
		double c = rayOrigin.VectAdd(sphereCenter.Negative()).DotProduct(rayOrigin.VectAdd(sphereCenter.Negative())) - (radius * radius);
		double discriminant = b * b - 4 * a * c;

		if (discriminant > 0)
		{
			// the ray intersects the sphere
			// the first root
			double root1 = ((-1 * b - sqrt(discriminant)) / 2.0 * a);
			if (root1 > 0.000001)
			{
				// the first root is the smallest positive root
				return root1;
			}
			
			double root2 = ((-1 * b + sqrt(discriminant)) / 2.0 * a);
			if (root2 > 0.000001)
			{
				// the second root is the smallest positive root	
				return root2;
			}

			return -1;
		}
		else
		{
			// the ray missed the sphere
			return -1;
		}
	}

	virtual void Rotate(Vect axis, float amount) {
		double a = amount * axis.getVectX();
		double b = amount * axis.getVectY();
		double c = amount * axis.getVectZ();

		Vect Rx (cos(b) * cos(c), 
				-cos(b) * sin(c), 
				 sin(b));

		Vect Ry (cos(a) * sin(c) + sin(a) * sin(b) * cos(c),	
				 cos(a) * cos(c) - sin(a) * sin(b) * sin(c),
				-sin(a) * cos(b));

		Vect Rz (sin(a) * sin(c) - cos(a) * sin(b) * cos(c),
				 sin(a) * cos(c) + cos(a) * sin(b) * sin(c),
				 cos(a) * cos(b));

		double newX = Rx.getVectX() * center.getVectX() +
					  Rx.getVectY() * center.getVectY() +
					  Rx.getVectZ() * center.getVectZ();

		double newY = Ry.getVectX() * center.getVectX() +
					  Ry.getVectY() * center.getVectY() +
					  Ry.getVectZ() * center.getVectZ();

		double newZ = Rz.getVectX() * center.getVectX() +
					  Rz.getVectY() * center.getVectY() +
					  Rz.getVectZ() * center.getVectZ();

		center = Vect(newX, newY, newZ);
	}

	virtual void Translate(Vect transform) {
		center = center.VectAdd(transform);
	}

	virtual void Scale(float scalar) {
		radius = radius * scalar;
	}
};

Sphere::Sphere()
{
	center   = Vect(0, 0, 0);
	radius   = 1.0;
	color    = Color(0.5, 0.5, 0.5);
	material = Material();
}

Sphere::Sphere(Vect c, double r, Color co, Material m)
{
	center   = c;
	radius   = r;
	color    = co;
	material = m;
}

#endif // SPHERE_H
