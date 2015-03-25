#ifndef OBJECT_H
#define OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object
{

public:
	Object();

	Color GetObjectColor() { return Color(0.0, 0.0, 0.0, 0.0); }

	double FindIntersection(Ray ray) { return 0; }
};

Object::Object() {}

#endif // OBJECT_H
