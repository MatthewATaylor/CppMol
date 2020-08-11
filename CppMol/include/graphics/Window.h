#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
private:
	GLFWwindow *window;

	static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

public:
	Window(const std::string &windowTitle, int screenWidth, int screenHeight, bool resizable);
	void setShouldClose(bool shouldClose);
	bool shouldClose() const;
	GLFWwindow *get() const;
	int getWidth() const;
	int getHeight() const;
	void swapBuffers();

	static void clear(int r, int g, int b);
};
