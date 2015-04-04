#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
private:
	bool isTile;
	bool isReflective;
	bool isRefractive;

	double reflectiveness;
	double refractiveIndex;
public:
	Material();

	Material(bool, bool, bool, double, double);

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

	double GetReflectiveness()
	{
		return reflectiveness;
	}

	double GetRefractiveIndex()
	{
		return refractiveIndex;
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
	isRefractive    = false ;
	
	reflectiveness  = 0.0;
	refractiveIndex = 0.0;
}

Material::Material(bool t, bool rl, bool rf, double rls, double rfs)
{
	isTile          = t;
	isReflective    = rl;
	isRefractive    = rf ;
	
	reflectiveness  = rls;
	refractiveIndex = rfs;
}

#endif // MATERIAL_H
