#pragma once
#include<glew.h>
#include <glfw3.h>
#include <map>
#include <fstream>
#include "shaderProgram.h"


using namespace std;

typedef struct {
	float x;
	float y;
	float width;
	float height;
	float xoffset;
	float yoffset;
	float xadvance;
}tChar;

typedef map<int, tChar> tFontData;


class textRenderer {
	public:
		textRenderer(const GLchar* fontBitmapFileName, const string fontDataFileName);
		void renderText(const string text, GLfloat startX, GLfloat startY, GLfloat scale, GLfloat r, GLfloat g, GLfloat b);
		~textRenderer();

	private:
		GLuint texture;
		tFontData fontData;
		unsigned int indices[6] = { 0, 1, 3, 3, 1, 2};
		GLuint VBO;
		GLuint VAO;
		GLuint EBO;
		GLint width, height;
		void getFontData(string fileName);
		void createBuffers();
		void createTexture(const GLchar* fileName);
		shaderProgram *program;
		void renderChar(const tChar& c, GLfloat startX, GLfloat startY, GLfloat scale, GLfloat r, GLfloat g, GLfloat b);
	
};