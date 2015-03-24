#ifndef VECT_H
#define VECT_H

#include "math.h"

class Vect
{
	double x;
	double y;
	double z;

public:
	Vect();

	Vect(double, double, double);

	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }

	double Magnitude();

	Vect Normalize();

	Vect Negative();

	double DotProduct(Vect v);

	Vect CrossProduct(Vect v);

	Vect VectAdd(Vect v);

	Vect VectMult(double scalar);
};


Vect::Vect()
{
	x = 0;
	y = 0;
	z = 0;
}


Vect::Vect(double i, double j, double k)
{
	x = i;
	y = j;
	z = k;
}


double Vect::Magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}


Vect Vect::Normalize()
{
	double magnitude = Magnitude();
	return Vect(x / magnitude, y / magnitude, z / magnitude);
}


Vect Vect::Negative()
{
	return Vect(-x, -y, -z);
}


double Vect::DotProduct(Vect v)
{
	return (x * v.getVectX()) + (y * v.getVectY()) + (z * v.getVectZ());
}


Vect Vect::CrossProduct(Vect v)
{
	 return Vect((y * v.getVectZ()) - (z * v.getVectY()),
	 			 (z * v.getVectX()) - (x * v.getVectZ()),
	 			 (x * v.getVectY()) - (y * v.getVectX()));
}


Vect Vect::VectAdd(Vect v)
{
	return Vect((x + v.getVectX()),
				(y + v.getVectY()),
				(z + v.getVectZ()));
}


Vect Vect::VectMult(double scalar)
{
	return Vect((scalar * x),
				(scalar * y),
				(scalar * z));
}

#endif // VECT_H




















