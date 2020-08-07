#include "textRender.h"
#include "stb_image.h"

using namespace std;

void textRenderer::getFontData(string fileName) {

	ifstream file;

	file.open(fileName);
	int id;

	if (file.is_open())

		for (int i = 32; i <= 122; i++) {
			tChar character;
			
			file >> id;
			file >> character.x;
			file >> character.y;
			file >> character.width;
			file >> character.height;
			file >> character.xoffset;
			file >> character.yoffset;
			file >> character.xadvance;


			file.ignore(1);

			fontData[id] = character;

		}

	else
		cout << "ERROR LOADING FONT DATA" << endl;

}

void textRenderer::createBuffers(){

	glCreateVertexArrays(1, &VAO);
	glCreateBuffers(1, &VBO);
	glCreateBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void textRenderer::createTexture(const GLchar* fileName) {

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int  nrChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	stbi_image_free(data);
	
}

textRenderer::textRenderer(const GLchar* fontBitmapFileName, const string fontDataFileName, const string vertexShaderFile, const string fragmentShaderFile) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	createTexture(fontBitmapFileName);
	getFontData(fontDataFileName);
	program = new shaderProgram(vertexShaderFile, fragmentShaderFile);
	createBuffers();
}


void textRenderer::renderChar(const tChar &c, GLfloat startX, GLfloat startY, GLfloat scale, GLfloat r, GLfloat g, GLfloat b) {

	
	GLfloat x, y, w, h;


	x = c.x / GLfloat(width);
	y = 1 - c.y / GLfloat(height);

	w = c.width / GLfloat(width);
	h = c.height / GLfloat(height);

	GLfloat w2 = w * scale;
	GLfloat h2 = h * scale;


	GLfloat vertices[] = {
		// positions						 // colors				// texture coords
		 startX + w2,  startY + h2, 0.0f,   r, g, b,	     x + w, y,   // top right
		 startX + w2, startY, 0.0f,			r, g, b,		 x + w, y - h,   // bottom right
		 startX, startY, 0.0f,				r, g, b,				 x, y - h,  // bottom left
		 startX,  startY + h2, 0.0f,		r, g, b,		 x, y    // top left 
	};


	GLuint indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	3, 1, 2
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(program->id);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void textRenderer::renderText(const string text, GLfloat startX, GLfloat startY, GLfloat scale, GLfloat r, GLfloat g, GLfloat b) {

	GLfloat initialX = startX;

	tChar c;

	for (const GLchar x : text) {


		if (x == '\n') {
			startY -= 100.0f / float(height) * scale;
			startX = initialX;
		}
			
		
		else {
			int id = int(x);
			tFontData::iterator it;
			it = fontData.find(id);

			if (it != fontData.end()) {
				c = it->second;
				renderChar(c, startX, startY, scale, r, g, b);
				startX += c.xadvance / float(width) * scale;
			}
		}
			
	}
}

textRenderer::~textRenderer() {
	delete program;
}