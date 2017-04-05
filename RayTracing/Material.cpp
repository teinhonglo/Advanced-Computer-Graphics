#include "Material.h"

void Color::setColor(float r, float g, float b){
	this->R = r;
	this->G = g;
	this->B = b;
}

void Material::setMaterial(float r, float g, float b, float Ka, float Kd, float Ks, float exp, float Reflect, float Refract, float Nr)
{
    this->color.setColor(r * 255, g * 255, b * 255);
	this->Ka = Ka;
	this->Kd = Kd;
	this->Ks = Ks;
	this->exp = exp;
	this->Reflect = Reflect;
	this->Refract = Refract;
	this->Nr = Nr;

}
