#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object
{

public:
	Object();

	virtual Color GetColor() { return Color(0.0, 0.0, 0.0, 0.0); }

	virtual	double FindIntersection(Ray ray) { return 0; }
};

Object::Object() {}

#endif // OBJECT_H
