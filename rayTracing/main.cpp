#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
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
    ifstream file( "hw1_input.txt" );
    string pattern = " ";
	string line;
	vector<string> info;
	while(getline(file, line))
	{
        info = split(line, pattern);
        for(int i = 1; i < info.size(); i++){
            if( info[0] == "E")
            {

            }else if(info[0] == "V")
            {

            }else if(info[0] == "F")
            {

            }else if(info[0] == "R")
            {

            }else if(info[0] == "S")
            {

            }else if(info[0] == "T")
            {

            }else{
                cout << "not mapping";
            }
        }

	}
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
