#include "graphics/Sphere.h"

Sphere::Sphere(
	const SphereTemplate *sphereTemplate,
	const Vec3 &center, float radius,
	float r, float g, float b) :
	sphereTemplate(sphereTemplate), radius(radius), center(center), r(r), g(g), b(b) {

	modelMatrix = 
		MathUtils::MatGen::scale<float, 4>(Vec3(radius)) * 
		MathUtils::MatGen::translation<float, 4>(center);
}

void Sphere::renderInModel(const Shader *shader) {
	shader->setColor(r / 255, g / 255, b / 255);
	shader->setModelMatrix(modelMatrix);
	shader->setNormalMatrix(MathUtils::MatGen::normal(modelMatrix));

	glDrawArrays(GL_TRIANGLES, 0, GLsizei(SphereTemplate::NUM_VERTICES / 3));
}

void Sphere::setRadius(float radius) {
	modelMatrix =
		MathUtils::MatGen::scale<float, 4>(Vec3(radius / this->radius)) *
		modelMatrix;
	this->radius = radius;
}

void Sphere::render(const Shader *shader, const Window *window, const Camera *camera) {
	if (!shader) {
		std::cerr << "ERROR > Unloaded shaders\n\n";
		return;
	}

	sphereTemplate->bind();

	shader->setViewMatrix(camera->getViewMatrix());
	shader->setProjectionMatrix(camera->getProjectionMatrix(window));

	shader->setColor(r / 255, g / 255, b / 255);

	shader->setModelMatrix(modelMatrix);
	shader->setNormalMatrix(MathUtils::MatGen::normal(modelMatrix));

	glDrawArrays(GL_TRIANGLES, 0, GLsizei(SphereTemplate::NUM_VERTICES / 3));
}

void Sphere::rotate(const Vec3 &angleRadians) {
	modelMatrix = modelMatrix * MathUtils::MatGen::rotation<float>(angleRadians);
}
