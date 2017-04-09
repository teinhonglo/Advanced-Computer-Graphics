#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <iostream>
#include"algebra3/algebra3.h"
#include "Ray.h"
#include "Material.h"

class Object3D
{
public:
	virtual bool intersect(Ray ray, float &t0, float &t1) = 0;
    virtual vec3 getNormal(vec3 intersect_p) = 0;
    virtual Material getMaterial() = 0;
};

#endif
