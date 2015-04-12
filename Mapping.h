#ifndef MAPPING_H
#define MAPPING_H

#include "Vect.h"

class Mapping {
public:
	Mapping();
	
	virtual void GetTextureCoord(Vect hitPoint, int hres, int vres, int& x, int& y) {
		// code here
	}
};

Mapping::Mapping() {
	// nothing to see here	
}

#endif // MAPPING_H
