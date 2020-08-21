#pragma once

#include <vector>
#include <cctype>
#include <functional>
#include <utility>

#include <GL/glew.h>

#include "Color.h"
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
#include "bio/Atom.h"
#include "math/Mat.h"
#include "math/Vec.h"
#include "math/MathUtils.h"

enum class ConnectorType {
	BACKBONE, DISULFIDE_BOND
};

class Model {
private:
	static unsigned int vertexArrayID;
	static unsigned int vertexBufferID; //For sphere template
	static unsigned int sphereBufferID; //For all spheres

	static const SphereTemplate *sphereTemplate;
	static const ConnectorTemplate *connectorTemplate;

	//Center (3) + radius (1) + color (3)
	static std::vector<float> atomSpheres;
	static std::vector<float> backboneSpheres;
	static std::vector<float> disulfideBondSpheres;

	static std::vector<Connector*> backboneSegments;
	static std::vector<Connector*> disulfideBonds;

	static const MoleculeData *moleculeData;

	//For model rotations
	static Mat4 modelMatrix;

	static void setSphereBufferAttributes();
	static void addSphere(
		const Vec3 &center,
		float radius,
		const Color *color,
		std::vector<float> *vec
	);

	static bool selectionIsValid(const Selection *selection);
	
	static void colorAtomsByFunc(
		std::function<Color(const Atom*)> func,
		const Selection *selection
	);
	static void colorConnectorsByFunc(
		std::function<Color(const Connector*)> func,
		const Selection *selection,
		ConnectorType connectorType
	);

public:
	static void reset();

	static void setSphereTemplate(const SphereTemplate *sphereTemplate);
	static void setConnectorTemplate(const ConnectorTemplate *connectorTemplate);

	//Generate graphical model of molecule given set of data
	static void loadMoleculeData(const MoleculeData *moleculeData);

	static void addAtom(
		const Vec3 &center,
		float radius,
		const Color *color
	);
	static void addConnector(
		const Atom *atom1, const Atom *atom2,
		float radius,
		const Color *color,
		const Vec3 &point1, const Vec3 &point2,
		ConnectorType connectorType
	);

	//Generate buffer objects for future rendering
	static void genBuffers();

	//Allocate and fill buffer for sphere template data
	static void fillSphereTemplateBuffer();

	//Allocate buffer for all sphere data
	static void allocateSphereBuffer();

	//Fill allocated sphere buffer with atom and/or connector sphere data
	static void syncSphereBuffer(
		bool syncAtomSpheres,
		bool syncBackboneSpheres,
		bool syncDisulfideBondSpheres
	);

	//Allocate and fill buffer for all sphere data
	static void fillSphereBuffer();

	static void render(
		const Shader *shader,
		const Shader *connectorShader,
		const Window *window,
		const Camera *camera
	);

	static void setAtomRadius(
		float radius, const Selection *selection, bool reversed = false
	);
	static void setConnectorRadius(
		float radius, const Selection *selection, ConnectorType connectorType,
		bool reversed = false
	);

	static void setAtomColor(const Color *color, const Selection *selection);
	static void colorAtomsDefault(const Selection *selection);
	static void colorAtomsByStructure(const Selection *selection);
	static void colorAtomsByChain(const Selection *selection);

	static void setConnectorColor(
		const Color *color, const Selection *selection,
		ConnectorType connectorType
	);
	static void colorConnectorsDefault(
		const Selection *selection, ConnectorType connectorType
	);
	static void colorConnectorsByStructure(
		const Selection *selection, ConnectorType connectorType
	);
	static void colorConnectorsByChain(
		const Selection *selection, ConnectorType connectorType
	);

	static void rotate(const Vec3 &angleRadians);
};
