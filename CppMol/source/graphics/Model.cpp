#include "graphics/Model.h"

unsigned int Model::vertexArrayID;
unsigned int Model::vertexBufferID;
unsigned int Model::sphereBufferID;
bool Model::vertexObjectsCreated = false;
Mat4 Model::modelMatrix(MathUtils::MatGen::identity<float, 4>());

std::vector<float> Model::spheres;
const SphereTemplate *Model::sphereTemplate = nullptr;

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
}
void Model::setSphereTemplate(const SphereTemplate *sphereTemplate) {
	Model::sphereTemplate = sphereTemplate;
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

void Model::genBuffers(bool setupSphereTemplateBuffer, bool allocateSphereBuffer, bool updateSphereBuffer) {
	if (!sphereTemplate) {
		std::cerr << "ERROR > Sphere template not set\n\n";
		return;
	}

	if (!vertexObjectsCreated) {
		glGenVertexArrays(1, &vertexArrayID);
		glGenBuffers(1, &vertexBufferID);
		glGenBuffers(1, &sphereBufferID);

		//Only Model will do rendering, so just bind vertex array once
		glBindVertexArray(vertexArrayID);

		vertexObjectsCreated = true;
	}

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
void Model::render(const Shader *shader, const Window *window, const Camera *camera) {
	if (!shader) {
		std::cerr << "ERROR > Unloaded shaders\n\n";
		return;
	}

	shader->setModelMatrix(modelMatrix);
	shader->setNormalMatrix(MathUtils::MatGen::normal<float>(modelMatrix));
	shader->setViewMatrix(camera->getViewMatrix());
	shader->setProjectionMatrix(camera->getProjectionMatrix(window));

	glDrawArraysInstanced(
		GL_TRIANGLES, 
		0, 
		GLsizei(SphereTemplate::NUM_VERTICES / 3),
		GLsizei(spheres.size() / 7)
	);
}

void Model::setSphereRadius(float radius) {
	for (size_t i = 3; i < spheres.size(); i += 7) {
		spheres[i] = radius;
	}
	genBuffers(false, false, true);
}
void Model::rotate(const Vec3 &angleRadians) {
	modelMatrix = modelMatrix * MathUtils::MatGen::rotation<float>(angleRadians);
}
