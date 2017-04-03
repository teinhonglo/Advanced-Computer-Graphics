#include <iostream>
#include"algebra3/algebra3.h"
#ifndef MATERIAL_H
#define MATERIAL_H

struct Color{
	float R;
	float G;
	float B;
};

struct Material
{
	Color color;
    float Ka, Kd, Ks, exp, Reflect, Refract, Nr;
    void setMaterial(float r, float g, float b, float Ka, float Kd, float Ks, float exp, float Reflect, float Refract, float Nr);
};

#endif
