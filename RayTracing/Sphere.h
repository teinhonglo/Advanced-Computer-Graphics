#include <iostream>
#include"algebra3/algebra3.h"
#include "Ray.h"
#ifndef SPHERE_H
#define SPHERE_H

class Sphere
{
private:
    vec3 center;
    float radius;
    bool isHit;
public :
    Sphere(float oX, float oY, float oZ, float radius);
    void setCenter(float oX, float oY, float oZ);
    bool intersect(Ray ray, float &t0, float &t1);
};

#endif
