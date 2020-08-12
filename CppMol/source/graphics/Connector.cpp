#include "graphics/Connector.h"

Connector::Connector(
	float radius, float r, float g, float b,
	const Vec3 &point1, const Vec3 &point2) :

	radius(radius), r(r), g(g), b(b) {

	float length = std::sqrt(
		(point1.getX() - point2.getX()) * (point1.getX() - point2.getX()) +
		(point1.getY() - point2.getY()) * (point1.getY() - point2.getY()) +
		(point1.getZ() - point2.getZ()) * (point1.getZ() - point2.getZ())
	);

	Vec3 midPoint(
		(point1.getX() + point2.getX()) / 2,
		(point1.getY() + point2.getY()) / 2,
		(point1.getZ() + point2.getZ()) / 2
	);

	Vec3 defaultDirection(0.0f, 1.0f, 0.0f);
	Vec3 pointDifference = point1 - point2;
	Vec3 rotationAxis = defaultDirection.cross(pointDifference);
	float rotationAngle = std::acos(
		//-1 to +1: when +1 no rotation is needed
		defaultDirection.dot(pointDifference) / pointDifference.mag()
	);

	scaleMatrix = MathUtils::MatGen::scale<float, 4>(Vec3(radius, length, radius));
	rotationMatrix = MathUtils::MatGen::rotationAboutAxis<float>(rotationAxis, rotationAngle);
	translationMatrix = MathUtils::MatGen::translation<float, 4>(midPoint);
}

void Connector::render(
	const Shader *shader,
	const Mat4 &modelRotationMatrix,
	const ConnectorTemplate *connectorTemplate
) {
	Mat4 modelMatrix = scaleMatrix * rotationMatrix * translationMatrix * modelRotationMatrix;

	shader->setModelMatrix(modelMatrix);
	shader->setNormalMatrix(MathUtils::MatGen::normal<float>(modelMatrix));
	
	shader->setVec3("color", r, g, b);

	glDrawArrays(
		GL_TRIANGLES,
		0,
		GLsizei(
			connectorTemplate->getVerticesLength() / ConnectorTemplate::VERTICES_PER_POINT
		)
	);
}
