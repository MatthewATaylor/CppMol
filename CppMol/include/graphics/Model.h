#pragma once

#include <vector>

#include <GL/glew.h>

#include "SphereTemplate.h"
#include "Shader.h"
#include "Window.h"
#include "Clock.h"
#include "Camera.h"
#include "Connector.h"
#include "ConnectorTemplate.h"
#include "bio/PDBFile.h"
#include "math/Mat.h"
#include "math/Vec.h"
#include "math/MathUtils.h"

class Model {
private:
	static unsigned int vertexArrayID;
	static unsigned int vertexBufferID; //For sphere template
	static unsigned int sphereBufferID; //For all spheres

	static const SphereTemplate *sphereTemplate;
	static const ConnectorTemplate *connectorTemplate;

	//Center (3) + radius (1) + color (3)
	static std::vector<float> atomSpheres;
	static std::vector<float> connectorSpheres;

	static std::vector<Connector*> connectors;

	//For model rotations
	static Mat4 modelMatrix;

	static void setSphereBufferAttributes();
	static void addSphere(
		const Vec3 &center,
		float radius,
		float r, float g, float b,
		std::vector<float> *vec
	);

public:
	static void reset();

	static void setSphereTemplate(const SphereTemplate *sphereTemplate);
	static void setConnectorTemplate(const ConnectorTemplate *connectorTemplate);

	//Generate graphical model of PDB molecule
	static void loadPDB(const PDBFile *pdbFile);

	static void addAtom(
		const Vec3 &center,
		float radius,
		float r, float g, float b
	);
	static void addConnector(
		float radius,
		float r, float g, float b,
		const Vec3 &point1, const Vec3 &point2
	);

	//Generate buffer objects for future rendering
	static void genBuffers();

	//Allocate and fill buffer for sphere template data
	static void fillSphereTemplateBuffer();

	//Allocate buffer for all sphere data
	static void allocateSphereBuffer();

	//Fill allocated sphere buffer with atom and/or connector sphere data
	static void syncSphereBuffer(bool syncAtomSpheres, bool syncConnectorSpheres);

	//Allocate and fill buffer for all sphere data
	static void fillSphereBuffer();

	/*
	TODO:

	Additional functions for modifying spheres and connectors (would call
	syncSphereBuffer() following change)

	Atom spheres:
		change color for selection
		change radius for selection

	Connectors and connector spheres:
		change color for selection
		change radius for selection
	*/

	static void render(
		const Shader *shader,
		const Shader *connectorShader,
		const Window *window, 
		const Camera *camera
	);

	static void setAtomRadius(float radius);
	static void setConnectorRadius(float radius);

	static void rotate(const Vec3 &angleRadians);
};
