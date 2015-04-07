#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vect.h"
#include "Color.h"
#include "Material.h"


class Triangle : public Object {
private:
	Vect v0, v1, v2;
	Vect normal, u, v, t;

	Color color;
	Material material;

	double distance;
public:
	Triangle(Vect, Vect, Vect, Color, Material);

	Vect GetNormal()
	{
		normal = u.CrossProduct(v).Normalize();
		return normal;
	}

	virtual Vect GetNormalAt(Vect point) 
	{
		return normal;
	}

	double GetTriangleDistance()
	{
		distance = normal.DotProduct(v0);
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
			double distanceToPlane = -1 * b / a - 0.000001;

			Vect P = rayOrigin.VectAdd(rayDirection.VectMult(distanceToPlane));

			// PA
			Vect Pv0 = P.VectAdd(v0.Negative());
			// PB
			Vect Pv1 = P.VectAdd(v1.Negative());
			// PC
			Vect Pv2 = P.VectAdd(v2.Negative());

			// (CA x PA) . N >= 0
			bool test0 = u.CrossProduct(Pv0).DotProduct(normal) >= 0;
			// (AB x PB) . N >= 0
			bool test1 = v.Negative().CrossProduct(Pv1).DotProduct(normal) >= 0;
			// bool test1 = v.Negative().CrossProduct(Pv1).DotProduct(normal) >= 0;
			// (BC x PC) . N >= 0
			bool test2 = t.CrossProduct(Pv2).DotProduct(normal) >= 0;

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
};

Triangle::Triangle(Vect a, Vect b, Vect c, Color co, Material m)
{
	v0 = a;
	v1 = b;
	v2 = c;

	// CA
	u = v2.VectAdd(v0.Negative());
	// BA
	v = v1.VectAdd(v0.Negative());
	// BC
	t = v1.VectAdd(v2.Negative());

	color = co;
	material = m;
}

#endif // TRIANGLE_H
