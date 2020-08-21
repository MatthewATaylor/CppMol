#pragma once

#include <cmath>

#include <GL/glew.h>

#include "ConnectorTemplate.h"
#include "Shader.h"
#include "Color.h"
#include "bio/Atom.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"

class Connector {
private:
	float radius;
	Color color;

	float length;

	Mat4 scaleMatrix;
	Mat4 rotationMatrix;
	Mat4 translationMatrix;

public:
	const Atom *atom1;
	const Atom *atom2;

	Connector(
		const Atom *atom1, const Atom *atom2,
		float radius, const Color *color,
		const Vec3 &point1, const Vec3 &point2
	);

	void render(
		const Shader *shader,
		const Mat4 &modelRotationMatrix,
		const ConnectorTemplate *connectorTemplate
	);

	void setRadius(float radius);
	void setColor(const Color *color);
};
