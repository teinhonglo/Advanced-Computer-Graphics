#include "Triangle.h"

Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3)
{
    position.set(x1, y1, z1);
    v1.set(x2, y2, z2);
    v2.set(x3, y3, z3);
    this->isHit = false;
}


Triangle::Triangle(float x1, float y1, float z1,
                   float x2, float y2, float z2,
                   float x3, float y3, float z3, Material m)
{
    position.set(x1, y1, z1);
    v1.set(x2, y2, z2);
    v2.set(x3, y3, z3);
    this->isHit = false;
    this->material = m;
}

bool Triangle::intersect(Ray ray, float &t0, float &t1)
{
    // base parameter
    vec3 ori = ray.getOri();
    vec3 dir = ray.getDir();
    vec3 pos = this->position;
    vec3 v1 = this->v1 - pos;
    vec3 v2 = this->v2 - pos;

    // compute t, u, v with Cramer's Rule
    vec3 T = ori  - pos;
    vec3 P = dir ^ v2;
    float det = P * v1;

    // scale u
    float u = P * T / det;
    if(u < 0 || u > 1) return false;

    // scale v
    vec3 Q = T ^ v1;
    float v = Q * dir / det;
    if(v < 0 || (u + v) > 1) return false;

    // ray direction
    float t  = Q * v2 / det;
    t0 = t;
    return true;
}

Material Triangle::getMaterial(){
    return this->material;
}
