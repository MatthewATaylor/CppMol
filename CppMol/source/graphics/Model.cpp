#include "graphics/Model.h"

unsigned int Model::vertexArrayID;
unsigned int Model::vertexBufferID;
bool Model::vertexObjectsCreated = false;
Mat4 Model::modelMatrix(MathUtils::MatGen::identity<float, 4>());

std::vector<float> Model::spheres;
const SphereTemplate *Model::sphereTemplate = nullptr;

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

void Model::genBuffer(bool allocate) {
	if (!sphereTemplate) {
		std::cerr << "ERROR > Sphere template not set\n\n";
		return;
	}

	if (!vertexObjectsCreated) {
		glGenVertexArrays(1, &vertexArrayID);
		glGenBuffers(1, &vertexBufferID);

		//Only Model will do rendering, so just bind these once
		glBindVertexArray(vertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

		vertexObjectsCreated = true;
	}

	const size_t POSITIONS_SIZE = sizeof(float) * SphereTemplate::NUM_VERTICES;
	const size_t SPHERES_SIZE = sizeof(float) * spheres.size();
	const size_t BUFFER_SIZE = POSITIONS_SIZE + SPHERES_SIZE;

	if (allocate) {
		//Allocate memory
		glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, 0, GL_STATIC_DRAW);

		//Add SphereTemplate data
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			POSITIONS_SIZE,
			sphereTemplate->getVerticesPtr()
		);
	}

	//Add sphere data
	glBufferSubData(
		GL_ARRAY_BUFFER,
		POSITIONS_SIZE,
		SPHERES_SIZE,
		spheres.data()
	);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Spheres (instanced)
	
	//Center
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)POSITIONS_SIZE);
	glEnableVertexAttribArray(1);

	//Radius
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(POSITIONS_SIZE + 3));
	glEnableVertexAttribArray(2);

	//Color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(POSITIONS_SIZE + 4));
	glEnableVertexAttribArray(3);
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
		spheres.size() / 7
	);
}

void Model::setSphereRadius(float radius) {
	for (size_t i = 3; i < spheres.size(); i += 7) {
		spheres[i] = radius;
	}
	genBuffer(false);
}
void Model::rotate(const Vec3 &angleRadians) {
	modelMatrix = modelMatrix * MathUtils::MatGen::rotation<float>(angleRadians);
}
