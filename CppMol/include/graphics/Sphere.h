#pragma once

#include <vector>
#include <cmath>

#include "GL/glew.h"

#include "Shader.h"
#include "Window.h"
#include "SphereTemplate.h"
#include "Camera.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"

class Model;

class Sphere {
private:
	const SphereTemplate *sphereTemplate;

	Vec3 center;
	float radius;
	float r, g, b;

	Mat4 modelMatrix;

	//Generate view and projection matrices once in Model::render
	void renderInModel(const Shader *shader);

public:
	Sphere(
		const SphereTemplate *sphereTemplate,
		const Vec3 &center, float radius,
		float r, float g, float b
	);

	void setRadius(float radius);

	void render(const Shader *shader, const Window *window, const Camera *camera);
	void rotate(const Vec3 &angleRadians);

	friend class Model; //For access to renderInModel()
};
