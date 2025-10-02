#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <optional>
#include <memory>
#include <string>
#include <glad/glad.h>
#include "asset.h"

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

using ShaderPtr = std::shared_ptr<Shader>;


class VertexShaderAsset : public IAsset
{
public:
	VertexShaderAsset() = delete;
	VertexShaderAsset(ShaderPtr shader);
public:
	virtual ~VertexShaderAsset() override;
public:
	static constexpr AssetType GetType() { return AssetType::VertexShader; }
public:
	ShaderPtr Get() const
	{
		return shader;
	}
private:
	ShaderPtr shader;
};

class VertexShaderAssetLoader : public IAssetLoader
{
public:
	virtual ~VertexShaderAssetLoader() override;
	virtual AssetType GetType() const override { return AssetType::VertexShader; }
	virtual std::optional<std::shared_ptr<IAsset>> Load(const AssetURI& uri) override;
};

class FragmentShaderAsset : public IAsset
{
public:
	FragmentShaderAsset() = delete;
	FragmentShaderAsset(ShaderPtr shader);
public:
	virtual ~FragmentShaderAsset() override;
public:
	static constexpr AssetType GetType() { return AssetType::FragmentShader; }
public:
	ShaderPtr Get() const
	{
		return shader;
	}
private:
	ShaderPtr shader;
};

class FragmentShaderAssetLoader : public IAssetLoader
{
public:
	virtual ~FragmentShaderAssetLoader() override;
	virtual AssetType GetType() const override { return AssetType::FragmentShader; }
	virtual std::optional<std::shared_ptr<IAsset>> Load(const AssetURI& uri) override;
};

using VertexShaderAssetPtr = std::shared_ptr<VertexShaderAsset>;
using FragmentShaderAssetPtr = std::shared_ptr<FragmentShaderAsset>;

#endif