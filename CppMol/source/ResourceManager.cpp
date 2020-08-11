#include "ResourceManager.h"

bool ResourceManager::initGLFW() {
	if (!glfwInit()) {
		std::cout << "ERROR > GLFW failed to initialize.\n\n";
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, DEFAULT_MAJOR_GL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, DEFAULT_MINOR_GL);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 1;
}
bool ResourceManager::initGLFW(int majorOpenGLVersion, int minorOpenGLVersion) {
	if (!glfwInit()) {
		std::cout << "ERROR > GLFW failed to initialize.\n\n";
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorOpenGLVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorOpenGLVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 1;
}
bool ResourceManager::initGLEW() {
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "ERROR > GLEW failed to initialize.\n\n";
		glfwTerminate();
		return 0;
	}
	return 1;
}
void ResourceManager::initOpenGL() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void ResourceManager::freeResources() {
	glfwTerminate();
}
