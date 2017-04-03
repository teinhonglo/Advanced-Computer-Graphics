#include "Material.h"

void Material::setMaterial(float r, float g, float b, float Ka, float Kd, float Ks, float exp, float Reflect, float Refract, float Nr)
{
    this->color.R = r;
	this->color.G = g;
	this->color.B = b;
	this->Ka = Ka;
	this->Kd = Kd;
	this->Ks = Ks;
	this->exp = exp;
	this->Reflect = Reflect;
	this->Refract = Refract;
	this->Nr = Nr;

}
