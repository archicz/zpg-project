#include <fstream>
#include <vector>
#include <plog/Log.h>
#include "shader.h"
#include "assetmanager.h"

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


VertexShaderAsset::VertexShaderAsset(ShaderPtr shdr):
	shader(shdr)
{
}

VertexShaderAsset::~VertexShaderAsset()
{
}

VertexShaderAssetLoader::~VertexShaderAssetLoader()
{
}

std::optional<std::shared_ptr<IAsset>> VertexShaderAssetLoader::Load(const AssetURI& uri)
{
	PLOGV << "Loading vertex shader asset from URI '" << uri << "'";

	auto& am = AssetManager::GetInstance();
	auto path = am.ResolvePath(uri);
	
	if (!path.has_value())
	{
		return std::nullopt;
	}

	std::ifstream vtxSourceFile(path.value());

	if (!vtxSourceFile)
	{
		PLOGE << "Can't open vertex shader source at '" << path.value() << "'";
		return std::nullopt;
	}

	std::stringstream vtxSource;
	vtxSource << vtxSourceFile.rdbuf();

	ShaderPtr shader = std::make_shared<Shader>(GL_VERTEX_SHADER, vtxSource.str());
	if (!shader->IsValid())
	{
		return std::nullopt;
	}

	VertexShaderAssetPtr shaderAsset = std::make_shared<VertexShaderAsset>(shader);
	return std::dynamic_pointer_cast<IAsset>(shaderAsset);
}


FragmentShaderAsset::FragmentShaderAsset(ShaderPtr shdr):
	shader(shdr)
{
}

FragmentShaderAsset::~FragmentShaderAsset()
{
}

FragmentShaderAssetLoader::~FragmentShaderAssetLoader()
{
}

std::optional<std::shared_ptr<IAsset>> FragmentShaderAssetLoader::Load(const AssetURI &uri)
{
	PLOGV << "Loading fragment shader asset from URI '" << uri << "'";

	auto& am = AssetManager::GetInstance();
	auto path = am.ResolvePath(uri);
	
	if (!path.has_value())
	{
		return std::nullopt;
	}

	std::ifstream frgSourceFile(path.value());

	if (!frgSourceFile)
	{
		PLOGE << "Can't open fragment shader source at '" << path.value() << "'";
		return std::nullopt;
	}

	std::stringstream frgSource;
	frgSource << frgSourceFile.rdbuf();

	ShaderPtr shader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, frgSource.str());
	if (!shader->IsValid())
	{
		return std::nullopt;
	}

	FragmentShaderAssetPtr shaderAsset = std::make_shared<FragmentShaderAsset>(shader);
	return std::dynamic_pointer_cast<IAsset>(shaderAsset);
}
