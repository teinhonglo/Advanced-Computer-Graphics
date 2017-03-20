#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include"../algebra3/algebra3.h"
#include <assert.h>

using namespace std;

struct Pixel
{
    unsigned char R, G, B;  // Blue, Green, Red
};

class ColorImage
{
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
    vec3 getOri()
    {
        return origin;
    }
    vec3 getDir()
    {
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
        center.printCoor();
        cout << radius;
        this->radius = radius;
        this->isHit = false;
    }

    void setCenter(float oX, float oY, float oZ)
    {
        this->center.set(oX, oY, oZ);
    }
    bool intersect(Ray ray, float &t0, float &t1)
    {
        vec3 ori = ray.getOri() - this->center;
        vec3 dir = ray.getDir();
        vec3 A = prod(dir, dir);                    // A = D * D
        vec3 B = 2 * prod(ori, dir);                // B = 2 * S * D
        vec3 C = prod(ori, ori) - (radius * radius);  // C = S * S - R * R
        vec3 discriminant  = prod(B, B) - 4 * prod(A, C);
        if(discriminant >= 0)
        {
            return true;
        }else{
            false;
        }

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
        s1.set(x2 - x1, y2 - y1, z2 - z1);
        s2.set(x3 - x1, y3 - y1, z3 - z1);
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
    int distance = 1;
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

            float oX = atof(info[1].c_str());
            float oY = atof(info[2].c_str());
            float oZ = atof(info[3].c_str());
            float radius = atof(info[4].c_str());
            Sphere s(oX, oY, oZ, radius);
            Spheres_vector.push_back(s);
        }
        else if(info[0] == "T")
        {
            float x1 = atof(info[1].c_str());
            float y1 = atof(info[2].c_str());
            float z1 = atof(info[3].c_str());
            float x2 = atof(info[4].c_str());
            float y2 = atof(info[5].c_str());
            float z2 = atof(info[6].c_str());
            float x3 = atof(info[7].c_str());
            float y3 = atof(info[8].c_str());
            float z3 = atof(info[9].c_str());
            Triangle t(x1, y1, z1, x2, y2, z2, x3, y3, z3);
            Triangles_vector.push_back(t);
        }
        else
        {
            cout << "not mapping";
        }
    }

    vec3 lookAtPoint = Eye + distance * View_Direction;
    vec3 up (0, 1, 0);
    vec3 U = View_Direction ^ up;
    vec3 V = U ^ View_Direction;

    int screen [width][height];
    float halfWidth = float(width) / 2;
    float halfHeight = float(height) / 2;
    float aspectRatio = width / float(height);
    float viewPlaneHalfWidth = tan(M_PI * 0.5 * FieldOfView / 180.);
    float viewPlaneHalfHeight = aspectRatio * viewPlaneHalfWidth;
    vec3 viewPlaneTopLeftPoint = lookAtPoint - prod(V, viewPlaneHalfHeight) + prod(U, viewPlaneHalfWidth);
    vec3 xIncVector = (U * 2 * halfWidth) / width;
    vec3 yIncVector = (V * 2 * halfHeight) / height;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            vec3 viewPlanePoint = viewPlaneTopLeftPoint + i*xIncVector + j*yIncVector;
            vec3 castRay = viewPlanePoint - Eye;
            Ray ray(Eye,castRay);
            float t0, t1;
            for (int sp_idx = 0;  sp_idx < Spheres_vector.size() ; sp_idx++)
            {
                screen[i][j] = (Spheres_vector[sp_idx].intersect(ray, t0, t1))? 255:0;
            }

            for (int tri_idx = 0;  tri_idx < Triangles_vector.size() ; tri_idx++)
            {
                //pixel[x][y] = (Triangles_vector[tri_idx].intersect())? 255:0;
            }
        }
    }
    ColorImage image;
    int x, y;
    Pixel p= {0,0,0};

    image.init(width, height);
    for (y=0; y<height; y++)
    {
        for (x=0; x<width; x++)
        {
            p.R = screen[x][y];
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
