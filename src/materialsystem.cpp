#include <plog/Log.h>
#include "materialsystem.h"

Material::Material(ShaderProgramPtr _shaderProgram):
	shaderProgram(_shaderProgram)
{
}

Material::~Material()
{
}

bool Material::IsValid()
{
	return shaderProgram->IsValid();
}

bool Material::Bind()
{
	return shaderProgram->Use();
}


MaterialSystem::MaterialSystem()
{
}

MaterialSystem::~MaterialSystem()
{
}

MaterialSystem& MaterialSystem::GetInstance()
{
	static MaterialSystem instance;
	return instance;
}

bool MaterialSystem::HasShader(const std::string& name)
{
	auto it = shaderPrograms.find(name);
	return (it != shaderPrograms.end());
}

bool MaterialSystem::AddShader(const std::string& name, std::string& vtxSource, std::string& fragSrc)
{
	PLOGI << "Adding shader '" << name << "'";

	auto vertexShader = std::make_unique<Shader>(GL_VERTEX_SHADER, vtxSource);
	if (!vertexShader->IsValid())
	{
		return false;
	}

	auto fragmentShader = std::make_unique<Shader>(GL_FRAGMENT_SHADER, fragSrc);
	if (!fragmentShader->IsValid())
	{
		return false;
	}

	auto shaderProgram = std::make_shared<ShaderProgram>(std::move(vertexShader), std::move(fragmentShader));
	if (!shaderProgram->IsValid())
	{
		return false;
	}

	shaderPrograms[name] = shaderProgram;

	return true;
}

std::optional<ShaderProgramPtr> MaterialSystem::GetShader(const std::string& name)
{
	auto it = shaderPrograms.find(name); 
	if (it == shaderPrograms.end())
	{
		return std::nullopt;
	}

	return (*it).second;
}

bool MaterialSystem::HasMaterial(const std::string& name)
{
	auto it = materials.find(name);
	return (it != materials.end());
}

bool MaterialSystem::AddMaterial(const std::string& name, const std::string& shaderName)
{
	PLOGI << "Adding material '" << name << "'";

	if (HasMaterial(name))
	{
		return false;
	}

	auto shader = GetShader(shaderName);
	if (!shader.has_value())
	{
		return false;
	}

	auto mat = std::make_shared<Material>(shader.value());
	materials[name] = mat;

	return true;
}

std::optional<MaterialPtr> MaterialSystem::GetMaterial(const std::string& name)
{
	auto it = materials.find(name); 
	if (it == materials.end())
	{
		return std::nullopt;
	}

	return (*it).second;
}