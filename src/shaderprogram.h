#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "shader.h"

class ShaderProgram
{
public:
	using ShaderPtr = std::unique_ptr<Shader>;
public:
	ShaderProgram(ShaderPtr vertexShader, ShaderPtr fragmentShader);
	~ShaderProgram();
public:
	void Destroy();
	bool IsValid() const;
	bool Use();
private:
	GLint LookupUniform(const std::string& name);
public:
	void Set(const std::string& name, float value);
	void Set(const std::string& name, const glm::mat4& mat);
private:
	ShaderPtr vertexShader;
	ShaderPtr fragmentShader;
	GLuint programId = 0;
	std::unordered_map<std::string, GLint> uniforms;
};

#endif