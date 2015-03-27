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

int WinningObjectIndex(vector<double> objectIntersections)
{
	// return the index of the winning intersection
	int indexOfMinimumValue;
	// prevent unnecesary calculations
	if (objectIntersections.size() == 0)
	{
		// if there are no intersections
		return -1;
	}
	else if (objectIntersections.size() == 1)
	{
		// if there is only one intersection
		if (objectIntersections.at(0) > 0)
		{
			// if that intersection is greater than 0 then it's our index of minimum value
			return 0;
		}
		else
		{
			// the only intersection value is negative
			return -1;
		}
	}
	else
	{
		// otherwise there is more than one intersection
		// first find the maximum value

		double max = 0;
		for (int i = 0; i < objectIntersections.size(); i++)
		{
			if (max < objectIntersections.at(i))
			{
				max = objectIntersections.at(i);
			}
		}

		// then starting from the maximum value find the minimum positive value
		if (max > 0)
		{
			// we only want positive intersections
			for (int i = 0; i < objectIntersections.size(); i++)
			{
				if (objectIntersections.at(i) > 0 && objectIntersections.at(i) <= max)
				{
					max = objectIntersections.at(i);
					indexOfMinimumValue = i;
				}
			}
			
			return indexOfMinimumValue;
		}
		else
		{
			// all the intersections are negative
			return -1;
		}
	}
}

int main(int argc, char const *argv[])
{
	cout << "rendering ..." << endl;

	int dpi    = 72;
	int width  = 640;
	int height = 480;
	int n      = width * height;

	double aspectRatio  = (double) width / (double) height;
	double ambientLight = 0.2;
	double accuracy     = 0.00001;

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

	Camera sceneCam(camPos, camDir, camRight, camDown);  

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


	double xAmount, yAmount;
	Vect camRayOrigin = sceneCam.getCameraPosition();
	Vect camRayDirection;

	// create array of scene objects and add them
	vector<Object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere));
	sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane));

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			thisOne = y * width + x;

			// start with no antialisng
			if (width > height)
			{
				 xAmount = ((x + 0.5) / width) * aspectRatio - (((width - height) / (double) height / 2));
				 yAmount = ((height - y) + 0.5) / height;
			}
			else if (width < height)
			{
				xAmount = (x + 0.5) / width;
				yAmount = (((height - y) + 0.5) / height) / aspectRatio - (((height - width) / (double) width) / 2);
			}
			else
			{
				xAmount = (x + 0.5) / width;
				yAmount = ((height - y) + 0.5) / height;
			}

			camRayDirection = camDir.VectAdd(camRight.VectMult(xAmount - 0.5)
									.VectAdd(camDown.VectMult(yAmount - 0.5)))
									.Normalize();
			Ray camRay(camRayOrigin, camRayDirection);

			vector<double> intersections;

			for (int index = 0; index < sceneObjects.size(); index++)
			{
				intersections.push_back(sceneObjects.at(index)->FindIntersection(camRay));
			}

			int indexOfWinningObject = WinningObjectIndex(intersections);
			
			if (indexOfWinningObject == -1)
			{
				// set background white
				pixels[thisOne].r = 0;
				pixels[thisOne].g = 0;
				pixels[thisOne].b = 0;
			}
			else
			{
				// index corresponds to object in scene
				if (intersections.at(indexOfWinningObject) > accuracy)
				{
					// determine the position and direction vectors at the point of intersection
					Vect intersectionPosition = camRayOrigin.VectAdd();

					Color thisColor = sceneObjects.at(indexOfWinningObject)->GetColor();
					pixels[thisOne].r = thisColor.GetColorRed();
					pixels[thisOne].g = thisColor.GetColorGreen();
					pixels[thisOne].b = thisColor.GetColorBlue();
				}
			}
		}
	}

	savebmp("first_scene.bmp", width, height, dpi, pixels);

	return 0;
}
