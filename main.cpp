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
#include "Source.h"
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


Color GetColorAt(Vect intersectionPosition, 
				 Vect intersectingRayDirection, 
				 vector<Object*> sceneObjects, 
		         int indexOfWinningObject, 
				 vector<Source*> lightSources, 
				 double accuracy, 
				 double ambientLight)
{
	Color winningObjectColor = sceneObjects.at(indexOfWinningObject)->GetColor();
	Vect winningObjectNormal = sceneObjects.at(indexOfWinningObject)->GetNormalAt(intersectionPosition);

	if (winningObjectColor.GetColorSpecial() == 2)
	{
		 // checker pattern
		int square = (int) floor(intersectionPosition.getVectX()) + (int) floor(intersectionPosition.getVectZ());
		if (square % 2 == 0)
		{
			// black tile
			winningObjectColor.SetColorRed(0);
			winningObjectColor.SetColorGreen(0);
			winningObjectColor.SetColorBlue(0);
		}
		else
		{
			// white tile
			winningObjectColor.SetColorRed(1);
			winningObjectColor.SetColorGreen(1);
			winningObjectColor.SetColorBlue(1);
		}
	}

	Color finalColor = winningObjectColor.ColorScalar(ambientLight);

	if (winningObjectColor.GetColorSpecial() > 0 && winningObjectColor.GetColorSpecial() <= 1)
	{
		// reflection of objects with speculat intensityon
		double dot1  = winningObjectNormal.DotProduct(intersectingRayDirection.Negative()); 
		Vect scalar1 = winningObjectNormal.VectMult(dot1);
		Vect add1 	 = scalar1.VectAdd(intersectingRayDirection);
		Vect scalar2 = add1.VectMult(2);
		Vect add2    = intersectingRayDirection.Negative().VectAdd(scalar2);
		Vect reflectionDirection = add2.Normalize();

		Ray reflectionRay(intersectionPosition, reflectionDirection);

		// determine what the ray intersects first
		vector<double> reflectionIntersections;
		for (int reflectionIndex = 0; reflectionIndex < sceneObjects.size(); reflectionIndex++)
		{
			reflectionIntersections.push_back(sceneObjects.at(reflectionIndex)->FindIntersection(reflectionRay));
		}

		int indexOfWinningObjectWithReflection = WinningObjectIndex(reflectionIntersections);
		if (indexOfWinningObjectWithReflection != -1)
		{
			// reflection ray missed everything else
			if (reflectionIntersections.at(indexOfWinningObjectWithReflection) > accuracy)
			{
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects color if it is reflected of something
				Vect reflectionIntersectionPosition     = intersectionPosition.VectAdd(reflectionDirection.VectMult(reflectionIntersections.at(indexOfWinningObjectWithReflection)));
				Vect reflectionIntersectingRayDirection = reflectionDirection;

				Color reflectionIntersectionColor = GetColorAt(reflectionIntersectionPosition, reflectionIntersectingRayDirection,
															   sceneObjects, indexOfWinningObjectWithReflection, lightSources, 
															   accuracy, ambientLight);
				finalColor = finalColor.ColorAdd(reflectionIntersectionColor.ColorScalar(winningObjectColor.GetColorSpecial()));
			}
		}
	}	

	for (int lightIndex = 0; lightIndex < lightSources.size(); lightIndex++)
	{
		Vect lightDirection = lightSources.at(lightIndex)->GetLightPosition().VectAdd(intersectionPosition.Negative()).Normalize();

		float cosineAngle = winningObjectNormal.DotProduct(lightDirection);
		if (cosineAngle > 0)
		{
			// test for shadows
			bool shadowed = false;

			Vect distanceToLight = lightSources.at(lightIndex)->GetLightPosition()
												.VectAdd(intersectionPosition.Negative()).Normalize();
			float distanceToLightMagnitude = distanceToLight.Magnitude();

			Ray shadowRay(intersectionPosition, lightSources.at(lightIndex)->GetLightPosition()
															.VectAdd(intersectionPosition.Negative()).Normalize());

			vector<double> secondaryIntersections;
			for (int objectIndex = 0; objectIndex < sceneObjects.size() && shadowed == false; objectIndex++)
			{
				secondaryIntersections.push_back(sceneObjects.at(objectIndex)->FindIntersection(shadowRay)); 
			}

			
			for (int c = 0; c < secondaryIntersections.size(); c++)
			{
				if (secondaryIntersections.at(c) > accuracy)
				{
					if (secondaryIntersections.at(c) <= distanceToLightMagnitude)
					{
						shadowed = true;
					}
					break;
				}
			}

			if (shadowed == false)
			{
				finalColor = finalColor.ColorAdd(winningObjectColor.ColorMultiply(lightSources.at(lightIndex)->GetLightColor()).ColorScalar(cosineAngle));

				if (winningObjectColor.GetColorSpecial() > 0 && winningObjectColor.GetColorSpecial() <= 1)
				{
					//special [0-1] shineness
					double dot1  = winningObjectNormal.DotProduct(intersectingRayDirection.Negative());
					Vect scalar1 = winningObjectNormal.VectMult(dot1);
					Vect add1	 = scalar1.VectAdd(intersectingRayDirection);
					Vect scalar2 = add1.VectMult(2);
					Vect add2    = intersectingRayDirection.Negative().VectAdd(scalar2);
					Vect reflectionDirection = add2.Normalize();

					double specular = reflectionDirection.DotProduct(lightDirection);
					if (specular > 0)
					{
						specular = pow(specular, 10);
						finalColor.ColorAdd(lightSources.at(lightIndex)->GetLightColor()
														.ColorScalar(specular * winningObjectColor.GetColorSpecial()));
					}
				}
			}
		}
	}

	return finalColor.Clip();
}


