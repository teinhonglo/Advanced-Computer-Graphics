#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include"../algebra3/algebra3.h"
#include <assert.h>

using namespace std;



struct Pixel {
	unsigned char R, G, B;  // Blue, Green, Red
};

class ColorImage {
	Pixel *pPixel;
	int xRes, yRes;
public:
	ColorImage();
	~ColorImage();
	void init(int xSize, int ySize);
	void clear(Pixel background);
	Pixel readPixel(int x, int y);
	void writePixel(int x, int y, Pixel p);
	void outputPPM(char *filename);
};

ColorImage::ColorImage()
{
	pPixel = 0;
}

ColorImage::~ColorImage()
{
	if (pPixel) delete[] pPixel;
	pPixel = 0;
}

void ColorImage::init(int xSize, int ySize)
{
	Pixel p = {0,0,0};
	xRes = xSize;
	yRes = ySize;
	pPixel = new Pixel[xSize*ySize];
	clear(p);
}

void ColorImage::clear(Pixel background)
{
	int i;

	if (! pPixel) return;
	for (i=0; i<xRes*yRes; i++) pPixel[i] = background;
}

Pixel ColorImage::readPixel(int x, int y)
{
	assert(pPixel); // die if image not initialized
	return pPixel[x + y*yRes];
}

void ColorImage::writePixel(int x, int y, Pixel p)
{
	assert(pPixel); // die if image not initialized
	pPixel[x + y*yRes] = p;
}

void ColorImage::outputPPM(char *filename)
{
    FILE *outFile = fopen(filename, "wb");

	assert(outFile); // die if file can't be opened

	fprintf(outFile, "P6 %d %d 255\n", xRes, yRes);
	fwrite(pPixel, 1, 3*xRes*yRes, outFile );

	fclose(outFile);
}

class Ray
{
private:
    vec3 origin;
    vec3 direction;
    float t = INFINITY;
public:
    Ray(vec3 ori, vec3 dir)
    {
        origin = ori;
        direction = dir;
    }
    vec3 getOri(){
        return origin;
    }
    vec3 getDir(){
        return direction;
    }
};
class Sphere
{
private:
    vec3 center;
    float radius;
    bool isHit;
public :
    Sphere(float oX, float oY, float oZ, float radius)
    {
        center.set(oX, oY, oZ);
        this->radius = radius;
        this->isHit = false;
    }

    void setCenter(float oX, float oY, float oZ)
    {
        this->center.set(oX, oY, oZ);
    }
    bool intersect(Ray ray, float &t0, float &t1)
    {
        vec3 ori = ray.getOri();
        vec3 dir = ray.getDir();
        vec3 l = this->center - ori;
        float tca = l * ori;
        if (tca < 0) return false;
        float d2 = l * l - tca * tca;
        if (d2 > this->radius) return false;
        float thc = sqrt(radius - d2);
        t0 = tca - thc;
        t1 = tca + thc;

        return true;
    }
};
class Triangle
{
private:
    vec3 position;
    vec3 s1;
    vec3 s2;
    bool isHit;
public:
    Triangle(float x1, float y1, float z1,
             float x2, float y2, float z2,
             float x3, float y3, float z3)
    {
        position.set(x1, y1, z1);
        s1.set(x2, y2, z2);
        s2.set(x3, y3, z3);
        this->isHit = false;
    }
    bool intersect()
    {
        return isHit;
    }
};


vector<string> split(string str, string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

int main()
{
    ifstream file( "hw1_input.txt");
    string pattern = " ";
    string line;
    vector<string> info;
    vec3 Eye;
    vec3 View_Direction;
    float FieldOfView;
    int width, height;
    vector<Sphere> Spheres_vector;
    vector<Triangle> Triangles_vector;

    // Read information
    while(getline(file, line))
    {
        info = split(line, pattern);
        if( info[0] == "E")
        {
            float ex = atof(info[1].c_str());
            float ey = atof(info[2].c_str());
            float ez = atof(info[3].c_str());
            Eye.set(ex, ey, ez);
        }
        else if(info[0] == "V")
        {
            float vx = atof(info[1].c_str());
            float vy = atof(info[2].c_str());
            float vz = atof(info[3].c_str());
            View_Direction.set(vx, vy, vz);
        }
        else if(info[0] == "F")
        {
            FieldOfView = atof(info[1].c_str());
        }
        else if(info[0] == "R")
        {
            width = atoi(info[1].c_str());
            height = atoi(info[2].c_str());
        }
        else if(info[0] == "S")
        {
            float oX = atoi(info[1].c_str());
            float oY = atoi(info[2].c_str());
            float oZ = atoi(info[3].c_str());
            float radius = atoi(info[4].c_str());
            Sphere s(oX, oY, oZ, radius);
            Spheres_vector.push_back(s);
        }
        else if(info[0] == "T")
        {
            float x1 = atoi(info[1].c_str());
            float y1 = atoi(info[2].c_str());
            float z1 = atoi(info[3].c_str());
            float x2 = atoi(info[4].c_str());
            float y2 = atoi(info[5].c_str());
            float z2 = atoi(info[6].c_str());
            float x3 = atoi(info[7].c_str());
            float y3 = atoi(info[8].c_str());
            float z3 = atoi(info[9].c_str());
            Triangle t(x1, y1, z1, x2, y2, z2, x3, y3, z3);
            Triangles_vector.push_back(t);
        }
        else
        {
            cout << "not mapping";
        }
    }

    vec3 screen [width][height];
    float invWidth = 1 / float(width), invHeight = 1 / float(height);
    float fov = FieldOfView, aspectratio = width / float(height);
    float angle = tan(M_PI * 0.5 * fov / 180.);

    int pixel [width][height];
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            float _x = (2 * ((_x + 0.5) * invWidth) - 1) * angle * aspectratio;
            float _y = (1 - 2 * ((_y + 0.5) * invHeight)) * angle;
            vec3 raydir = vec3(_x, _y, 0) + View_Direction;
            raydir.normalize();
            Ray ray(Eye, raydir);
            float t0, t1;
            for (int sp_idx = 0;  sp_idx < Spheres_vector.size() ; sp_idx++)
            {
                pixel[x][y] = (Spheres_vector[sp_idx].intersect(ray, t0, t1))? 0:255;
            }
            for (int tri_idx = 0;  tri_idx < Triangles_vector.size() ; tri_idx++)
            {
                //pixel[x][y] = (Triangles_vector[tri_idx].intersect())? 255:0;
            }
        }
    }
    ColorImage image;
    int x, y;
	Pixel p={0,0,0};

	image.init(width, height);
	for (y=0; y<height; y++) {
		for (x=0; x<width; x++) {
			p.R = pixel[x][y];
			image.writePixel(x, y, p);
		}
	}

	image.outputPPM("reds.ppm");

    /**
    For every pixel
        Construct a ray from the eye
        For every object in the scene
            Find intersection with ray
        Keep if closest
    **/
    /**
    Sphere s1;
    s1.setCenter(1.0, 2.0, 3.0);
    vec3 v3;
    vec3 v4(1, 2, 3);
    v3 = v3 + v4;
    v3.printCoor();
    system("pause");
    **/
    return 0;
}
