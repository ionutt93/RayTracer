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

	double GetBrightness()
	{
		return (red + green + blue) / 3;
	}
	
	Color ColorScalar(double scalar)
	{
		return Color(red * scalar, 
					 green * scalar, 
					 blue * scalar, 
					 special);
	}

	Color ColorAdd(Color c)
	{
		return Color(red   + c.GetColorRed(), 
					 green + c.GetColorGreen(), 
					 blue  + c.GetColorBlue(),
					 special);
	}

	Color Multiply(Color c)
	{
		return Color(red   * c.GetColorRed(), 
					 green * c.GetColorGreen(), 
					 blue  * c.GetColorBlue(),
					 special);
	}

	Color ColorAverage(Color c)
	{
		return Color((red   + c.GetColorRed())   / 2, 
					 (green + c.GetColorGreen()) / 2, 
					 (blue  + c.GetColorBlue())  / 2,
					 special);
	}

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
