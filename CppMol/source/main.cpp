#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <utility>
#include <optional>
#include <stdexcept>

#include "ResourceManager.h"
#include "Input.h"
#include "Selection.h"
#include "Parser.h"
#include "bio/Protein.h"
#include "bio/PDBFile.h"
#include "bio/MoleculeData.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"
#include "graphics/Color.h"
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

	
	//Prepare Model
	SphereTemplate sphereTemplate;
	Model::setSphereTemplate(&sphereTemplate);

	ConnectorTemplate connectorTemplate;
	Model::setConnectorTemplate(&connectorTemplate);

	Model::genBuffers();
	Model::fillSphereTemplateBuffer();


	MoleculeData *moleculeData = nullptr;
	Camera camera(Vec3(0.0f, 0.0f, 10.0f));
	Selection selection;

	double prevMouseX = 0.0;
	double prevMouseY = 0.0;
	bool prevMousePosSet = false;

	while (!window.shouldClose() && !shouldExit) {
		//Handle window input
		Input::pollInput(&window);

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
			std::vector<std::string> commandWords =
				Parser::split(Parser::lowercase(commands[0]), ' ');
			if (commandWords.size() == 3 && commandWords[0] == "load") {
				if (commandWords[1] == "pdb") {
					delete moleculeData;
					moleculeData = nullptr;

					std::string url;
					if (commandWords[2].size() == 4) {
						url = "http://files.rcsb.org/view/" + commandWords[2] + ".pdb";
					}
					else {
						url = commandWords[2];
					}

					moleculeData = new PDBFile(url);
					Model::loadMoleculeData(moleculeData);
					selection.reset();
				}
			}
			else if (commandWords.size() == 1 && commandWords[0] == "unload") {
				delete moleculeData;
				moleculeData = nullptr;

				Model::reset();
				camera.reset();
				selection.reset();
			}
			else if (commandWords.size() == 1 && commandWords[0] == "reset") {
				selection.reset();
				Model::setAtomRadius(SphereTemplate::DEFAULT_RADIUS, &selection);
				Model::setConnectorRadius(
					ConnectorTemplate::DEFAULT_RADIUS,
					&selection,
					ConnectorType::BACKBONE
				);
				Model::setConnectorRadius(
					ConnectorTemplate::DEFAULT_RADIUS,
					&selection,
					ConnectorType::DISULFIDE_BOND
				);
			}
			else if (commandWords.size() == 2 && commandWords[0] == "print") {
				const char *noMoleculeDataError = "ERROR > No molecule data loaded\n\n";
				if (commandWords[1] == "help") {
					std::cout << "USAGE > print <sequence | helices | sheets | ssbonds " <<
						"| atoms | chains | selection>\n\n";
				}
				else if (commandWords[1] == "sequence") {
					if (moleculeData) {
						moleculeData->printSequence();
					}
					else {
						std::cerr << noMoleculeDataError;
					}
				}
				else if (commandWords[1] == "helices") {
					if (moleculeData) {
						moleculeData->printHelices();
					}
					else {
						std::cerr << noMoleculeDataError;
					}
				}
				else if (commandWords[1] == "sheets") {
					if (moleculeData) {
						moleculeData->printSheets();
					}
					else {
						std::cerr << noMoleculeDataError;
					}
				}
				else if (commandWords[1] == "ssbonds") {
					if (moleculeData) {
						moleculeData->printDisulfideBonds();
					}
					else {
						std::cerr << noMoleculeDataError;
					}
				}
				else if (commandWords[1] == "atoms") {
					if (moleculeData) {
						moleculeData->printAtoms();
					}
					else {
						std::cerr << noMoleculeDataError;
					}
				}
				else if (commandWords[1] == "chains") {
					if (moleculeData) {
						moleculeData->printChains();
					}
					else {
						std::cerr << noMoleculeDataError;
					}
				}
				else if (commandWords[1] == "selection") {
					selection.print();
				}
				else {
					std::cerr << "ERROR > Invalid argument: " << commandWords[1] << "\n\n";
				}
			}
			else if (commandWords.size() > 1 && commandWords[0] == "select") {
				//Help message
				if (commandWords.size() == 2 && commandWords[1] == "help") {
					std::cout <<
						"USAGE > select [r=<start residue>:<end residue> | r=<residue>] " <<
						"[e=<element>] [c=<chain>]\n\n";
				}
				else {
					std::vector<std::string> selectQuery = commandWords;
					selectQuery.erase(selectQuery.begin()); //Remove "select"
					selection.parseQuery(selectQuery);
				}
			}
			else if (commandWords.size() > 1 && commandWords[0] == "restrict") {
				//Help message
				if (commandWords.size() == 2 && commandWords[1] == "help") {
					std::cout <<
						"USAGE > restrict [r=<start residue>:<end residue> | r=<residue>] " <<
						"[e=<element>] [c=<chain>]\n\n";
				}
				else {
					std::vector<std::string> selectQuery = commandWords;
					selectQuery.erase(selectQuery.begin()); //Remove "restrict"
					selection.parseQuery(selectQuery);

					//Hide all renderables but those in selection
					Model::setAtomRadius(0.0f, &selection, true);
					Model::setConnectorRadius(0.0f, &selection, ConnectorType::BACKBONE, true);
					Model::setConnectorRadius(0.0f, &selection, ConnectorType::DISULFIDE_BOND, true);
				}
			}
			else if (commandWords.size() == 2 && commandWords[0] == "atom") {
				std::string radiusStr = commandWords[1];
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
			else if (commandWords.size() == 2 && (commandWords[0] == "backbone" || commandWords[0] == "ssbond")) {
				ConnectorType connectorType = commandWords[0] == "backbone" ?
					ConnectorType::BACKBONE :
					ConnectorType::DISULFIDE_BOND;

				std::string radiusStr = commandWords[1];
				if (radiusStr == "default") {
					Model::setConnectorRadius(
						ConnectorTemplate::DEFAULT_RADIUS,
						&selection,
						connectorType
					);
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
						Model::setConnectorRadius(
							radius / 1000.0f, &selection, connectorType
						);
					}
					catch (...) {
						std::cerr << "ERROR > Invalid size argument\n\n";
					}
				}
			}
			else if (commandWords.size() >= 2 && commandWords[0] == "color") {
				if (commandWords[1] == "atom") {
					if (commandWords.size() == 5) {
						int r, g, b;
						std::string rStr = commandWords[2];
						std::string gStr = commandWords[3];
						std::string bStr = commandWords[4];
						try {
							r = std::stoi(rStr);
							g = std::stoi(gStr);
							b = std::stoi(bStr);

							if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0) {
								throw std::exception();
							}

							Color color = Color::fromByte(
								(unsigned char)r, (unsigned char)g, (unsigned char)b
							);
							Model::setAtomColor(&color, &selection);
						}
						catch (...) {
							std::cerr << "ERROR > Invalid color argument: " <<
								"color values must be numbers between 0 and 255\n\n";
						}
					}
					else if (commandWords.size() == 3) {
						if (commandWords[2] == "default") {
							Model::colorAtomsDefault(&selection);
						}
						else if (commandWords[2] == "structure") {
							Model::colorAtomsByStructure(&selection);
						}
						else if (commandWords[2] == "chain") {
							Model::colorAtomsByChain(&selection);
						}
						else {
							try {
								Color color = Color::fromName(commandWords[2]);
								Model::setAtomColor(&color, &selection);
							}
							catch (...) {
								std::cerr << "ERROR > Invalid color argument\n\n";
							}
						}
					}
				}
				else if (commandWords[1] == "backbone" || commandWords[1] == "ssbond") {
					ConnectorType connectorType = commandWords[1] == "backbone" ?
						ConnectorType::BACKBONE :
						ConnectorType::DISULFIDE_BOND;

					if (commandWords.size() == 5) {
						int r, g, b;
						std::string rStr = commandWords[2];
						std::string gStr = commandWords[3];
						std::string bStr = commandWords[4];
						try {
							r = std::stoi(rStr);
							g = std::stoi(gStr);
							b = std::stoi(bStr);

							if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0) {
								throw std::exception();
							}

							Color color = Color::fromByte(
								(unsigned char)r, (unsigned char)g, (unsigned char)b
							);
							Model::setConnectorColor(&color, &selection, connectorType);
						}
						catch (...) {
							std::cerr << "ERROR > Invalid color argument: " <<
								"color values must be numbers between 0 and 255\n\n";
						}
					}
					else if (commandWords.size() == 3) {
						if (commandWords[2] == "default") {
							Model::colorConnectorsDefault(&selection, connectorType);
						}
						else if (commandWords[2] == "structure") {
							Model::colorConnectorsByStructure(&selection, connectorType);
						}
						else if (commandWords[2] == "chain") {
							Model::colorConnectorsByChain(&selection, connectorType);
						}
						else {
							try {
								Color color = Color::fromName(commandWords[2]);
								Model::setConnectorColor(&color, &selection, connectorType);
							}
							catch (...) {
								std::cerr << "ERROR > Invalid color argument\n\n";
							}
						}
					}
				}
				//TODO: add invalid command message for bad color command
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
