#include <iostream>
#include <vector>
#include <plog/Log.h>
#include "shader.h"

Shader::Shader(GLenum kind, const std::string& source):
	shaderKind(kind)
{
	const GLchar* sourcePtr = source.c_str();

	shaderId = glCreateShader(kind);
	glShaderSource(shaderId, 1, &sourcePtr, 0);
	glCompileShader(shaderId);

	GLint status = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

		PLOGE << "Shader compilation failed, " << std::string(infoLog.begin(), infoLog.end()) << '\n';
		Destroy();
	}
}

Shader::~Shader()
{
	Destroy();
}

void Shader::Destroy()
{
	if (IsValid())
	{
		glDeleteShader(shaderId);
	}

	shaderId = 0;
}

bool Shader::IsValid() const
{
	return (shaderId != 0);
}
