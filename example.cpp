// GLFW is used here to create the window and its openGL context.

#include<glew.h>
#include <glfw3.h>
#include<string>
#include "textRender.h"

using namespace std;

const int WIDTH = 500;
const int LENGTH = 500;

int main()
{
	

	if (!glfwInit())
		return -1;
	
	GLFWwindow* window;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, LENGTH, "Hello World", NULL, NULL);


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glewInit();

	textRenderer renderer("arial.png", "arial.txt");

	string text;
	int i = 1;

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		text = "Hello world! \n\nFrames: " + to_string(i);
		renderer.renderText(text, -0.5f, 0.0f, 3.5f, 1.0f, 1.0f, 1.0f);
			
		glfwSwapBuffers(window);
		glfwPollEvents();
		i++;
	}

	glfwTerminate();
	return 0;
}

