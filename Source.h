#ifndef SOURCE_H
#define SOURCE_H

class Source
{
public:
	Source();

	virtual Vect GetLightPosition() { return Vect(0, 0, 0);     }
	virtual Color GetLightColor()   { return Color(1, 1, 1, 0); }
};

Source::Source()
{

}


#endif
