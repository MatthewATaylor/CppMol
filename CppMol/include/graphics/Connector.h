#pragma once

#include <cmath>

#include <GL/glew.h>

#include "ConnectorTemplate.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"

class Connector {
private:
	ConnectorTemplate *connectorTemplate = nullptr;

	float radius;
	Vec3 color;

	Mat4 modelMatrix;

public:
	Connector(
		ConnectorTemplate *connectorTemplate,
		float radius, const Vec3 &color,
		const Vec3 &point1, const Vec3 &point2
	);

	void render();
};
