#include <iostream>
#include"algebra3/algebra3.h"
#include "Material.h"
#include "Ray.h"
#include <math.h>       /* pow */
#ifndef SPHERE_H
#define SPHERE_H

class Sphere
{
private:
    vec3 center;
    float radius;
    bool isHit;
    Material material;
public :
    Sphere(float oX, float oY, float oZ, float radius);
    Sphere(float oX, float oY, float oZ, float radius, Material m);
    void setCenter(float oX, float oY, float oZ);
    vec3 getCenter();
    void setRadius(float r);
    vec3 getRadius();
    bool intersect(Ray ray, float &t0, float &t1);
    Material getMaterial();
};

#endif
