#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include "math/Vec.h"
#include "math/MathUtils.h"

class SphereTemplate {
private:
	static const unsigned int NUM_SECTORS = 16;
	static const unsigned int NUM_STACKS = 8;

public:
	//Size of position and normal arrays
	static const unsigned int NUM_VERTICES =
		NUM_SECTORS * 18 + //Top and bottom triangles
		NUM_SECTORS * (NUM_STACKS - 2) * 18; //Middle quadrilaterals

private:
	std::vector<std::vector<Vec3>> points;

	float positions[NUM_VERTICES];
	size_t verticesIndex = 0;

	void genPoints();

	//Add points, normals, and colors to vertices
	void addVertexPoint(const Vec3 &point);

	/*
	Convert list of points into properly-ordered triangle vertices
	(points + colors)
	*/
	void genVertices();

public:
	SphereTemplate();
	const float *getVerticesPtr() const;
};
