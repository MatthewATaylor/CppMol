#pragma once

#include <vector>

#include <GL/glew.h>

#include "math/Vec.h"
#include "math/MathUtils.h"

class ConnectorTemplate {
private:
	static const unsigned int NUM_PANELS = 16;
	static const float HEIGHT;

public:
	//3 position vertices + 3 normal vertices + 1 side indicator
	static const unsigned int VERTICES_PER_POINT = 7;

	static const unsigned int NUM_VERTICES = 2 * 3 * VERTICES_PER_POINT * NUM_PANELS * 2;

	static const float DEFAULT_RADIUS;

private:
	unsigned int vertexArrayID;
	unsigned int vertexBufferID;

	//Positions and normals
	float vertices[NUM_VERTICES];
	size_t verticesIndex = 0;

	//Add points to vertices array
	void addVertexPoint(const Vec3 &point, const Vec3 &normal, bool isTop);

	void genCylinderVertices();

public:
	ConnectorTemplate();
	const float *getVerticesPtr() const;
	size_t getVerticesLength() const;
	void bind() const;
};
