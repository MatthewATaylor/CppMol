#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <limits>
#include <utility>

#include "ResourceManager.h"
#include "Input.h"
#include "Selection.h"
#include "bio/Protein.h"
#include "bio/PDBFile.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"
#include "graphics/Window.h"
#include "graphics/Shader.h"
#include "graphics/SphereTemplate.h"
#include "graphics/ConnectorTemplate.h"
#include "graphics/Model.h"

#undef min
#undef max

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

	
	//Prepare Model
	SphereTemplate sphereTemplate;
	Model::setSphereTemplate(&sphereTemplate);

	ConnectorTemplate connectorTemplate;
	Model::setConnectorTemplate(&connectorTemplate);

	Model::genBuffers();
	Model::fillSphereTemplateBuffer();


	Protein protein;
	Camera camera(Vec3(0.0f, 0.0f, 10.0f));
	Selection selection = { Selection::ALL, 0 };

	double prevMouseX = 0.0;
	double prevMouseY = 0.0;
	bool prevMousePosSet = false;

	while (!window.shouldClose() && !shouldExit) {
		//Handle window input
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
				PDBFile *file = new PDBFile(url);
				protein = Protein(file);
				Model::loadMoleculeData(file);
			}
			else if (commands[0] == "reset") {
				Model::reset();
				Model::delMoleculeData();
				protein.reset();
				camera.reset();
				selection.tag = Selection::ALL;
			}
			else if (commands[0] == "print sequence") {
				std::cout << protein << "\n\n";
			}
			else if (commands[0] == "print selection") {
				switch (selection.tag) {
				case Selection::ALL:
					std::cout << "All\n\n";
					break;
				case Selection::NONE:
					std::cout << "None\n\n";
					break;
				case Selection::RESIDUE:
					std::cout << "Residue: " << selection.residue << "\n\n";
					break;
				case Selection::RESIDUE_RANGE:
					std::cout << "Residues: " << selection.residueRange.first << ":" <<
						selection.residueRange.second << "\n\n";
					break;
				case Selection::ELEMENT:
					std::cout << "Element: " << selection.element << "\n\n";
					break;
				case Selection::CHAIN:
					std::cout << "Chain: " << selection.chain << "\n\n";
				}
			}
			else if (commands[0].size() > 7 && commands[0].substr(0, 6) == "select") {
				std::string selectQuery = commands[0].substr(7);
				size_t colonPos = selectQuery.find(":");

				//Residue range provided
				if (colonPos != std::string::npos) {
					/*
					num1:num2
					:num2 -> 0:num2
					num1: -> num1:end
					: -> 0:end
					*/
					
					int start = -1, end = -1;

					if (colonPos != 0) {
						std::string startStr = selectQuery.substr(0, colonPos);
						try {
							start = std::stoi(startStr);
						}
						catch (...) {
							std::cerr << "ERROR > Invalid start residue\n\n";
							start = -1;
						}
					}
					else {
						start = 0;
					}

					if (colonPos != selectQuery.size() - 1) {
						std::string endStr = selectQuery.substr(colonPos + 1);
						try {
							end = std::stoi(endStr);
						}
						catch (...) {
							std::cerr << "ERROR > Invalid end residue\n\n";
							end = -1;
						}
					}
					else {
						end = std::numeric_limits<int>::max();
					}
					
					if (start >= 0 && end >= 0) {
						selection.tag = Selection::RESIDUE_RANGE;
						selection.residueRange = std::make_pair(start, end);
					}
				}

				//Single residue provided
				else {
					try {
						int residue = std::stoi(selectQuery);
						selection.tag = Selection::RESIDUE;
						selection.residue = residue;
					}
					catch (...) {
						std::cerr << "ERROR > Invalid residue\n\n";
					}
				}
			}
			else if (commands[0].size() > 5 && commands[0].substr(0, 4) == "atom") {
				std::string radiusStr = commands[0].substr(5);
				if (radiusStr == "default") {
					Model::setAtomRadius(SphereTemplate::DEFAULT_RADIUS, &selection);
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
						Model::setAtomRadius(radius / 1000.0f, &selection);
					}
					catch (...) {
						std::cerr << "ERROR > Invalid size argument\n\n";
					}
				}
			}
			else if (commands[0].size() > 9 && commands[0].substr(0, 8) == "backbone") {
				std::string radiusStr = commands[0].substr(9);
				if (radiusStr == "default") {
					//Model::setConnectorRadius(ConnectorTemplate::DEFAULT_RADIUS);
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
						//Model::setConnectorRadius(radius / 1000.0f);
					}
					catch (...) {
						std::cerr << "ERROR > Invalid size argument\n\n";
					}
				}
			}
			else {
				std::cerr << "ERROR > Invalid command\n\n";
			}
			commands.erase(commands.begin());
		}

		//Render model
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
