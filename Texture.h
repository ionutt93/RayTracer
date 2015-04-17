#ifndef TEXTURE_H
#define TEXTURE_H

#include "Color.h"
#include "SphericalMapping.h"
#include "Image.h"

using namespace std;

class Texture
{
	Image* image;
	SphericalMapping* mapping;

	int hres;
	int vres;


public:
	Texture(int, int, const char*);
	Color GetTextureColorAt(Vect);
	
};


Texture::Texture(int h, int v, const char* fileName) 
{
	hres = h;
	vres = v;

	image   = new Image(fileName);
	mapping = new SphericalMapping();
}

Color Texture::GetTextureColorAt(Vect hitpoint)
{
	int x, y;
	
	mapping->GetTextureCoord(hitpoint, hres, vres, x, y);
	float blue = (float) image->imageData[x][y * 3 + 2] / 255.0f;
	float green = (float) image->imageData[x][y * 3 + 3] / 255.0f;
	float red = (float) image->imageData[x][y * 3 + 4] / 255.0f;

	return Color(red, green, blue);
}


#endif // TEXTURE_H
