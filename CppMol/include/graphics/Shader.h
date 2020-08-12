#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>

#include "math/Mat.h"
#include "math/Vec.h"

enum class ShaderType {
	VERTEX, FRAGMENT
};

class Shader {
private:
	unsigned int id;
	unsigned int projectionLocation;
	unsigned int viewLocation;
	unsigned int modelLocation;
	unsigned int normalLocation;

	static Shader *sphere;
	static Shader *connector;

	void checkShaderCompileErrors(unsigned int id, ShaderType shaderType);
	void checkShaderLinkErrors(unsigned int id);
	void readShaders(const std::string &vertexShader, const std::string &fragmentShader);

public:
	Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

	void useProgram() const;

	unsigned int getUniformLocation(const std::string &uniformName) const;
	unsigned int getID() const;

	void setProjectionMatrix(const Mat4 &mat) const;
	void setViewMatrix(const Mat4 &mat) const;
	void setModelMatrix(const Mat4 &mat) const;
	void setNormalMatrix(const Mat3 &mat) const;

	void setVec3(const std::string &uniformName, float value1, float value2, float value3) const;

	static void loadDefaultShaders();

	static const Shader *getSphereDefault();
	static const Shader *getConnectorDefault();

	static void freeResources();
};
