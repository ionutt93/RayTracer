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
#include "Triangle.h"


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
				 Ray intersectionRay, 
				 vector<Object*> sceneObjects, 
		         int indexOfWinningObject, 
				 vector<Source*> lightSources, 
				 double accuracy, 
				 double ambientLight);


Color GetRefractedColor(Ray refractionRay,
						Vect intersectionPosition,
						vector<Object*> sceneObjects,
						int indexOfWinningObject,
						vector<Source*> lightSources,
						double accuracy,
						double ambientLight)
{
	double n1 = refractionRay.GetRefractiveIndex();
	double n2 = sceneObjects.at(indexOfWinningObject)->GetMaterial().GetRefractiveIndex();

	if (n1 == n2)
	{
		// ray inside the same material, means that it is going to be refracted outside, 
		n2 = 1.000293;
	}

	double n  = n1 / n2;

	Vect I = refractionRay.GetRayDirection();
	Vect N = sceneObjects.at(indexOfWinningObject)->GetNormalAt(intersectionPosition);

	double cosTheta1 = -N.DotProduct(I);
	// we need the normal pointing towards the side the ray is coming from
	if (cosTheta1 < 0) 
	{
		N = N.Negative();
		cosTheta1 = -N.DotProduct(I);
	}

	double sinTheta2 = n * sqrt(1 - (cosTheta1 * cosTheta1));
	if (sinTheta2 > 1) {
		//printf("Total refraction\n");
	}
	double cosTheta2 = sqrt(1 - (n * n) * (1 - (cosTheta1 * cosTheta1)));
	Vect refractionDirection = I.VectMult(n).VectAdd(N.VectMult(n * cosTheta1 - cosTheta2));


	Ray newRefractionRay(intersectionPosition, refractionDirection, n2, refractionRay.GetRemainingIntersections());
	// Ray newRefractionRay(intersectionPosition.VectAdd(refractionDirection.VectMult(0.1)), refractionDirection, n2, refractionRay.GetRemainingIntersections());

	// printf("Created refraction ray\n");
	vector<double> refractionIntersections;
	for (int refractionID = 0; refractionID < sceneObjects.size(); refractionID++)
	{
		refractionIntersections.push_back(sceneObjects.at(refractionID)->FindIntersection(newRefractionRay));
	}

	// printf("Got the refraction intersecions\n");
	// printf("%.2f %.2f %.2f \n", intersectionPosition.getVectX(), intersectionPosition.getVectY(), intersectionPosition.getVectZ());
	// printf("%d \n", indexOfWinningObject);
	// printf("%.6f %.6f\n", n1, n2);

	int indexOfWinningObjectWithRefraction = WinningObjectIndex(refractionIntersections);
	// printf("Got indexOfWinningObjectWithRefraction\n");
	if (indexOfWinningObjectWithRefraction != -1)
	{
		if (refractionIntersections.at(indexOfWinningObjectWithRefraction) > accuracy)
		{
			Vect refractionIntersectionPosition = intersectionPosition.VectAdd(refractionDirection.VectMult(refractionIntersections.at(indexOfWinningObjectWithRefraction)));
			// Vect objectNormal = sceneObjects.at(indexOfWinningObjectWithRefraction)->GetNormalAt(refractionIntersectionPosition);
			// Color objectColor = sceneObjects.at(indexOfWinningObjectWithRefraction)->GetColor();

			// printf("Got stuff for get color\n");
				// printf("Getting final color\n");
			Color intersectionColor = GetColorAt(refractionIntersectionPosition, newRefractionRay, sceneObjects, 
												indexOfWinningObjectWithRefraction, lightSources, accuracy, ambientLight);
			// printf("Got the color\n");
				// printf("Got final color\n");
			return intersectionColor;
			
		}
		else
		{
			//printf("Not a valid intersecion\n");
			return Color(0, 0, 0);
		}
	}
	else
	{
		//printf("No intersections\n");
		return Color(0, 0, 0);
	}
	

}

