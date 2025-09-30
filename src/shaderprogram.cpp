#include <iostream>
#include <sstream>
#include <fstream>
#include <plog/Log.h>
#include "shaderprogram.h"

ShaderProgram::ShaderProgram(ShaderPtr vtxShader, ShaderPtr fragShader):
	vertexShader(std::move(vtxShader)), fragmentShader(std::move(fragShader))
{
	if (vertexShader->IsValid() && fragmentShader->IsValid())
	{
		programId = glCreateProgram();
		glAttachShader(programId, vertexShader->shaderId);
		glAttachShader(programId, fragmentShader->shaderId);

		glLinkProgram(programId);
		glDetachShader(programId, vertexShader->shaderId);
		glDetachShader(programId, fragmentShader->shaderId);

		GLint status;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);

			PLOGE << "Shader program link failed, " << std::string(infoLog.begin(), infoLog.end()) << '\n';
			Destroy();
		}
	}
}

ShaderProgram::~ShaderProgram()
{
	Destroy();
}

void ShaderProgram::Destroy()
{
	if (IsValid())
	{
		glDeleteProgram(programId);
		programId = 0;
	}
}

bool ShaderProgram::IsValid() const
{
	return (programId != 0);
}

bool ShaderProgram::Use()
{
	if (IsValid())
	{
		glUseProgram(programId);
		return true;
	}

	return false;
}

GLint ShaderProgram::LookupUniform(const std::string& name)
{
	if (!IsValid())
	{
		return -1;
	}

	if (uniforms.find(name) == uniforms.end())
	{
		uniforms[name] = glGetUniformLocation(programId, name.c_str());
	}

	return uniforms[name];
}

void ShaderProgram::Set(const std::string& name, float value)
{
	GLint uniformLocation = LookupUniform(name);

	if (uniformLocation != -1)
	{
		glUniform1f(uniformLocation, value);
	}
}

void ShaderProgram::Set(const std::string& name, const glm::mat4& mat)
{
	GLint uniformLocation = LookupUniform(name);

	if (uniformLocation != -1)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat[0][0]);
	}
}
