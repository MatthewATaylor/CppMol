#include "graphics/Model.h"

std::vector<Sphere*> Model::spheres;
const SphereTemplate *Model::sphereTemplate = nullptr;

void Model::reset() {
	for (size_t i = 0; i < spheres.size(); ++i) {
		delete spheres[i];
	}
	spheres.clear();
}

void Model::setSphereTemplate(const SphereTemplate *sphereTemplate) {
	Model::sphereTemplate = sphereTemplate;
}

void Model::addSphere(const Vec3 &center, float radius, float r, float g, float b) {
	if (sphereTemplate) {
		spheres.push_back(new Sphere(sphereTemplate, center, radius, r, g, b));
	}
	else {
		std::cerr << "ERROR > Sphere template not set\n\n";
	}
}

void Model::render(const Shader *shader, const Window *window, const Camera *camera) {
	if (!sphereTemplate) {
		std::cerr << "ERROR > Sphere template not set\n\n";
		return;
	}
	if (!shader) {
		std::cerr << "ERROR > Unloaded shaders\n\n";
		return;
	}

	sphereTemplate->bind();

	shader->setViewMatrix(camera->getViewMatrix());
	shader->setProjectionMatrix(camera->getProjectionMatrix(window));

	for (size_t i = 0; i < spheres.size(); ++i) {
		spheres[i]->renderInModel(shader);
	}
}

void Model::setSphereRadius(float radius) {
	for (size_t i = 0; i < spheres.size(); ++i) {
		spheres[i]->setRadius(radius);
	}
}
void Model::rotate(const Vec3 &angleRadians) {
	for (size_t i = 0; i < spheres.size(); ++i) {
		spheres[i]->rotate(angleRadians);
	}
}
