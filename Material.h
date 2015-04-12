#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"
#include "Vect.h"

class Material {
private:
	bool isTile;
	bool isReflective;
	bool isRefractive;
	bool isTextured;

	double reflectiveness;
	double refractiveIndex;
	Texture* texture;
public:
	Material();

	Material(bool, bool, bool, bool, double, double);

	// getter functions
	bool GetIsTile()
	{
		return isTile;
	}

	bool GetIsReflective()
	{
		return isReflective;
	}

	bool GetIsRefractive()
	{
		return isRefractive;
	}

	bool GetIsTextured()
	{
		return isTextured;
	}

	double GetReflectiveness()
	{
		return reflectiveness;
	}

	double GetRefractiveIndex()
	{
		return refractiveIndex;
	}

	virtual Color GetTextureColor(Vect hitPoint)
	{
		return texture->GetTextureColorAt(hitPoint);
	}

	// setter functions
	void SetIsTile(bool c)
	{
		isTile = c;
	}

	void SetIsReflective(bool c)
	{
		isReflective = c;
	}

	void SetIsRefractive(bool c)
	{
		isRefractive = c;
	}

	void SetReflectiveness(double r)
	{
		reflectiveness = r;
	}

	void SetRefractiveIndex(double r)
	{
		refractiveIndex = r;
	}
};

Material::Material()
{
	isTile          = false;
	isReflective    = false;
	isRefractive    = false;
	isTextured 		= false;
	
	reflectiveness  = 0.0;
	refractiveIndex = 0.0;
	texture         = new Texture(100, 100);
}

Material::Material(bool t, bool rl, bool rf, bool it, double rls, double rfs)
{
	isTile          = t;
	isReflective    = rl;
	isRefractive    = rf ;
	isTextured 		= it;
	
	reflectiveness  = rls;
	refractiveIndex = rfs;
	texture         = new Texture(100, 100);
}

#endif // MATERIAL_H
