#pragma once
#include<glad/glad.h>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader {
public:
	unsigned int ID;

	//compile and link shader program
	Shader(const char* vertexPath, const char* fragmentPath);

	void activateGeometryShader(const char* geometryShaderPath);

	//activate the shader
	void use();

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};
