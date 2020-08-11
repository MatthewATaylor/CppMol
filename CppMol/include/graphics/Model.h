#pragma once

#include <vector>

#include <GL/glew.h>

#include "Sphere.h"
#include "SphereTemplate.h"
#include "Shader.h"
#include "Window.h"
#include "Clock.h"
#include "Camera.h"
#include "math/Mat.h"
#include "math/MathUtils.h"

class Model {
private:
	static std::vector<Sphere*> spheres;
	static const SphereTemplate *sphereTemplate;

public:
	static void reset();
	static void setSphereTemplate(const SphereTemplate *sphereTemplate);
	static void addSphere(const Vec3 &center, float radius, float r, float g, float b);
	static void render(const Shader *shader, const Window *window, const Camera *camera);

	static void setSphereRadius(float radius);
	static void rotate(const Vec3 &angleRadians);
};
