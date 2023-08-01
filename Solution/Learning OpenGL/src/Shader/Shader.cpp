#include "Shader.h"



Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	unsigned int vertexShader = CompileShader(ReadShader(vertexPath).c_str(), GL_VERTEX_SHADER);
	unsigned int fragmentShader = CompileShader(ReadShader(fragmentPath).c_str(), GL_FRAGMENT_SHADER);

	m_ID = glCreateProgram();
	GLCall(glAttachShader(m_ID, vertexShader));
	GLCall(glAttachShader(m_ID, fragmentShader));
	GLCall(glLinkProgram(m_ID));

	CheckAndLogProgramErrors(m_ID);

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));
}

void Shader::Bind()
{
	GLCall(glUseProgram(m_ID));
}

void Shader::SetBool(const std::string& name, bool v0)
{
	GLCall(glUniform1i(GetUniformLocation(name), (int)v0));
}

void Shader::SetInt(const std::string& name, int v0)
{
	GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetFloat(const std::string& name, float v0)
{
	GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::Set4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetMatrix4(const std::string& name, glm::mat4& v0)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v0)));
}


std::string Shader::ReadShader(const char* shaderPath)
{	
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open a the file 
		shaderFile.open(shaderPath);

		// Create a datastream
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "SHADER COMPILE ERROR: File not read succesfully. FileName: " << shaderPath << std::endl;
	}

	return shaderCode;
}

unsigned int Shader::CompileShader(const char* shaderCode, unsigned int shaderType) {
	unsigned int shader = glCreateShader(shaderType);
	GLCall(glShaderSource(shader, 1, &shaderCode, NULL));
	GLCall(glCompileShader(shader));

	CheckAndLogShaderErrors(shader);
	return shader;
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformMap.find(name) == m_UniformMap.end()) {
		m_UniformMap.insert({ name, glGetUniformLocation(m_ID, name.c_str()) });
	}

	return m_UniformMap[name];
}

void Shader::CheckAndLogShaderErrors(unsigned int id) {
	int success;
	char infoLog[512];
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));

	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "[SHADER COMPILE ERROR]: " << infoLog << std::endl;
	}
}

void Shader::CheckAndLogProgramErrors(unsigned int id) {
	int success;
	char infoLog[512];
	GLCall(glGetProgramiv(id, GL_LINK_STATUS, &success));

	if (!success) {
		GLCall(glGetProgramInfoLog(id, 512, NULL, infoLog));
		std::cout << "[SHADER COMPILE ERROR]: " << infoLog << std::endl;
	}
}


