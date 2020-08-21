#include "graphics/ConnectorTemplate.h"

const float ConnectorTemplate::HEIGHT = 1.0f;
const float ConnectorTemplate::DEFAULT_RADIUS = 0.01f;

void ConnectorTemplate::addVertexPoint(const Vec3 &point, const Vec3 &normal, bool isTop) {
	vertices[verticesIndex] = point.getX();
	vertices[verticesIndex + 1] = point.getY();
	vertices[verticesIndex + 2] = point.getZ();

	vertices[verticesIndex + 3] = normal.getX();
	vertices[verticesIndex + 4] = normal.getY();
	vertices[verticesIndex + 5] = normal.getZ();

	vertices[verticesIndex + 6] = (float)isTop;

	verticesIndex += VERTICES_PER_POINT;
}

void ConnectorTemplate::genCylinderVertices() {
	//Multiply this by i to get panel angle
	float panelAngleIntermediate = 2 * MathUtils::PI / NUM_PANELS;

	const float Y_VALUES[] = { 0.5f, 0.0f, -0.5f };

	for (unsigned int i = 0; i < NUM_PANELS; ++i) {
		float panelAngle = panelAngleIntermediate * i;
		float nextPanelAngle = panelAngleIntermediate * (i + 1);

		float x = std::cos(panelAngle);
		float z = std::sin(panelAngle);
		float nextX = std::cos(nextPanelAngle);
		float nextZ = std::sin(nextPanelAngle);

		//Add top and bottom half triangles
		for (unsigned int j = 0; j <= 1; ++j) {
			const float TOP_Y = Y_VALUES[j];
			const float BOTTOM_Y = Y_VALUES[j + 1];

			//Top left triangle
			addVertexPoint(Vec3(nextX, TOP_Y, nextZ), Vec3(nextX, 0.0f, nextZ), (bool)j); //Top left
			addVertexPoint(Vec3(nextX, BOTTOM_Y, nextZ), Vec3(nextX, 0.0f, nextZ), (bool)j); //Bottom left
			addVertexPoint(Vec3(x, TOP_Y, z), Vec3(x, 0.0f, z), (bool)j); //Top right

			//Bottom right triangle
			addVertexPoint(Vec3(x, TOP_Y, z), Vec3(x, 0.0f, z), (bool)j); //Top right
			addVertexPoint(Vec3(nextX, BOTTOM_Y, nextZ), Vec3(nextX, 0.0f, nextZ), (bool)j); //Bottom left
			addVertexPoint(Vec3(x, BOTTOM_Y, z), Vec3(x, 0.0f, z), (bool)j); //Bottom right
		}
	}
}

ConnectorTemplate::ConnectorTemplate() {
	genCylinderVertices();

	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verticesIndex * sizeof(float), vertices, GL_STATIC_DRAW);

	//Positions
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, VERTICES_PER_POINT * sizeof(GLfloat),
		(GLvoid*)0
	);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, VERTICES_PER_POINT * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(1);

	//Position indicators
	glVertexAttribPointer(
		2, 1, GL_FLOAT, GL_FALSE, VERTICES_PER_POINT * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(2);
}

const float *ConnectorTemplate::getVerticesPtr() const {
	return vertices;
}

size_t ConnectorTemplate::getVerticesLength() const {
	return verticesIndex;
}

void ConnectorTemplate::bind() const {
	glBindVertexArray(vertexArrayID);
}
