#pragma once

#include <vector>

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
		NUM_SECTORS * 18 + //Top and bottom triangles (hemispheres)
		NUM_SECTORS * (NUM_STACKS * 2 - 2) * 18 + //Middle quadrilaterals (hemispheres)
		6 * NUM_PANELS; //Cylinder

	static const float DEFAULT_RADIUS;

private:
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
};
