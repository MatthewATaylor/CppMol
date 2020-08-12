#include "graphics/ConnectorTemplate.h"

const float ConnectorTemplate::HEIGHT = 1.0f;
const float ConnectorTemplate::DEFAULT_RADIUS = 0.025f;

void ConnectorTemplate::genHemispherePoints() {
	//Multiply this by j to get sector angle
	float sectorAngleIntermediate = 2 * MathUtils::PI / NUM_SECTORS;

	//Multiply this by i, subtract from pi / 2 to get stack angle
	float stackAngleIntermediate = MathUtils::PI / 2 / NUM_STACKS;

	for (unsigned int i = 0; i <= NUM_STACKS; ++i) {
		//Angle ranges from 0 to pi / 2
		float stackAngle = MathUtils::PI / 2 - stackAngleIntermediate * i;
		
		float rCosStackAngle = std::cos(stackAngle);
		float y = std::sin(stackAngle);

		std::vector<Vec3> topSectors;
		std::vector<Vec3> bottomSectors;

		for (unsigned int j = 0; j <= NUM_SECTORS; ++j) {
			float sectorAngle = sectorAngleIntermediate * j;
			float z = rCosStackAngle * std::cos(sectorAngle);
			float x = rCosStackAngle * std::sin(sectorAngle);

			topSectors.push_back(Vec3(x, y + HEIGHT / 2, z));
			bottomSectors.push_back(Vec3(x, -y - HEIGHT / 2, z));
		}

		topHemispherePoints.push_back(topSectors);
		bottomHemispherePoints.push_back(bottomSectors);
	}
}

void ConnectorTemplate::addVertexPoint(const Vec3 &point, const Vec3 &normal) {
	vertices[verticesIndex] = point.getX();
	vertices[verticesIndex + 1] = point.getY();
	vertices[verticesIndex + 2] = point.getZ();

	vertices[verticesIndex + 3] = normal.getX();
	vertices[verticesIndex + 4] = normal.getY();
	vertices[verticesIndex + 5] = normal.getZ();

	verticesIndex += 6;
}

void ConnectorTemplate::genCylinderVertices() {
	//Multiply this by i to get panel angle
	float panelAngleIntermediate = 2 * MathUtils::PI / NUM_PANELS;

	float topY = 0.5f;
	float bottomY = -0.5f;

	for (unsigned int i = 0; i < NUM_PANELS; ++i) {
		float panelAngle = panelAngleIntermediate * i;
		float nextPanelAngle = panelAngleIntermediate * (i + 1);

		float x = std::cos(panelAngle);
		float z = std::sin(panelAngle);
		float nextX = std::cos(nextPanelAngle);
		float nextZ = std::sin(nextPanelAngle);

		//Top left triangle
		addVertexPoint(Vec3(nextX, topY, nextZ), Vec3(nextX, 0.0f, nextZ)); //Top left
		addVertexPoint(Vec3(nextX, bottomY, nextZ), Vec3(nextX, 0.0f, nextZ)); //Bottom left
		addVertexPoint(Vec3(x, topY, z), Vec3(x, 0.0f, z)); //Top right

		//Bottom right triangle
		addVertexPoint(Vec3(x, topY, z), Vec3(x, 0.0f, z)); //Top right
		addVertexPoint(Vec3(nextX, bottomY, nextZ), Vec3(nextX, 0.0f, nextZ)); //Bottom left
		addVertexPoint(Vec3(x, bottomY, z), Vec3(x, 0.0f, z)); //Bottom right
	}
}
void ConnectorTemplate::genHemisphereVertices() {
	Vec3 normalOffset(0.0f, HEIGHT / 2, 0.0f);

	Vec3 point1;
	Vec3 point2;
	Vec3 point3;

	auto addTriangle = [&](int offsetDir) {
		addVertexPoint(point1, point1 + normalOffset * offsetDir);
		addVertexPoint(point2, point2 + normalOffset * offsetDir);
		addVertexPoint(point3, point3 + normalOffset * offsetDir);
	};

	for (unsigned int i = 0; i < NUM_STACKS; ++i) {
		for (unsigned int j = 0; j < NUM_SECTORS; ++j) {
			//Triangles only used in full tiles
			if (i != 0) {
				//Top hemisphere
				point1 = topHemispherePoints[i][j]; //Top left
				point2 = topHemispherePoints[i + 1][j]; //Bottom left
				point3 = topHemispherePoints[i][j + 1]; //Top right
				addTriangle(-1);

				//Bottom hemisphere
				point1 = bottomHemispherePoints[i][j]; //Bottom left
				point2 = bottomHemispherePoints[i][j + 1]; //Bottom right
				point3 = bottomHemispherePoints[i + 1][j]; //Top left
				addTriangle(1);
			}

			//Triangles used in ends and full tiles

			//Top hemisphere
			point1 = topHemispherePoints[i][j + 1]; //Top right
			point2 = topHemispherePoints[i + 1][j]; //Bottom left
			point3 = topHemispherePoints[i + 1][j + 1]; //Bottom right
			addTriangle(-1);

			//Bottom hemisphere
			point1 = bottomHemispherePoints[i][j + 1]; //Bottom right
			point2 = bottomHemispherePoints[i + 1][j + 1]; //Top right
			point3 = bottomHemispherePoints[i + 1][j]; //Top left
			addTriangle(1);
		}
	}
	topHemispherePoints.clear();
	bottomHemispherePoints.clear();
}

ConnectorTemplate::ConnectorTemplate() {
	genCylinderVertices();

	genHemispherePoints();
	genHemisphereVertices();
}

const float *ConnectorTemplate::getVerticesPtr() const {
	return vertices;
}