int main(int argc, char const *argv[])
{
	cout << "rendering ..." << endl;

	int dpi    = 72;
	int width  = 640;
	int height = 480;
	int n      = width * height;

	double aspectRatio  = (double) width / (double) height;
	double ambientLight = 0.3;
	double accuracy     = 0.000001;
	int aadepth 		= 1;

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
	Color tileFloor(1.0, 1.0, 1.0, 2.0);
	Color prettyGreen(0.5, 1.0, 0.5, 0.2); 
	Color gray(0.5, 0.5, 0.5, 0.2);
	Color black(0.0, 0.0, 0.0, 0.0);
	Color maroon(0.5, 0.25, 0.25, 0.1);


	Vect lightPosition(-5, 1, -2);
	Vect lightPosition2(10, 1, 3);

	Light sceneLight(lightPosition, whiteLight);
	Light sceneLight2(lightPosition2, whiteLight);

	// scene objects
	Sphere sceneSphere(O, 1, prettyGreen);
	Sphere sceneSphere2(O.VectAdd(Vect(1.5, 0, 0)), 1, Color(0.8, 0, 0, 0.3));
	Sphere sceneSphere3(O.VectAdd(Vect(0, 1.5, 0)), 1.2, Color(0, 0.8, 0, 0.3));
	Sphere sceneSphere4(O.VectAdd(Vect(0, 0, -1.5)), 0.8, Color(0, 0, 0.8, 0.3));
	Plane scenePlane(Y, -1, gray);
	Plane wallPlane(X, -7, gray);
	Plane wallPlane2(Z, 7, gray);


	double xAmount, yAmount;
	Vect camRayOrigin = sceneCam.getCameraPosition();
	Vect camRayDirection;

	// create array of scene objects and add them
	vector<Object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere2));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere3));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere4));
	sceneObjects.push_back(dynamic_cast<Object*>(&wallPlane));
	sceneObjects.push_back(dynamic_cast<Object*>(&wallPlane2));

	vector<Source*> lightSources;
	lightSources.push_back(dynamic_cast<Source*>(&sceneLight));
	lightSources.push_back(dynamic_cast<Source*>(&sceneLight2));

	int aaIndex;

	double tempRed[aadepth * aadepth];
	double tempGreen[aadepth * aadepth];
	double tempBlue[aadepth * aadepth];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			thisOne = y * width + x;
			// start with a blank pixel

			for (int aax = 0; aax < aadepth; aax++)
			{
				for (int aay = 0; aay < aadepth; aay++)
				{
					aaIndex = aay * aadepth + aax;

					// create the ray from the camera to this pixel
					if (aadepth == 1)
					{
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
					}
					else
					{
						// anti-aliasing
						double aavar = (double) aax / ((double) aadepth - 1);
						if (width > height)
						{
							xAmount = ((x + aavar) / width) * aspectRatio - (((width - height) / (double) height / 2));
							yAmount = ((height - y) + aavar) / height;
						}
						else if (width < height)
						{
							xAmount = (x + aavar) / width;
							yAmount = (((height - y) + aavar) / height) / aspectRatio - (((height - width) / (double) width) / 2);
						}
						else
						{
							xAmount = (x + aavar) / width;
							yAmount = ((height - y) + aavar) / height;
						}
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
						tempRed[aaIndex]   = 0;
						tempGreen[aaIndex] = 0;
						tempBlue[aaIndex]  = 0;
					}
					else
					{
						// index corresponds to object in scene
						if (intersections.at(indexOfWinningObject) > accuracy)
						{
							// determine the position and direction vectors at the point of intersection
							Vect intersectionPosition     = camRayOrigin.VectAdd(camRayDirection.VectMult(intersections.at(indexOfWinningObject)));
							Vect intersectingRayDirection = camRayDirection;

							Color intersectionColor = GetColorAt(intersectionPosition, intersectingRayDirection, sceneObjects, 
																 indexOfWinningObject, lightSources, accuracy, ambientLight);

							tempRed[aaIndex]   = intersectionColor.GetColorRed();
							tempGreen[aaIndex] = intersectionColor.GetColorGreen();
							tempBlue[aaIndex]  = intersectionColor.GetColorBlue();
						}
					}
				}
			}

			// average the pixel value
			double totalRed   = 0;
			double totalGreen = 0;
			double totalBlue  = 0;

			for (int iRed = 0; iRed < aadepth * aadepth; iRed++)
			{
				totalRed += tempRed[iRed];
			}

			for (int iGreen = 0; iGreen < aadepth * aadepth; iGreen++)
			{
				totalGreen += tempGreen[iGreen];	
			}

			for (int iBlue = 0; iBlue < aadepth * aadepth; iBlue++)
			{
				totalBlue += tempBlue[iBlue];
			}

			double avgRed   = totalRed   / (double) (aadepth * aadepth);
			double avgGreen = totalGreen / (double) (aadepth * aadepth);
			double avgBlue  = totalBlue  / (double) (aadepth * aadepth);

			pixels[thisOne].r = avgRed;
			pixels[thisOne].g = avgGreen;
			pixels[thisOne].b = avgBlue;
		}
	}

	savebmp("first_scene.bmp", width, height, dpi, pixels);

	delete pixels;
	return 0;
}
