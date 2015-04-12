#ifndef SPHERICAL_MAPPING_H
#define SPHERICAL_MAPPING_H

#include "Mapping.h"

#define TWO_PI 6.2831853071
#define INV_PI 0.3183098861
#define INV_TWO_PI 0.1591549430

class SphericalMapping : public Mapping {
public:
	SphericalMapping();
	
	virtual void GetTextureCoord(Vect hitPoint, int hres, int vres, int& x, int& y) {
		float theta = acos(hitPoint.getVectY());
		float phi   = atan2(hitPoint.getVectX(), hitPoint.getVectZ());

		if (phi < 0.0) {
			phi += TWO_PI;
		}

		float u = phi * INV_TWO_PI;
		float v = 1 - theta * INV_PI;

		y = (int) ((hres - 1) * u);
		x = (int) ((vres - 1) * v);
	}
};

SphericalMapping::SphericalMapping() {
	// nothing to see here	
}


#endif // SPHERICAL_MAPPING_H
