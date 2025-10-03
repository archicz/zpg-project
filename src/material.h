#ifndef MATERIAL_H
#define MATERIAL_H
#pragma once

#include <nlohmann/json.hpp>
#include "shaderprogram.h"
#include "asset.h"

class Material
{
public:
	Material() = delete;
	Material(ShaderProgramAssetHandle shaderProgramHandle, nlohmann::json params);
	~Material();
public:
	void Destroy();
	bool IsValid() const;
	bool Use() const;
private:
	ShaderProgramAssetHandle shaderProgramHandle;
};

using MaterialPtr = std::shared_ptr<Material>;


class MaterialAsset : public IAsset
{
public:
	MaterialAsset() = delete;
	MaterialAsset(MaterialPtr material);
public:
	virtual ~MaterialAsset() override;
public:
	static constexpr AssetType GetType() { return AssetType::Material; }
public:
	MaterialPtr Get() const
	{
		return material;
	}
private:
	MaterialPtr material;
};

class MaterialAssetLoader : public IAssetLoader
{
public:
	virtual ~MaterialAssetLoader() override;
	virtual AssetType GetType() const override { return AssetType::Material; }
	virtual std::optional<std::shared_ptr<IAsset>> Load(const AssetURI& uri) override;
};

using MaterialAssetHandle = AssetHandle<MaterialAsset>;
using MaterialAssetPtr = std::shared_ptr<MaterialAsset>;

#endif