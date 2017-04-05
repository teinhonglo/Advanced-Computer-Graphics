#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <string.h>
#include <limits.h>
#include "algebra3/algebra3.h"
#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "output.h"
#include "Triangle.h"
#include "Light.h"


using namespace std;

vector<string> split(char str [], char * pattern)
{

    std::vector<std::string> result;
    char *p;
    p = strtok(str,pattern);
    while(p)
    {
        result.push_back(p);
        p=strtok(NULL,pattern);
    }
    return result;
}

int tracing(Ray ray, vector<Sphere> Spheres_vector, vector<Triangle> Triangles_vector, vector< vector<Color> > &screen, int i, int j)
{
    float t0 = 0;
    float t1 = 0;
    float curNearestDist = INT_MAX;
    int nearestObj = -1;
    // Sphere
    for (int sp_idx = 0;  sp_idx < Spheres_vector.size() ; sp_idx++)
    {
        // Compare whether distance is nearest or not
        if((Spheres_vector[sp_idx].intersect(ray, t0, t1)))
        {
            if (t0 < curNearestDist)
            {
                nearestObj = sp_idx;
                curNearestDist = t0;
            }
        }
    }
    // Triangle
    for (int tri_idx = 0;  tri_idx < Triangles_vector.size() ; tri_idx++)
    {
        // Compare whether distance is nearest or not
        if(Triangles_vector[tri_idx].intersect(ray, t0, t1))
        {
            //cout << t0 << endl;
            nearestObj = Spheres_vector.size() + tri_idx;
        }
    }
    // Keep if closest
    if(nearestObj >= 0)
    {
        if(nearestObj < Spheres_vector.size())
        {

            screen[i][j].R = Spheres_vector[nearestObj].getMaterial().color.R;
            screen[i][j].G = Spheres_vector[nearestObj].getMaterial().color.G;
            screen[i][j].B = Spheres_vector[nearestObj].getMaterial().color.B;
            //cout << "Sphere" << endl;
        }
        else
        {
            nearestObj -= Spheres_vector.size();
            screen[i][j].R = Triangles_vector[nearestObj].getMaterial().color.R;
            screen[i][j].G = Triangles_vector[nearestObj].getMaterial().color.G;
            screen[i][j].B = Triangles_vector[nearestObj].getMaterial().color.B;
            //cout << "Triangle" << endl;
        }
    }
    else
    {
        screen[i][j].R = 0;
        screen[i][j].G = 0;
        screen[i][j].B = 0;
    }
}


int main()
{
    ifstream file( "hw2_input.txt");
    char * pattern = " ";
    string line;
    vector<string> info;
    vec3 Eye;
    vec3 View_Direction;
    float FieldOfView;
    int width, height;
    int distance = 1;
    vector<Sphere> Spheres_vector;
    vector<Triangle> Triangles_vector;
    Light light;
    Material material;

    // Read information
    while(getline(file, line))
    {
        char *cstr = new char[line.length() + 1];
        strcpy(cstr, line.c_str());
        info = split(cstr, pattern);
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
            Sphere s(oX, oY, oZ, radius, material);
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
            Triangle t(x1, y1, z1, x2, y2, z2, x3, y3, z3, material);
            Triangles_vector.push_back(t);
        }
        else if(info[0] == "M")
        {
            float r = atof(info[1].c_str());
            float g = atof(info[2].c_str());
            float b = atof(info[3].c_str());
            float Ka = atof(info[4].c_str());
            float Kd = atof(info[5].c_str());
            float Ks = atof(info[6].c_str());
            float exp = atof(info[7].c_str());
            float Reflect = atof(info[8].c_str());
            float Refract = atof(info[9].c_str());
            float Nr = atof(info[10].c_str());
            material.setMaterial(r, g, b, Ka, Kd, Ks, exp, Reflect, Refract, Nr);
        }
        else if(info[0] == "L")
        {
            float x1 = atof(info[1].c_str());
            float y1 = atof(info[2].c_str());
            float z1 = atof(info[3].c_str());
            light.setLight(vec3(x1, y1, z1));
        }
        else
        {
            cout << "not mapping";
        }
    }

    /**
    For every pixel
        Construct a ray from the eye
        For every object in the scene
            Find intersection with ray
        Keep if closest
    **/

    float halfWidth = float(width) / 2;
    float halfHeight = float(height) / 2;
    distance = halfWidth / tan(M_PI * 0.5 * FieldOfView / 180.);
    vec3 lookAtPoint = Eye + distance * View_Direction;
    vec3 up (0, 1, 0);
    vec3 U = View_Direction ^ up;
    U = U.normalize();
    vec3 V = U ^ View_Direction;
    V = V.normalize();

    vector<vector <Color> > screen(width, vector <Color>(height));
    float aspectRatio = width / float(height);
    float viewPlaneHalfWidth = tan(M_PI * 0.5 * FieldOfView / 180.);
    float viewPlaneHalfHeight = aspectRatio * viewPlaneHalfWidth;
    //vec3 viewPlaneTopLeftPoint = lookAtPoint - halfHeight * prod(V, viewPlaneHalfHeight) + halfWidth * prod(U, viewPlaneHalfWidth);
    vec3 viewPlaneTopLeftPoint = lookAtPoint + halfHeight * V + halfWidth * U;
    vec3 xIncVector = -(U * 2 * halfWidth) / width;
    vec3 yIncVector = -(V * 2 * halfHeight) / height;
    float t0, t1;
    float curNearestDist;
    int nearestObj;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            vec3 viewPlanePoint = viewPlaneTopLeftPoint + i*xIncVector + j*yIncVector;
            vec3 castRay = viewPlanePoint - Eye;
            Ray ray(Eye, castRay);
            tracing(ray, Spheres_vector, Triangles_vector, screen, i, j);
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

            p.R = screen[x][y].R;
            p.G = screen[x][y].G;
            p.B = screen[x][y].B;
            image.writePixel(x, y, p);
        }
    }
    image.outputPPM("reds.ppm");
    return 0;
}

