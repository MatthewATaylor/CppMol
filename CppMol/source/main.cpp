#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <stdexcept>

#include "AminoAcid.h"
#include "Protein.h"
#include "ResourceManager.h"
#include "Input.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"
#include "graphics/Window.h"
#include "graphics/Shader.h"
#include "graphics/SphereTemplate.h"
#include "graphics/ConnectorTemplate.h"
#include "graphics/Model.h"

std::vector<std::string> commands;
bool shouldExit = false;

//Run in separate thread
void displayGraphics() {
	if (!ResourceManager::initGLFW(3, 3)) {
		std::cin.get();
		return;
	}
	Window window("CppMol", 600, 600, false);
	if (!ResourceManager::initGLEW()) {
		std::cin.get();
		return;
	}
	ResourceManager::initOpenGL();
	Shader::loadDefaultShaders();

	SphereTemplate sphereTemplate;
	Model::setSphereTemplate(&sphereTemplate);

	ConnectorTemplate connectorTemplate;
	Model::setConnectorTemplate(&connectorTemplate);

	Vec3 point1(-1.0f, -1.0f, 0.0f);
	Vec3 point2(1.0f, 1.0f, 0.0f);

	Model::addSphere(point1, 0.6f, 0.5f, 0.5f, 0.5f);
	Model::addSphere(point2, 0.6f, 0.0f, 0.0f, 0.7f);
	Model::addSphere(Vec3(0.0f, 0.0f, 0.0f), 0.4f, 1.0f, 1.0f, 1.0f);
	Model::genSphereBuffers(true, true, true);

	Model::addConnector(0.5f, 0.7f, 0.0f, 0.0f, point1, point2);

	Protein protein;

	Camera camera(Vec3(0.0f, 0.0f, 10.0f));

	double prevMouseX = 0.0;
	double prevMouseY = 0.0;
	bool prevMousePosSet = false;

	while (!window.shouldClose() && !shouldExit) {
		Input::pollInput(&window);

		if (Input::keyPressed(&window, Key::ESCAPE)) {
			window.setShouldClose(true);
		}

		camera.zoom((float)Input::getMouseScrollOffsetY());

		double mouseX = Input::getMouseX();
		double mouseY = Input::getMouseY();
		if (prevMousePosSet) {
			if (Input::mouseButtonPressed(&window, MouseButton::LEFT)) {
				if (
					Input::keyPressed(&window, Key::LEFT_CTRL) ||
					Input::keyPressed(&window, Key::RIGHT_CTRL)
				) {
					camera.move(
						Vec3(
							float(prevMouseX - mouseX) / 100,
							float(prevMouseY - mouseY) / -100,
							0.0f
						)
					);
				}
				else {
					Model::rotate(
						Vec3(
							float(prevMouseY - mouseY) / 100,
							float(prevMouseX - mouseX) / 100,
							0.0f
						)
					);
				}
			}
			else if (Input::mouseButtonPressed(&window, MouseButton::RIGHT)) {
				Model::rotate(Vec3(0.0f, 0.0f, float(prevMouseX - mouseX) / 100));
			}
		}
		prevMouseX = mouseX;
		prevMouseY = mouseY;
		prevMousePosSet = true;

		//Read commands sent from console
		for (size_t i = 0; i < commands.size(); ++i) {
			if (commands[0].size() > 5 && commands[0].substr(0, 4) == "load") {
				std::string url = commands[0].substr(5);
				protein.loadPDB(url);
				protein.genModel();
			}
			else if (commands[0] == "reset") {
				Model::reset();
				protein.reset();
				camera.reset();
			}
			else if (commands[0] == "print sequence") {
				std::cout << protein << "\n\n";
			}
			else if (commands[0].size() > 11 && commands[0].substr(0, 10) == "set radius") {
				std::string radiusStr = commands[0].substr(11);
				if (radiusStr == "default") {
					Model::setSphereRadius(SphereTemplate::DEFAULT_RADIUS);
				}
				else {
					try {
						int radius = std::stoi(radiusStr);
						if (radius > 100) {
							radius = 100;
						}
						else if (radius < 0) {
							radius = 0;
						}
						Model::setSphereRadius(radius / 1000.0f);
					}
					catch (std::invalid_argument) {
						std::cerr << "ERROR > Invalid radius\n\n";
					}
				}
			}
			else {
				std::cerr << "ERROR > Invalid command\n\n";
			}
			commands.erase(commands.begin());
		}

		Window::clear(0, 0, 0);
		Model::render(Shader::getSphereDefault(), Shader::getConnectorDefault(), &window, &camera);
		window.swapBuffers();
	}

	Shader::freeResources();
	ResourceManager::freeResources();
}

int main() {
	std::thread graphicsThread(displayGraphics);

	std::string command;
	while (std::getline(std::cin, command)) {
		if (command == "exit") {
			shouldExit = true;
			break;
		}
		if (command.size() > 0) {
			commands.push_back(command);
		}
	}

	graphicsThread.join();

	return 0;
}
