#include "Sphere.h"

Sphere::Sphere(float oX, float oY, float oZ, float radius)
{
    center.set(oX, oY, oZ);
    this->radius = radius;
    this->isHit = false;
}

void Sphere::setCenter(float oX, float oY, float oZ)
{
    this->center.set(oX, oY, oZ);
}

bool Sphere::intersect(Ray ray, float &t0, float &t1)
{
    vec3 ori = ray.getOri();
    vec3 dir = ray.getDir();
    float t = (this->center * dir - ori * dir ) / (dir * dir);
    vec3 nearest_point = ray.getPoint(t);
    float distance = (nearest_point - this->center).length();
    if(distance <= this->radius){
        return true;
    }else{
        return false;
    }
}
