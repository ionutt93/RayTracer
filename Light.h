#ifndef LIGHT_H
#define LIGHT_H

#include "Vect.h"
#include "Color.h"
#include "Source.h"

class Light : public Source
{
	Vect position;
	Color color;
public:
	Light();
	Light(Vect, Color);

	virtual Vect GetLightPosition() { return position; }
	virtual Color GetLightColor()    { return color;    }
	
};

Light::Light()
{
	position = Vect(0, 0, 0);
	color    = Color(1, 1, 1, 0);
}

Light::Light(Vect p, Color c)
{
	position = p;
	color    = c;
}

#endif // LIGHT_H
