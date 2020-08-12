#include "graphics/Connector.h"

Connector::Connector(
	ConnectorTemplate *connectorTemplate,
	float radius, const Vec3 &color,
	const Vec3 &point1, const Vec3 &point2) :

	connectorTemplate(connectorTemplate),
	radius(radius), color(color) {

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
	Vec3 angles(
		std::atan2(newPoint1.getY(), newPoint1.getZ()),
		std::atan2(newPoint1.getZ(), newPoint1.getX()),
		std::atan2(newPoint1.getY(), newPoint1.getZ()) - MathUtils::PI / 2
		//								 Template already rotated 90 degrees about Z axis
	);

	modelMatrix =
		MathUtils::MatGen::scale<float, 4>(Vec3(radius, length, radius)) *
		MathUtils::MatGen::rotation<float>(angles) *
		MathUtils::MatGen::translation<float, 4>(midPoint);
}

void Connector::render() {

}
