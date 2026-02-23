#pragma once

#include "glad/glad.h"

#include <iostream>

struct ShaderProgramSource {
	std::string vertex;
	std::string fragment;
};

class Shader
{
private:
	unsigned int ID;
public:
	Shader(const char* shaderPath);
	~Shader();

	void Bind() const;
	void Unbind() const;
private:
	ShaderProgramSource ParseShader(const char* shaderPath) const;
	void AddShaders(const ShaderProgramSource src) const;
	unsigned int CompileShader(GLenum type, std::string src) const;
	void LinkProgram(const unsigned int vertexShader, unsigned int fragmentShader) const;

public:
	int GetUniformLocation(const char* name) const;
	void U1i(const char* name, const int v1) const;
	void U1f(const char* name, const float v1) const;
	void U3f(const char* name, const float v1, const float v2, const float v3) const;
	void U3fv(const char* name, const float* v1) const;
	void U4f(const char* name, const float v1, const float v2, const float v3, const float v4) const;
	void U4fv(const char* name, const float* v1) const;
	void UMat4fv(const char* name, const float* v1, const bool transpose = false) const;
};