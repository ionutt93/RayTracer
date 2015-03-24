#ifndef COLOR_H
#define COLOR_H

class Color
{
	double red, green, blue, special;

public:
	Color();
	
	Color(double, double, double, double);

	double GetColorRed()     { return red;     }
	double GetColorGreen()   { return green;   }
	double GetColorBlue()    { return blue;    }
	double GetColorSpecial() { return special; }

	void SetColorRed(double value)     { red     = value; }
	void SetColorGreen(double value)   { green   = value; }
	void SetColorBlue(double value)    { blue    = value; }
	void SetColorSpecial(double value) { special = value; }

};

Color::Color()
{
	red   = 0.5;
	green = 0.5;
	blue  = 0.5;
}

Color::Color(double r, double g, double b, double s)
{
	red     = r;
	green   = g;
	blue    = b;
	special = s;
}

#endif // COLOR_H
