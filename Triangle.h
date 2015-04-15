#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vect.h"
#include "Color.h"
#include "Material.h"


class Triangle : public Object {
private:
	Vect v0, v1, v2;
	Vect normal;

	Color color;
	Material material;

	double distance;
public:
	Triangle(Vect, Vect, Vect, Color, Material);

	Vect GetNormal()
	{
		normal = v1.VectAdd(v0.Negative()).CrossProduct(v2.VectAdd(v0.Negative())).Normalize();
		return normal;
	}

	virtual Vect GetNormalAt(Vect point) 
	{
		return normal;
	}

	double GetTriangleDistance()
	{
		distance = -normal.DotProduct(v0);
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

	virtual double FindIntersection(Ray r)
	{
		Vect rayDirection = r.GetRayDirection();
		Vect rayOrigin	  = r.GetRayOrigin();

		GetNormal();
		GetTriangleDistance();
		double a = rayDirection.DotProduct(normal);

		if (a == 0)
		{
			// ray is parallel to plane
			return -1;
		}
		else
		{
			double b = normal.DotProduct(rayOrigin.VectAdd(normal.VectMult(distance).Negative()));
			double distanceToPlane = -1 * b / a;

			// if (distanceToPlane <= 0.000001) {
			// 	printf("Too small\n");
			// 	return -1;
			// }

			Vect P = rayOrigin.VectAdd(rayDirection.VectMult(distanceToPlane));

			Vect Pv0 = P.VectAdd(v0.Negative());
			Vect Pv1 = P.VectAdd(v1.Negative());
			Vect Pv2 = P.VectAdd(v2.Negative());

			bool test0 = v1.VectAdd(v0.Negative()).CrossProduct(Pv0).DotProduct(normal) >= 0;
			bool test1 = v2.VectAdd(v1.Negative()).CrossProduct(Pv1).DotProduct(normal) >= 0;
			bool test2 = v0.VectAdd(v2.Negative()).CrossProduct(Pv2).DotProduct(normal) >= 0;

			// // (CA x PA) . N >= 0
			// bool test0 = u.CrossProduct(Pv0).DotProduct(normal) >= 0;
			// // (AB x PB) . N >= 0
			// bool test1 = v.Negative().CrossProduct(Pv1).DotProduct(normal) >= 0;
			// // bool test1 = v.Negative().CrossProduct(Pv1).DotProduct(normal) >= 0;
			// // (BC x PC) . N >= 0
			// bool test2 = t.CrossProduct(Pv2).DotProduct(normal) >= 0;

			 if (test0 == true && test1 == true && test2 == true)
			 {
				 // inside triangle
				 return distanceToPlane;
			 }
			 else
			 {
				 // outside the triangle
				 return -1;
			 }
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

		double newX, newY, newZ;
		// for v0
		newX = Rx.getVectX() * v0.getVectX() +
					  Rx.getVectY() * v0.getVectY() +
					  Rx.getVectZ() * v0.getVectZ();

		newY = Ry.getVectX() * v0.getVectX() +
					  Ry.getVectY() * v0.getVectY() +
					  Ry.getVectZ() * v0.getVectZ();

		newZ = Rz.getVectX() * v0.getVectX() +
					  Rz.getVectY() * v0.getVectY() +
					  Rz.getVectZ() * v0.getVectZ();

		v0 = Vect(newX, newY, newZ);

		// for v1
		newX = Rx.getVectX() * v1.getVectX() +
					  Rx.getVectY() * v1.getVectY() +
					  Rx.getVectZ() * v1.getVectZ();

		newY = Ry.getVectX() * v1.getVectX() +
					  Ry.getVectY() * v1.getVectY() +
					  Ry.getVectZ() * v1.getVectZ();

		newZ = Rz.getVectX() * v1.getVectX() +
					  Rz.getVectY() * v1.getVectY() +
					  Rz.getVectZ() * v1.getVectZ();

		v1 = Vect(newX, newY, newZ);

		// for v2
		newX = Rx.getVectX() * v2.getVectX() +
					  Rx.getVectY() * v2.getVectY() +
					  Rx.getVectZ() * v2.getVectZ();

		newY = Ry.getVectX() * v2.getVectX() +
					  Ry.getVectY() * v2.getVectY() +
					  Ry.getVectZ() * v2.getVectZ();

		newZ = Rz.getVectX() * v2.getVectX() +
					  Rz.getVectY() * v2.getVectY() +
					  Rz.getVectZ() * v2.getVectZ();

		v2 = Vect(newX, newY, newZ);
	}

	virtual void Scale(float scalar) {
		v0 = v0.VectMult(scalar);
		v1 = v1.VectMult(scalar);
		v2 = v2.VectMult(scalar);
	}

	virtual void Translate(Vect transform) {
		v0 = v0.VectAdd(transform);
		v1 = v1.VectAdd(transform);
		v2 = v2.VectAdd(transform);
	}
};

Triangle::Triangle(Vect a, Vect b, Vect c, Color co, Material m)
{
	v0 = a;
	v1 = b;
	v2 = c;

	color = co;
	material = m;
}

#endif // TRIANGLE_H
