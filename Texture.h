#ifndef TEXTURE_H
#define TEXTURE_H

#include "Color.h"
#include "SphericalMapping.h"

using namespace std;

class Texture
{
	vector<Color> image;
	SphericalMapping* mapping;

	int hres;
	int vres;

public:
	Texture(int, int);
	Color GetTextureColorAt(Vect);
	
};


Texture::Texture(int h, int v)
{
	hres = h;
	vres = v;

	image.reserve(hres * vres);

	for (int i = 0; i < vres; i++) {
		for (int j = 0; j < hres; j++) {
			double r = ((double) rand() / (double) (RAND_MAX));
			double g = ((double) rand() / (double) (RAND_MAX));
			double b = ((double) rand() / (double) (RAND_MAX));

			image.push_back(Color(r, g, b));
		}
	}

	mapping = new SphericalMapping();
}

Color Texture::GetTextureColorAt(Vect hitpoint)
{
	int x, y;
	mapping->GetTextureCoord(hitpoint, hres, vres, x, y);
	return image.at(x * hres + y);
}


#endif // TEXTURE_H
