#include <iostream>
#include"../algebra3/algebra3.h"
using namespace std;
struct Sphere
{
    vec3 centerV3;
    float radius;

    void setCenter(float oX, float oY, float oZ){
        centerV3.set(oX, oY, oZ);
    }

};
struct Triangle
{

};
int main()
{
    cout << "Hello world!" << endl;
    Sphere s1;
    s1.setCenter(1.0, 2.0, 3.0);
    vec3 v3;
    vec3 v4(1, 2, 3);
    v3 = v3 + v4;
    cout << v3.;
    v3.printCoor();
    system("pause");
    return 0;
}
