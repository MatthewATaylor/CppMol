#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ResourceManager {
private:
	static const int DEFAULT_MAJOR_GL = 3;
	static const int DEFAULT_MINOR_GL = 3;

public:
	//Initialize the GLFW library (must be done before initGLEW)
	static bool initGLFW();

	//Initialize the GLFW library (must be done before initGLEW)
	static bool initGLFW(int majorOpenGLVersion, int minorOpenGLVersion);
	
	static bool initGLEW();
	static void initOpenGL();
	static void freeResources();
};
