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
	Vect point;
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

		double newX = Rx.getVectX() * normal.getVectX() +
					  Rx.getVectY() * normal.getVectY() +
					  Rx.getVectZ() * normal.getVectZ();

		double newY = Ry.getVectX() * normal.getVectX() +
					  Ry.getVectY() * normal.getVectY() +
					  Ry.getVectZ() * normal.getVectZ();

		double newZ = Rz.getVectX() * normal.getVectX() +
					  Rz.getVectY() * normal.getVectY() +
					  Rz.getVectZ() * normal.getVectZ();

		normal = Vect(newX, newY, newZ);
	}

	virtual void Scale(float scalar) {
		// A plane is infinite, cannot be scaled
	}

	virtual void Translate(Vect transform) {
		// TODO need to reimplement planes using a point of referrence 
	}
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
	double a = ray.GetRayDirection().DotProduct(normal);
	if (a == 0) {
		// ray is parallel to our plane
		return -1;
	}

	// N . (RO - N * d)  
	double b = normal.DotProduct(ray.GetRayOrigin().VectAdd(normal.VectMult(distance).Negative()));
	double result = -1 * b / a;
	if (result > 0.000001) {
		return result;
	}
		
	return -1;
}
#endif // PLANE_H

