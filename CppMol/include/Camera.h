#pragma once

#include "graphics/Shader.h"
#include "graphics/Window.h"
#include "math/Vec.h"
#include "math/Mat.h"
#include "math/MathUtils.h"

class Camera {
private:
	const Vec3 START_POS;
	const float START_FOV = 45.0f;

	Vec3 position;
	float fov = 45.0f;

public:
	Camera(const Vec3 &initialPosition);
	void move(const Vec3 &values);
	void zoom(float value);
	void reset();
	Mat4 getViewMatrix() const;
	Mat4 getProjectionMatrix(const Window *window) const;
};
