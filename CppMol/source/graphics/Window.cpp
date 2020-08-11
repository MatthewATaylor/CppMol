#include "graphics/Window.h"

Window::Window(const std::string &windowTitle, int screenWidth, int screenHeight, bool resizable) {
	if (!resizable) {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

	window = glfwCreateWindow(screenWidth, screenHeight, windowTitle.c_str(), 0, 0);
	if (!window) {
		std::cout << "ERROR > Window failed to initialize.\n\n";
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (resizable) {
		glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glViewport(0, 0, screenWidth, screenHeight);
	glScissor(0, 0, screenWidth, screenHeight);
}

void Window::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::setShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(window, shouldClose);
}
bool Window::shouldClose() const {
	return glfwWindowShouldClose(window);
}
GLFWwindow *Window::get() const {
	return window;
}
int Window::getWidth() const {
	int viewportData[4];
	glGetIntegerv(GL_VIEWPORT, viewportData);
	return viewportData[2];
}
int Window::getHeight() const {
	int viewportData[4];
	glGetIntegerv(GL_VIEWPORT, viewportData);
	return viewportData[3];
}
void Window::swapBuffers() {
	glfwSwapBuffers(window);
}

void Window::clear(int r, int g, int b) {
	const float MAX_COLOR_VALUE = 255.0f;
	glClearColor(
		r / MAX_COLOR_VALUE, 
		g / MAX_COLOR_VALUE, 
		b / MAX_COLOR_VALUE, 
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
