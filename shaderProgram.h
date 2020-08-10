#pragma once
#include<glew.h>
#include <iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

class shaderProgram
{
	public:
		int id;
		shaderProgram(const string vPath, const string fPath);
};

