#include "graphics/Model.h"

unsigned int Model::vertexArrayID;
unsigned int Model::vertexBufferID;
unsigned int Model::sphereBufferID;
bool Model::vertexObjectsCreated = false;
Mat4 Model::modelMatrix(MathUtils::MatGen::identity<float, 4>());

std::vector<float> Model::spheres;
std::vector<Connector*> Model::connectors;

const SphereTemplate *Model::sphereTemplate = nullptr;
const ConnectorTemplate *Model::connectorTemplate = nullptr;

bool Model::drawSpheres = true;
bool Model::drawConnectors = true;

void Model::setSphereBufferAttributes() {
	//Center
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	//Radius
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);

	//Color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
}

void Model::reset() {
	spheres.clear();
	for (size_t i = 0; i < connectors.size(); ++i) {
		delete connectors[i];
	}
	connectors.clear();
}
void Model::setSphereTemplate(const SphereTemplate *sphereTemplate) {
	Model::sphereTemplate = sphereTemplate;
}
void Model::setConnectorTemplate(const ConnectorTemplate *connectorTemplate) {
	Model::connectorTemplate = connectorTemplate;
}
void Model::addSphere(const Vec3 &center, float radius, float r, float g, float b) {
	spheres.reserve(spheres.size() + 7);
	spheres.push_back(center.getX());
	spheres.push_back(center.getY());
	spheres.push_back(center.getZ());
	spheres.push_back(radius);
	spheres.push_back(r);
	spheres.push_back(g);
	spheres.push_back(b);
}
void Model::addConnector(
	float radius,
	float r, float g, float b,
	const Vec3 &point1, const Vec3 &point2
) {
	connectors.push_back(new Connector(radius, r, g, b, point1, point2));
}

void Model::genSphereBuffers(
	bool setupSphereTemplateBuffer,
	bool allocateSphereBuffer,
	bool updateSphereBuffer
) {
	if (!sphereTemplate) {
		std::cerr << "ERROR > Sphere template not set\n\n";
		return;
	}

	if (!vertexObjectsCreated) {
		glGenVertexArrays(1, &vertexArrayID);
		glGenBuffers(1, &vertexBufferID);
		glGenBuffers(1, &sphereBufferID);

		vertexObjectsCreated = true;
	}

	glBindVertexArray(vertexArrayID);

	const size_t POSITIONS_SIZE = sizeof(float) * SphereTemplate::NUM_VERTICES;
	const size_t SPHERES_SIZE = sizeof(float) * spheres.size();

	if (setupSphereTemplateBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, POSITIONS_SIZE, sphereTemplate->getVerticesPtr(), GL_STATIC_DRAW);

		//Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	}

	if (allocateSphereBuffer || updateSphereBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, sphereBufferID);
	}

	if (allocateSphereBuffer && updateSphereBuffer) {
		glBufferData(GL_ARRAY_BUFFER, SPHERES_SIZE, spheres.data(), GL_STATIC_DRAW);
		setSphereBufferAttributes();
	}
	else if (allocateSphereBuffer && !updateSphereBuffer) {
		glBufferData(GL_ARRAY_BUFFER, SPHERES_SIZE, 0, GL_STATIC_DRAW);
		setSphereBufferAttributes();
	}
	else if (!allocateSphereBuffer && updateSphereBuffer) {
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			SPHERES_SIZE,
			spheres.data()
		);
	}
}
void Model::render(
	const Shader *sphereShader,
	const Shader *connectorShader,
	const Window *window,
	const Camera *camera
) {
	Mat4 viewMatrix = camera->getViewMatrix();
	Mat4 projectionMatrix = camera->getProjectionMatrix(window);

	if (drawSpheres) {
		if (!sphereShader) {
			std::cerr << "ERROR > Unloaded sphere shaders\n\n";
			return;
		}

		glBindVertexArray(vertexArrayID);
		sphereShader->useProgram();

		sphereShader->setModelMatrix(modelMatrix);
		sphereShader->setNormalMatrix(MathUtils::MatGen::normal<float>(modelMatrix));
		sphereShader->setViewMatrix(viewMatrix);
		sphereShader->setProjectionMatrix(projectionMatrix);

		glDrawArraysInstanced(
			GL_TRIANGLES,
			0,
			GLsizei(SphereTemplate::NUM_VERTICES / 3),
			GLsizei(spheres.size() / 7)
		);
	}
	if (drawConnectors) {
		if (!connectorShader) {
			std::cerr << "ERROR > Unloaded connector shaders\n\n";
			return;
		}
		if (!connectorTemplate) {
			std::cerr << "ERROR > Connector template not set\n\n";
			return;
		}

		connectorTemplate->bind();
		connectorShader->useProgram();

		connectorShader->setViewMatrix(viewMatrix);
		connectorShader->setProjectionMatrix(projectionMatrix);

		for(size_t i = 0; i < connectors.size(); ++i) {
			//Set connector's model and normal matrices, set shader's color, draw arrays
			connectors[i]->render(connectorShader, modelMatrix, connectorTemplate);
		}
	}
}

void Model::setSphereRadius(float radius) {
	for (size_t i = 3; i < spheres.size(); i += 7) {
		spheres[i] = radius;
	}
	genSphereBuffers(false, false, true);
}
void Model::rotate(const Vec3 &angleRadians) {
	modelMatrix = modelMatrix * MathUtils::MatGen::rotation<float>(angleRadians);
}
