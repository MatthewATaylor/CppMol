#include "graphics/Shader.h"

Shader::Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
	readShaders(vertexShaderSource, fragmentShaderSource);
	projectionLocation = getUniformLocation("projection");
	viewLocation = getUniformLocation("view");
	modelLocation = getUniformLocation("model");
	normalLocation = getUniformLocation("normalMat");
	colorLocation = getUniformLocation("color");
	cameraPosLocation = getUniformLocation("cameraPos");
}

Shader *Shader::main = nullptr;

void Shader::checkShaderCompileErrors(unsigned int id, ShaderType shaderType) {
	const int ERROR_BUFFER_SIZE = 512;
	int success;
	char infoLog[ERROR_BUFFER_SIZE];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, ERROR_BUFFER_SIZE, 0, infoLog);
		const char *shaderTypeStr = "Shader";
		if (shaderType == ShaderType::VERTEX) {
			shaderTypeStr = "Vertex shader";
		}
		else if (shaderType == ShaderType::FRAGMENT) {
			shaderTypeStr = "Fragment shader";
		}
		std::cerr << "ERROR > " << shaderTypeStr << " failed to compile: " << infoLog << "\n\n";
	}
}
void Shader::checkShaderLinkErrors(unsigned int id) {
	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		std::cerr << "ERROR > Shaders failed to link.\n\n";
	}
}
void Shader::readShaders(const std::string &vertexShader, const std::string &fragmentShader) {
	const char* vertexShader_cstr = vertexShader.c_str();
	const char* fragmentShader_cstr = fragmentShader.c_str();

	//Compile Vertex Shader
	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader_cstr, NULL);
	glCompileShader(vertexShaderID);
	checkShaderCompileErrors(vertexShaderID, ShaderType::VERTEX);

	//Compile Fragment Shader
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShader_cstr, NULL);
	glCompileShader(fragmentShaderID);
	checkShaderCompileErrors(fragmentShaderID, ShaderType::FRAGMENT);

	//Link Shaders
	unsigned int shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);
	this->id = shaderProgramID;
	checkShaderLinkErrors(id);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void Shader::useProgram() const {
	glUseProgram(id);
}
unsigned int Shader::getUniformLocation(const std::string &uniformName) const {
	return glGetUniformLocation(id, uniformName.c_str());
}
unsigned int Shader::getID() const {
	return id;
}

void Shader::setProjectionMatrix(const Mat4 &mat) const {
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, mat.getPtr());
}
void Shader::setViewMatrix(const Mat4 &mat) const {
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, mat.getPtr());
}
void Shader::setModelMatrix(const Mat4 &mat) const {
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mat.getPtr());
}
void Shader::setNormalMatrix(const Mat3 &mat) const {
	glUniformMatrix3fv(normalLocation, 1, GL_FALSE, mat.getPtr());
}

void Shader::setColor(float r, float g, float b) const {
	glUniform3f(colorLocation, r, g, b);
}
void Shader::setCameraPos(const Vec3 &position) const {
	glUniform3f(cameraPosLocation, position.getX(), position.getY(), position.getZ());
}

void Shader::loadShaders() {
	std::string vertexShader = 
R"(#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec3 fragmentPos;
out vec3 fragmentNormal;
out vec3 fragmentColor;
out vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

uniform vec3 color;
uniform vec3 cameraPos;

void main() {
	vec4 worldPos = model * vec4(vertexPos, 1.0f);
	gl_Position = projection * view * worldPos;

	fragmentPos = vec3(worldPos);
	fragmentNormal = normalMat * vertexPos;
	fragmentColor = color;
	viewPos = cameraPos;
}
)";
	std::string fragmentShader = 
R"(#version 330 core

in vec3 fragmentPos;
in vec3 fragmentNormal;
in vec3 fragmentColor;
in vec3 viewPos;

out vec4 finalFragmentColor;

void main() {
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDirection = vec3(0.0f, 0.0f, 1.0f);

	//Ambient
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * lightColor;

	//Diffuse
	vec3 normal = normalize(fragmentNormal);
	float diffuseStrength = max(dot(normal, lightDirection), 0.1f);
	vec3 diffuse = 0.65f * diffuseStrength * lightColor;

	//Specular
	//vec3 viewDirection = normalize(viewPos - fragmentPos);
	//vec3 reflectDirection = reflect(-lightDirection, normal);
	//float specularStrength = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
	//vec3 specular = 0.5f * specularStrength * lightColor;

	finalFragmentColor = vec4((ambient + diffuse) * fragmentColor, 1.0f);
}
)";
	main = new Shader(vertexShader, fragmentShader);
	main->useProgram();
}
const Shader *Shader::getMainShader() {
	return main;
}
void Shader::freeResources() {
	delete main;
	main = nullptr;
}