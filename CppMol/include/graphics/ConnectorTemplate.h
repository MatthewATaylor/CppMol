#pragma once

#include <vector>

#include <GL/glew.h>

#include "math/Vec.h"
#include "math/MathUtils.h"

class ConnectorTemplate {
private:
	//Cylinder
	static const unsigned int NUM_PANELS = 16;
	static const float HEIGHT;

	//Hemispheres
	static const unsigned int NUM_SECTORS = 16;
	static const unsigned int NUM_STACKS = 4;

public:
	//Size of position and normal arrays
	static const unsigned int NUM_VERTICES =
		NUM_SECTORS * 36 + //Top and bottom triangles (hemispheres)
		NUM_SECTORS * (NUM_STACKS * 2 - 2) * 36 + //Middle quadrilaterals (hemispheres)
		36 * NUM_PANELS; //Cylinder

	//3 position vertices + 3 normal vertices
	static const unsigned int VERTICES_PER_POINT = 6;

	static const float DEFAULT_RADIUS;

private:
	unsigned int vertexArrayID;
	unsigned int vertexBufferID;

	std::vector<std::vector<Vec3>> topHemispherePoints;
	std::vector<std::vector<Vec3>> bottomHemispherePoints;

	//Positions and normals
	float vertices[NUM_VERTICES];
	size_t verticesIndex = 0;

	//Temporarily store points into 2D vector
	void genHemispherePoints();

	//Add points to positions array
	void addVertexPoint(const Vec3 &point, const Vec3 &normal);

	void genCylinderVertices();

	/*
	Convert list of hemisphere points into properly-ordered triangle 
	vertices
	*/
	void genHemisphereVertices();

public:
	ConnectorTemplate();
	const float *getVerticesPtr() const;
	size_t getVerticesLength() const;
	void bind() const;
};
