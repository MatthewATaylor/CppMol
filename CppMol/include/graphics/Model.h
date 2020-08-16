#pragma once

#include <vector>
#include <cctype>

#include <GL/glew.h>

#include "SphereTemplate.h"
#include "Shader.h"
#include "Window.h"
#include "Clock.h"
#include "Camera.h"
#include "Connector.h"
#include "ConnectorTemplate.h"
#include "Selection.h"
#include "Parser.h"
#include "bio/MoleculeData.h"
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

	static const MoleculeData *moleculeData;

	//For model rotations
	static Mat4 modelMatrix;

	static void setSphereBufferAttributes();
	static void addSphere(
		const Vec3 &center,
		float radius,
		float r, float g, float b,
		std::vector<float> *vec
	);

	static bool selectionIsValid(const Selection *selection);

public:
	static void reset();
	static void delMoleculeData();

	static void setSphereTemplate(const SphereTemplate *sphereTemplate);
	static void setConnectorTemplate(const ConnectorTemplate *connectorTemplate);

	//Generate graphical model of molecule given set of data
	static void loadMoleculeData(const MoleculeData *moleculeData);

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

	static void render(
		const Shader *shader,
		const Shader *connectorShader,
		const Window *window, 
		const Camera *camera
	);

	static void setAtomRadius(float radius, const Selection *selection);
	//static void setConnectorRadius(float radius, const Selection *selection);

	static void rotate(const Vec3 &angleRadians);
};
