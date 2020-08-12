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
#include "math/Mat.h"
#include "math/Vec.h"
#include "math/MathUtils.h"

class Model {
private:
	static unsigned int vertexArrayID;
	static unsigned int vertexBufferID;
	static unsigned int sphereBufferID;
	static bool vertexObjectsCreated;

	static const SphereTemplate *sphereTemplate;
	static const ConnectorTemplate *connectorTemplate;

	//Center (3) + radius (1) + color (3)
	static std::vector<float> spheres;

	static std::vector<Connector*> connectors;

	//For model rotations
	static Mat4 modelMatrix;

	static void setSphereBufferAttributes();

public:
	static bool drawSpheres;
	static bool drawConnectors;

	static void reset();
	static void setSphereTemplate(const SphereTemplate *sphereTemplate);
	static void setConnectorTemplate(const ConnectorTemplate *connectorTemplate);
	static void addSphere(const Vec3 &center, float radius, float r, float g, float b);
	static void addConnector(
		float radius,
		float r, float g, float b,
		const Vec3 &point1, const Vec3 &point2
	);

	static void genSphereBuffers(
		bool setupSphereTemplateBuffer, 
		bool allocateSphereBuffer, 
		bool updateSphereBuffer
	);
	static void render(
		const Shader *shader,
		const Shader *connectorShader,
		const Window *window, 
		const Camera *camera
	);

	static void setSphereRadius(float radius);
	static void rotate(const Vec3 &angleRadians);
};
