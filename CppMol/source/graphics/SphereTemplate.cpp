#include "graphics/SphereTemplate.h"

void SphereTemplate::genPoints() {
	//Multiply this by j to get sector angle
	float sectorAngleIntermediate = 2 * MathUtils::PI / NUM_SECTORS;

	//Multiply this by i, subtract from pi / 2 to get stack angle
	float stackAngleIntermediate = MathUtils::PI / NUM_STACKS;

	for (unsigned int i = 0; i <= NUM_STACKS; ++i) {
		float stackAngle = MathUtils::PI / 2 - stackAngleIntermediate * i;
		float rCosStackAngle = std::cos(stackAngle);
		float y = std::sin(stackAngle);
		std::vector<Vec3> sectors;
		for (unsigned int j = 0; j <= NUM_SECTORS; ++j) {
			float sectorAngle = sectorAngleIntermediate * j;
			float z = rCosStackAngle * std::cos(sectorAngle);
			float x = rCosStackAngle * std::sin(sectorAngle);
			sectors.push_back(Vec3(x, y, z));
		}
		points.push_back(sectors);
	}
}

void SphereTemplate::addVertexPoint(const Vec3 &point) {
	positions[verticesIndex] = point.getX();
	positions[verticesIndex + 1] = point.getY();
	positions[verticesIndex + 2] = point.getZ();

	verticesIndex += 3;
}

void SphereTemplate::genVertices() {
	for (unsigned int i = 0; i < NUM_STACKS; ++i) {
		for (unsigned int j = 0; j < NUM_SECTORS; ++j) {
			//Top left triangle
			if (i != 0) {
				Vec3 point1 = points[i][j]; //Top left
				Vec3 point2 = points[i + 1][j]; //Bottom left
				Vec3 point3 = points[i][j + 1]; //Top right

				addVertexPoint(point1);
				addVertexPoint(point2);
				addVertexPoint(point3);
			}

			//Bottom right triangle
			if (i != NUM_STACKS - 1) {
				Vec3 point1 = points[i][j + 1]; //Top right
				Vec3 point2 = points[i + 1][j]; //Bottom left
				Vec3 point3 = points[i + 1][j + 1]; //Bottom right

				addVertexPoint(point1);
				addVertexPoint(point2);
				addVertexPoint(point3);
			}
		}
	}
	points.clear();
}

SphereTemplate::SphereTemplate() {
	genPoints();
	genVertices();

	/*
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);

	glBindVertexArray(vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(
		GL_ARRAY_BUFFER,
		NUM_VERTICES * sizeof(float),
		positions,
		GL_STATIC_DRAW
	);

	//Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	*/
}

/*
void SphereTemplate::bind() const {
	glBindVertexArray(vertexArrayID);
}
*/

const float *SphereTemplate::getVerticesPtr() const {
	return positions;
}
