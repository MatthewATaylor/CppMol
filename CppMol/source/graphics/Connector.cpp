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

	//Either point can be used to calculate angles
	Vec3 newPoint1 = point1 - midPoint;

	/*
	Vec3 angles(
		std::atan2(newPoint1.getY(), newPoint1.getZ()),
		std::atan2(newPoint1.getZ(), newPoint1.getX()),
		std::atan2(newPoint1.getY(), newPoint1.getZ()) - MathUtils::PI / 2
		//								 Template already rotated 90 degrees about Z axis
	);
	*/
	/*
	Vec3 angles(
		std::atan2(newPoint1.getY(), newPoint1.getZ()) - MathUtils::PI / 2,
		//								 Template already rotated 90 degrees about Z axis
		std::atan2(newPoint1.getY(), newPoint1.getZ()),
		std::atan2(newPoint1.getZ(), newPoint1.getX())
	);
	*/
	Vec3 angles(
		0.0f,
		0.0f,
		MathUtils::PI / 4
	);

	std::cout << angles << "\n";
	std::cout << length << "\n";

	scaleMatrix = MathUtils::MatGen::scale<float, 4>(Vec3(radius, length / 2, radius));
	rotationMatrix = MathUtils::MatGen::rotation<float>(angles);
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
