#ifndef MODEL_H
#define MODEL_H
#pragma once

#include "asset.h"
#include "material.h"
#include "mesh.h"

struct ModelMeshPart
{
	size_t partIndex;
	MaterialAssetHandle material;
};

class Model
{
public:
	Model() = delete;
	Model(MeshPtr mesh, const std::vector<ModelMeshPart>& meshParts);
	~Model();
private:
	MeshPtr mesh;
	std::vector<ModelMeshPart> meshParts;
};

using ModelPtr = std::shared_ptr<Model>;


class ModelAsset : public IAsset
{
public:
	ModelAsset() = delete;
	ModelAsset(ModelPtr model);
public:
	virtual ~ModelAsset() override;
public:
	static constexpr AssetType GetType() { return AssetType::Model; }
public:
	ModelPtr Get() const
	{
		return model;
	}
private:
	ModelPtr model;
};

class ModelAssetLoader : public IAssetLoader
{
public:
	virtual ~ModelAssetLoader() override;
	virtual AssetType GetType() const override { return AssetType::Model; }
	virtual std::optional<std::shared_ptr<IAsset>> Load(const AssetURI& uri) override;
};

using ModelAssetHandle = AssetHandle<ModelAsset>;
using ModelAssetPtr = std::shared_ptr<ModelAsset>;

#endif