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
	unsigned int cameraPosLocation;

	static Shader *main;

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

	void setCameraPos(const Vec3 &position) const;

	static void loadShaders();

	static const Shader *getMainShader();
	static void freeResources();
};
