#include "Material.h"

void Material::setMaterial(float r, float g, float b, float Ka, float Kd, float Ks, float exp, float Reflect, float Refract, float Nr)
{
    this->color.R = r * 255;
	this->color.G = g * 255;
	this->color.B = b * 255;
	this->Ka = Ka;
	this->Kd = Kd;
	this->Ks = Ks;
	this->exp = exp;
	this->Reflect = Reflect;
	this->Refract = Refract;
	this->Nr = Nr;

}
