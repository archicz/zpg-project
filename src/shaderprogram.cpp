#include <iostream>
#include <sstream>
#include <fstream>
#include <plog/Log.h>
#include <nlohmann/json.hpp>
#include "shaderprogram.h"
#include "assetmanager.h"

using nlohmann::json;

ShaderProgram::ShaderProgram(ShaderPtr vertexShader, ShaderPtr fragmentShader)
{
	if (!vertexShader->IsValid() || !fragmentShader->IsValid())
	{
		return;
	}

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


ShaderProgramAsset::ShaderProgramAsset(ShaderProgramPtr prg):
	shaderProgram(prg)
{
}

ShaderProgramAsset::~ShaderProgramAsset()
{
}

ShaderProgramAssetLoader::~ShaderProgramAssetLoader()
{
}

std::optional<std::shared_ptr<IAsset>> ShaderProgramAssetLoader::Load(const AssetURI& uri)
{
	PLOGV << "Loading shader program asset from URI '" << uri << "'";

	auto& am = AssetManager::GetInstance();
	auto data = am.GetData(uri);
	
	if (!data.has_value())
	{
		return std::nullopt;
	}

	json programRecipe = json::parse(data.value());

	if (!programRecipe.contains("vertexShader"))
	{
		PLOGE << "Shader program recipe '" << uri << "' is missing vertex shader URI";
		return std::nullopt;
	}

	if (!programRecipe.contains("fragmentShader"))
	{
		PLOGE << "Shader program recipe '" << uri << "' is missing fragment shader URI";
		return std::nullopt;
	}

	AssetURI vertexShaderURI(programRecipe["vertexShader"]);
	auto vtxShaderHandle = am.Require<VertexShaderAsset>(vertexShaderURI);
	if (!vtxShaderHandle)
	{
		return std::nullopt;
	}

	AssetURI fragmentShaderURI(programRecipe["fragmentShader"]);
	auto fragShaderHandle = am.Require<FragmentShaderAsset>(fragmentShaderURI);
	if (!fragShaderHandle)
	{
		return std::nullopt;
	}

	ShaderPtr vtxShader = am.Get<VertexShaderAsset>(vtxShaderHandle).value()->Get();
	ShaderPtr fragShader = am.Get<FragmentShaderAsset>(fragShaderHandle).value()->Get();

	ShaderProgramPtr shaderProgram = std::make_shared<ShaderProgram>(vtxShader, fragShader);
	if (!shaderProgram->IsValid())
	{
		return std::nullopt;
	}

	PLOGV << "Shader program asset from URI '" << uri << "' OK";

	ShaderProgramAssetPtr shaderProgramAsset = std::make_shared<ShaderProgramAsset>(shaderProgram);
	return std::dynamic_pointer_cast<IAsset>(shaderProgramAsset);
}
