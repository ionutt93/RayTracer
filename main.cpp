#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

struct RGBType
{
	double r;
	double g;
	double b;
};


void savebmp(const char *filename, int w, int h, int dpi, RGBType *data)
{
	FILE *f;
	int k = w * h;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi * m;
	unsigned char bmpFileHeader[14] = {'B', 'M', 0, 0, 0, 0,  0, 0, 0, 0,  54, 0, 0, 0};
	unsigned char bmpInfoHeader[40] = {40, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  1, 0, 24, 0};

	bmpFileHeader[2] = (unsigned char) (filesize);
	bmpFileHeader[3] = (unsigned char) (filesize>>8);
	bmpFileHeader[4] = (unsigned char) (filesize>>16);
	bmpFileHeader[5] = (unsigned char) (filesize>>24);

	bmpInfoHeader[4] = (unsigned char) (w);
	bmpInfoHeader[5] = (unsigned char) (w>>8);
	bmpInfoHeader[6] = (unsigned char) (w>>16);
	bmpInfoHeader[7] = (unsigned char) (w>>24);

	bmpInfoHeader[8]  = (unsigned char) (h);
	bmpInfoHeader[9]  = (unsigned char) (h>>8);
	bmpInfoHeader[10] = (unsigned char) (h>>16);
	bmpInfoHeader[11] = (unsigned char) (h>>24);

	bmpInfoHeader[21] = (unsigned char) (s);
	bmpInfoHeader[22] = (unsigned char) (s>>8);
	bmpInfoHeader[23] = (unsigned char) (s>>16);
	bmpInfoHeader[24] = (unsigned char) (s>>24);

	bmpInfoHeader[25] = (unsigned char) (ppm);
	bmpInfoHeader[26] = (unsigned char) (ppm>>8);
	bmpInfoHeader[27] = (unsigned char) (ppm>>16);
	bmpInfoHeader[28] = (unsigned char) (ppm>>24);

	bmpInfoHeader[29] = (unsigned char) (ppm);
	bmpInfoHeader[30] = (unsigned char) (ppm>>8);
	bmpInfoHeader[31] = (unsigned char) (ppm>>16);
	bmpInfoHeader[32] = (unsigned char) (ppm>>24);


	f = fopen(filename, "wb");
	fwrite(bmpFileHeader, 1, 14, f);
	fwrite(bmpInfoHeader, 1, 40, f);

	for (int i = 0; i < k; i++)
	{
		RGBType rgb = data[i];

		double red = data[i].r * 255;
		double green = data[i].g * 255;
		double blue = data[i].b * 255;

		unsigned char color[3] = {(int) floor(blue), (int) floor(green), (int) floor(red)};

		fwrite(color, 1, 3, f);
	}

	fclose(f);
}

using namespace std;

int main(int argc, char const *argv[])
{
	cout << "rendering ..." << endl;

	int dpi    = 72;
	int width  = 640;
	int height = 480;
	int n      = width * height;

	RGBType *pixels = new RGBType[n];
	int thisOne;

	Vect O(0, 0, 0);
	Vect X(1, 0, 0);
	Vect Y(0, 1, 0);
	Vect Z(0, 0, 1);
 
	Vect camPos(3.f, 1.5f, -4.f);
	Vect lookAt(0, 0, 0);
	Vect diffBtw(camPos.getVectX() - lookAt.getVectX(), camPos.getVectY() - lookAt.getVectY(), camPos.getVectZ() - lookAt.getVectZ());
	
	Vect camDir   = diffBtw.Negative().Normalize();
	Vect camRight = Y.CrossProduct(camDir).Normalize();
	Vect camDown  = camRight.CrossProduct(camDir).Normalize();

	Camera sceneCame(camPos, camDir, camRight, camDown);  

	Color whiteLight(1.0, 1.0, 1.0, 0.0);
	Color prettyGreen(0.5, 1.0, 0.5, 0.3); 
	Color gray(0.5, 0.5, 0.5, 0.0);
	Color black(0.0, 0.0, 0.0, 0.0);
	Color maroon(0.5, 0.25, 0.25, 0.0);

	Vect lightPosition(-7, 10, -10);
	Light sceneLight(lightPosition, whiteLight);

	// scene objects
	Sphere sceneSphere(O, 1, prettyGreen);
	Plane scenePlane(Y, -1, maroon);



	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			thisOne = x * width + y;
			
			if ((x > 200 && x < 280) && (y > 200 && y < 440))
			{
				pixels[thisOne].r = 23;
				pixels[thisOne].g = 222;
				pixels[thisOne].b = 10;
			}
			else
			{
				pixels[thisOne].r = 0;
				pixels[thisOne].g = 0;
				pixels[thisOne].b = 0;
			}
		}
	}

	savebmp("first_scene.bmp", width, height, dpi, pixels);

	return 0;
}
