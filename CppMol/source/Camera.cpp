#include "Camera.h"

Camera::Camera(const Vec3 &initialPosition) :
	START_POS(initialPosition), position(initialPosition) {}

void Camera::move(const Vec3 &values) {
	position += values;
}
void Camera::zoom(float value) {
	fov -= value;
	if (fov < 0.1f) {
		fov = 0.1f;
	}
	else if (fov > 179.0f) {
		fov = 179.0f;
	}
}
void Camera::reset() {
	position = START_POS;
	fov = START_FOV;
}

Mat4 Camera::getViewMatrix() const {
	return MathUtils::MatGen::lookAt(
		position,
		Vec3(position.getX(), position.getY(), 1.0f),
		Vec3(0.0f, 1.0f, 0.0f)
	);
}
Mat4 Camera::getProjectionMatrix(const Window *window) const {
	return MathUtils::MatGen::perspective<float>(
		MathUtils::toRadians(fov),
		(float)window->getWidth(),
		(float)window->getHeight(),
		2.0f,
		20.0f
	);
}
