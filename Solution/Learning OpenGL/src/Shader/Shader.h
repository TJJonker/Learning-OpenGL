#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "../Debugging/debugger.h"

class Shader {
public:
	unsigned int m_ID;

private:
	std::map<std::string, int> m_UniformMap;

public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void Bind();

	void SetBool(const std::string& name, bool v0);
	void SetInt(const std::string& name, int v0);
	void SetFloat(const std::string& name, float v0);
	void Set4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	std::string ReadShader(const char* shaderPath);
	void CheckAndLogShaderErrors(unsigned int id);
	void CheckAndLogProgramErrors(unsigned int id);
	unsigned int CompileShader(const char* shaderCode, unsigned int shaderType);
	GLint GetUniformLocation(const std::string& name);
};