#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <string>
#include <glad/glad.h>

// forward declaration
class ShaderProgram;

class Shader
{
public:
	friend class ShaderProgram;
public:
	Shader(GLenum kind, const std::string& source);
	~Shader();
public:
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	
	Shader(Shader&& other) noexcept:
		shaderKind(other.shaderKind)
	{
		std::swap(shaderId, other.shaderId);
	}

	Shader& operator=(Shader&& other) noexcept
	{
		if (this != &other) 
		{
			shaderKind = other.shaderKind;
			std::swap(shaderId, other.shaderId);
		} 
		
		return *this; 
	}
public:
	void Destroy();
	bool IsValid() const;
protected:
	GLenum shaderKind = GL_VERTEX_SHADER;
	GLuint shaderId = 0;
};

#endif