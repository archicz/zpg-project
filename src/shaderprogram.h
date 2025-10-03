#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include "shader.h"
#include "asset.h"

class ShaderProgram
{
public:
	ShaderProgram(ShaderPtr vertexShader, ShaderPtr fragmentShader);
	~ShaderProgram();
public:
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&& other) noexcept = delete;
	Shader& operator=(Shader&& other) noexcept = delete;
public:
	void Destroy();
	bool IsValid() const;
	bool Use() const;
private:
	GLint LookupUniform(const std::string& name);
public:
	void Set(const std::string& name, float value);
	void Set(const std::string& name, const glm::mat4& mat);
private:
	GLuint programId = 0;
	std::unordered_map<std::string, GLint> uniforms;
};

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;


class ShaderProgramAsset : public IAsset
{
public:
	ShaderProgramAsset() = delete;
	ShaderProgramAsset(ShaderProgramPtr shaderProgram);
public:
	virtual ~ShaderProgramAsset() override;
public:
	static constexpr AssetType GetType() { return AssetType::ShaderProgram; }
public:
	ShaderProgramPtr Get() const
	{
		return shaderProgram;
	}
private:
	ShaderProgramPtr shaderProgram;
};

class ShaderProgramAssetLoader : public IAssetLoader
{
public:
	virtual ~ShaderProgramAssetLoader() override;
	virtual AssetType GetType() const override { return AssetType::ShaderProgram; }
	virtual std::optional<std::shared_ptr<IAsset>> Load(const AssetURI& uri) override;
};

using ShaderProgramAssetHandle = AssetHandle<ShaderProgramAsset>;
using ShaderProgramAssetPtr = std::shared_ptr<ShaderProgramAsset>;

#endif