Color GetColorAt(Vect intersectionPosition, 
				 Ray intersectionRay, 
				 vector<Object*> sceneObjects, 
		         int indexOfWinningObject, 
				 vector<Source*> lightSources, 
				 double accuracy, 
				 double ambientLight)
{
	//printf("Getting them colors\n");
	Color winningObjectColor       = sceneObjects.at(indexOfWinningObject)->GetColor();
	Vect winningObjectNormal       = sceneObjects.at(indexOfWinningObject)->GetNormalAt(intersectionPosition);
	Material winningObjectMaterial = sceneObjects.at(indexOfWinningObject)->GetMaterial();
	Vect intersectingRayDirection  = intersectionRay.GetRayDirection();

	if (winningObjectMaterial.GetIsTextured() == true) {
		winningObjectColor = winningObjectMaterial.GetTextureColor(intersectionPosition);
	}

	intersectionRay.SubstractIntersection();

	//printf("Got stuff in get color at\n");

	if (winningObjectMaterial.GetIsTile() == true && intersectionRay.GetRemainingIntersections() > 0)
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

	//printf("Checked for tile\n");

	Color finalColor = winningObjectColor.ColorScalar(ambientLight);

	//printf("Got ambientLight\n");
	// refraction
	if (winningObjectMaterial.GetIsRefractive() == true && intersectionRay.GetRemainingIntersections() > 0)
	{
		// printf("Is refractive\n");
		// printf("Remaining: %d\n", intersectionRay.GetRemainingIntersections());

		// refracting = true;
		Color refractionColor = GetRefractedColor( intersectionRay,
												   intersectionPosition,
												   sceneObjects,
												   indexOfWinningObject,
												   lightSources,
												   accuracy,
												   ambientLight);

		// refracting = false;
		finalColor = refractionColor;
	}


	// printf("Checked for refraction\n");

	if (winningObjectMaterial.GetIsReflective() == true &&
		intersectionRay.GetRemainingIntersections() > 0)
	{
		//printf("Material is reflective\n");
		// reflection of objects with specular intersection
		double dot1  = -winningObjectNormal.DotProduct(intersectingRayDirection); 
		Vect scalar1 = winningObjectNormal.VectMult(dot1);
		// Vect add1 	 = scalar1.VectAdd(intersectingRayDirection);
		Vect scalar2 = scalar1.VectMult(2);
		Vect add2    = intersectingRayDirection.VectAdd(scalar2);
		Vect reflectionDirection = add2.Normalize();

		Ray reflectionRay(intersectionPosition, reflectionDirection, 1.000293, intersectionRay.GetRemainingIntersections());

		//printf("Computed the new ray\n");

		// determine what the ray intersects first
		vector<double> reflectionIntersections;
		for (int reflectionIndex = 0; reflectionIndex < sceneObjects.size(); reflectionIndex++)
		{
			reflectionIntersections.push_back(sceneObjects.at(reflectionIndex)->FindIntersection(reflectionRay));
		}

		//printf("Found intersections\n");

		int indexOfWinningObjectWithReflection = WinningObjectIndex(reflectionIntersections);
		//printf("Got winning object index %d\n", indexOfWinningObjectWithReflection);
		if (indexOfWinningObjectWithReflection != -1)
		{
			// reflection ray missed everything else
			if (reflectionIntersections.at(indexOfWinningObjectWithReflection) > accuracy)
			{
				// determine the position and direction at the point of intersection with the reflection ray
				// the ray only affects color if it is reflected of something
				Vect reflectionIntersectionPosition     = intersectionPosition.VectAdd(reflectionDirection.VectMult(reflectionIntersections.at(indexOfWinningObjectWithReflection)));
				Vect reflectionIntersectingRayDirection = reflectionDirection;

				//printf("Computed position and direction\n");
				//printf("Position: %f %f %f \n", reflectionIntersectionPosition.getVectX(), reflectionIntersectionPosition.getVectY(), reflectionIntersectionPosition.getVectZ());
				//printf("Direction: %f %f %f \n", reflectionIntersectingRayDirection.getVectX(), reflectionIntersectingRayDirection .getVectY(), reflectionIntersectingRayDirection .getVectZ());
				Color reflectionIntersectionColor = GetColorAt(reflectionIntersectionPosition, reflectionRay,
															   sceneObjects, indexOfWinningObjectWithReflection, lightSources, 
															   accuracy, ambientLight);
				//printf("Got reflected color\n");
				finalColor = finalColor.ColorAdd(reflectionIntersectionColor.ColorScalar(winningObjectMaterial.GetReflectiveness()));
			}
		}
	}

	// printf("Checked for reflection\n");
	// if(winningObjectMaterial.GetIsRefractive() == false)
	// {
	for (int lightIndex = 0; lightIndex < lightSources.size(); lightIndex++)
	{
		Vect lightDirection = lightSources.at(lightIndex)->GetLightPosition().VectAdd(intersectionPosition.Negative()).Normalize();
		double cosineAngle = winningObjectNormal.DotProduct(lightDirection);


		if (cosineAngle > 0.0)
		{
			// test for shadows
			bool shadowed = false;

			Vect distanceToLight = lightSources.at(lightIndex)->GetLightPosition().VectAdd(intersectionPosition.Negative());
			double distanceToLightMagnitude = distanceToLight.Magnitude();

			// printf("%f \n", distanceToLightMagnitude);

			// Ray shadowRay(intersectionPosition, lightSources.at(lightIndex)->GetLightPosition()
			// 												.VectAdd(intersectionPosition.Negative()).Normalize());

			Ray shadowRay(intersectionPosition, lightDirection);

			vector<double> secondaryIntersections;
			for (int objectIndex = 0; objectIndex < sceneObjects.size() && shadowed == false; objectIndex++)
			{
				secondaryIntersections.push_back(sceneObjects.at(objectIndex)->FindIntersection(shadowRay)); 
				// Triangle* t = dynamic_cast<Triangle*>(sceneObjects.at(objectIndex));
				// if (t != NULL && indexOfWinningObject == 0 && secondaryIntersections.at(objectIndex) != -1) {
				// 	printf("intersections : %f\n", secondaryIntersections.at(objectIndex));
				// }
			}

			// printf("size %d\n", sceneObjects.size());
			for (int c = 0; c < sceneObjects.size(); c++)
			{
				if (secondaryIntersections.at(c) > accuracy)
				{
					// printf("%f \n", secondaryIntersections.at(c));
					if (secondaryIntersections.at(c) <= distanceToLightMagnitude)
					{
						shadowed = true;
						break;
					}
				}
			}

			if (shadowed == false)
			{
				// if (winningObjectMaterial.GetIsRefractive() == false) {
					finalColor = finalColor.ColorAdd(winningObjectColor.ColorMultiply(lightSources.at(lightIndex)->GetLightColor()).ColorScalar(cosineAngle));
				// }

				// printf("Computing specular color\n");
				if (winningObjectMaterial.GetIsReflective() == true)
				{
					//special [0-1] shineness

					double dot1  = -winningObjectNormal.DotProduct(intersectingRayDirection); 
					Vect scalar1 = winningObjectNormal.VectMult(dot1);
					// Vect add1 	 = scalar1.VectAdd(intersectingRayDirection);
					Vect scalar2 = scalar1.VectMult(2);
					Vect add2    = intersectingRayDirection.VectAdd(scalar2);
					Vect reflectionDirection = add2.Normalize();

					double specular = reflectionDirection.DotProduct(lightDirection);
					if (specular > 0)
					{
						// printf("Specular > 0 %.10f\n", specular);
						specular = pow(specular, 10.0);
						// printf("After specular %.10f\n", specular);
						finalColor = finalColor.ColorAdd(lightSources.at(lightIndex)->GetLightColor()
														.ColorScalar(specular * winningObjectMaterial.GetReflectiveness()));
					}
				}
			}
		}
	}	
	// }
	

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
 
	// Vect camPos(1.5, 0.75, -2);
	Vect camPos(0, 1.5, -6);
	Vect lookAt(0, 0, 0);
	Vect diffBtw(camPos.getVectX() - lookAt.getVectX(), camPos.getVectY() - lookAt.getVectY(), camPos.getVectZ() - lookAt.getVectZ());
	
	Vect camDir   = diffBtw.Negative().Normalize();
	Vect camRight = Y.CrossProduct(camDir).Normalize();
	Vect camDown  = camRight.CrossProduct(camDir).Normalize();

	Camera sceneCam(camPos, camDir, camRight, camDown);  

	Color whiteLight(1.0, 1.0, 1.0);
	Color tileFloor(1.0, 1.0, 1.0);
	Color prettyGreen(0.5, 1.0, 0.5); 
	Color gray(0.5, 0.5, 0.5);
	Color black(0.0, 0.0, 0.0);
	Color maroon(0.5, 0.25, 0.25);

	Material tile(true, true, false, false, 0.4, 0.0);
	Material matt(false, false, false, false, 0.0, 0.0);
	Material shiny(false, true, false, false, 0.3, 0.0);
	Material glass(false, true, true, false, 0.1, 1.46);
	Material textured(false, false, false, true, 0.0, 0.0);
	textured.SetTexture(256, 256, "texture2.bmp");

	Vect lightPosition(0, 10, -10);
	//Vect lightPosition2(10, 10, 3);

	Light sceneLight(lightPosition, whiteLight);
	//Light sceneLight2(lightPosition2, whiteLight);

	// scene objects
	Sphere sceneSphere(O.VectAdd(Vect(-0.5, 0.3, -0.5)), 0.3, Color(0.2, 0.4, 0.1), matt);
	Sphere sceneSphere2(X, 0.5, Color(0.8, 0.0, 0.0), matt);
	Sphere sceneSphere3(Y, 0.5, Color(0.0, 0.8, 0.0), matt);
	Sphere sceneSphere4(Z, 0.5, Color(0.0, 0.0, 0.8), matt);
	Sphere sceneSphere5(Vect(2, 1, -0.5), 0.3, maroon, matt);
	Sphere sceneSphere6(Vect(1, 1.1, 1.5), 0.7, gray, matt);
	Sphere sceneSphere7(Vect(-2, 1.3, 0.0), 0.9, Color(0.2, 0.1, 0.5), matt);

	Plane scenePlane(Y, -3, tileFloor, matt);
	Plane wallPlane(X, -7, gray, matt);
	Plane wallPlane2(Z, 7, gray, matt);
	Plane wallPlane3(X, 7, gray, matt);

	Triangle sceneTriangle(O, Vect(0, 2, 0), Vect(2, 0, 0), prettyGreen, matt);
	Triangle sceneTriangle2(O, Vect(0, -2, 0), Vect(-2, 0, 0), prettyGreen, matt);
	Triangle sceneTriangle3(Vect(2, -2, 0), Vect(0, -2, 0), Vect(2, 0, 0), Color(0.2, 0.3, 0.4), matt);
	Triangle sceneTriangle4(Vect(-2, 2, 0), Vect(0, 2, 0), Vect(-2, 0, 0), Color(0.2, 0.3, 0.4), matt);

	// Triangle sceneTriangle(X.VectAdd(Vect(0, 0, 0)), Y.VectAdd(Vect(0, 0, 0)), Z.VectAdd(Vect(0, 0, 0)), prettyGreen, shiny);

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
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere5));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere6));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere7));
	sceneObjects.push_back(dynamic_cast<Object*>(&wallPlane));
	sceneObjects.push_back(dynamic_cast<Object*>(&wallPlane2));
	sceneObjects.push_back(dynamic_cast<Object*>(&wallPlane3));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneTriangle));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneTriangle2));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneTriangle3));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneTriangle4));

	vector<Source*> lightSources;
	lightSources.push_back(dynamic_cast<Source*>(&sceneLight));
	//lightSources.push_back(dynamic_cast<Source*>(&sceneLight2));

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
							// Vect intersectingRayDirection = camRayDirection;

							Color intersectionColor = GetColorAt(intersectionPosition, camRay, sceneObjects, 
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
