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

int TRACEDEPTH = 10;

using namespace std;
vector<string> split(char [], char *);
Color tracing(Ray, vector<Sphere>, vector<Triangle>, Light, vec3, int);
// Split string with space
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
// Reflection
Color reflection(Ray incoming_ray, vec3 normal, vec3 intersect_p, int depth,
                 vector<Sphere> Spheres_vector, vector<Triangle> Triangles_vector,
                 Light light, vec3 eye)
{
    vec3 relect_dir = incoming_ray.getDir() - 2 * (incoming_ray.getDir() * normal) * normal;
    Ray reflect_ray(intersect_p, relect_dir.normalize());
    Color Reflection_Color;

    Color clr = tracing(reflect_ray, Spheres_vector, Triangles_vector, light, eye, depth);
    return clr;
}
// Refraction
Color refraction(Ray incoming_ray, vec3 normal, vec3 intersect_p, float Nr,int depth,
                 vector<Sphere> Spheres_vector, vector<Triangle> Triangles_vector,
                 Light light, vec3 eye)
{
    float cos_theta1 = - normal * incoming_ray.getDir();
    float cos_theta2 = sqrt(1- (1 / pow(Nr, 2)) * (1 - pow(cos_theta1, 2)) );
    vec3 refract_dir = (incoming_ray.getDir() / Nr) + ( cos_theta1 / Nr - cos_theta2) * normal;
    Ray refract_ray(intersect_p, refract_dir);
    Color clr = tracing(refract_ray, Spheres_vector, Triangles_vector, light, eye, depth);
    return clr;
}
// Tracing Ray
Color tracing(Ray ray, vector<Sphere> Spheres_vector, vector<Triangle> Triangles_vector,
              Light light, vec3 eye, int depth)
{
    float t0 = 0;
    float t1 = 0;
    float curNearestDist = INT_MAX;
    int nearestObj = -1;
    Color curColor;

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
        vec3 intersect_p = ray.getPoint(curNearestDist);
        vec3 N;                                                     // Normal
        vec3 L = (light.getPostion() - intersect_p).normalize();    // Light - intersect
        vec3 V = (eye - intersect_p).normalize();                   // View - intersect
        vec3 H = (L + V).normalize();                               // L + V
        float exp;
        float li = 1.0;                                             // Light intensity
        float ka, kd, ks;
        float reflect, refract;
        float Nr;
        Color color;

        if(nearestObj < Spheres_vector.size())
        {
            color.R = Spheres_vector[nearestObj].getMaterial().color.R;
            color.G = Spheres_vector[nearestObj].getMaterial().color.G;
            color.B = Spheres_vector[nearestObj].getMaterial().color.B;
            // Get Normal
            N = intersect_p - Spheres_vector[nearestObj].getCenter();
            N.normalize();
            // Get constant
            exp = Spheres_vector[nearestObj].getMaterial().exp;
            ka = Spheres_vector[nearestObj].getMaterial().Ka;
            kd = Spheres_vector[nearestObj].getMaterial().Kd;
            ks = Spheres_vector[nearestObj].getMaterial().Ks;
            reflect = Spheres_vector[nearestObj].getMaterial().Reflect;
            refract = Spheres_vector[nearestObj].getMaterial().Refract;
            Nr = Spheres_vector[nearestObj].getMaterial().Nr;
            //cout << "Sphere" << endl;
        }
        else
        {
            nearestObj -= Spheres_vector.size();
            color.R = Triangles_vector[nearestObj].getMaterial().color.R;
            color.G = Triangles_vector[nearestObj].getMaterial().color.G;
            color.B = Triangles_vector[nearestObj].getMaterial().color.B;
            // Get Normal
            N = Triangles_vector[nearestObj].getNormal();
            N = N.normalize();
            // Get constant
            exp = Triangles_vector[nearestObj].getMaterial().exp;
            ka = Triangles_vector[nearestObj].getMaterial().Ka;
            kd = Triangles_vector[nearestObj].getMaterial().Kd;
            ks = Triangles_vector[nearestObj].getMaterial().Ks;
            reflect = Triangles_vector[nearestObj].getMaterial().Reflect;
            refract = Triangles_vector[nearestObj].getMaterial().Refract;
            Nr = Triangles_vector[nearestObj].getMaterial().Nr;
            //cout << "Triangle" << endl;
        }
        // Phong Reflection model
        float Id = ((N * L) > 0) ?  li * (N * L) : 0;
        float Is = ((N * H) > 0) ?  li * pow((H * N), exp) : 0;
        float Ia = 1;
        curColor.R = ka * Ia * color.R + kd * Id * color.R + ks * Is * 255;
        curColor.G = ka * Ia * color.G + kd * Id * color.G + ks * Is * 255;
        curColor.B = ka * Ia * color.B + kd * Id * color.B + ks * Is * 255;

        if(depth > 0){
            // Reflection Recursive Method
            Color reflection_color = reflection(ray, N, intersect_p, depth-1, Spheres_vector, Triangles_vector, light, eye);
            // Refraction Recursive Method
            Color refraction_color = refraction(ray, N, intersect_p, Nr, depth-1, Spheres_vector, Triangles_vector, light, eye);
            // Accumulated Color
            curColor.R += 0.2 * reflection_color.R + 0.2 * refraction_color.R;
            curColor.G += 0.2 * reflection_color.G + 0.2 * refraction_color.G;
            curColor.B += 0.2 * reflection_color.B + 0.2 * refraction_color.B;
        }
        curColor.R = (curColor.R > 255)? 255 :curColor.R;
        curColor.G = (curColor.G > 255)? 255 :curColor.G;
        curColor.B = (curColor.B > 255)? 255 :curColor.B;
        return curColor;
    }
    else
    {
        curColor.R = 0;
        curColor.G = 0;
        curColor.B = 0;
        return curColor;
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
            Ray ray(Eye, castRay.normalize());
            Color clr;
            clr = tracing(ray, Spheres_vector, Triangles_vector, light, Eye, 3);
            screen[i][j].setColor(clr.R, clr.G, clr.B);
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
    image.outputPPM("tracing.ppm");
    return 0;
}

