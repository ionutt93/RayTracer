#ifndef TEXTURE_H
#define TEXTURE_H

#include "CImg.h"
#include "Color.h"

using namespace cimg_library; 

class Texture
{
	CImg<unsigned char> image;
	int hres;
	int vres;

public:
	Texture(string);

	Color GeTexturetColorAt(int, int);
	
};

Texture::Texture(string source)
{
	image = CImg<unsigned char>(source);
	hres  = image.dimx();
	vres  = image.dimy();
}

Color Texture::GetTextureColorAt(double u, double v)
{
	int x = (hres - 1) * u;
	int y = (vres - 1) * v;
}


#endif // TEXTURE_H
