#include <plog/Log.h>
#include "material.h"
#include "assetmanager.h"

using nlohmann::json;

Material::Material(ShaderProgramAssetHandle handle, nlohmann::json params):
	shaderProgramHandle(handle)
{
}

Material::~Material()
{
}

void Material::Destroy()
{
}

bool Material::IsValid() const
{
	auto& am = AssetManager::GetInstance();
	return am.IsValid(shaderProgramHandle);
}

bool Material::Use() const
{
	if (!IsValid())
	{
		return false;
	}

	auto& am = AssetManager::GetInstance();
	auto shaderProgramAsset = am.Get(shaderProgramHandle);
	auto shaderProgram = (*shaderProgramAsset)->Get();
	
	return shaderProgram->Use();
}


MaterialAsset::MaterialAsset(MaterialPtr mat):
	material(mat)
{
}

MaterialAsset::~MaterialAsset()
{
}

MaterialAssetLoader::~MaterialAssetLoader()
{
}

std::optional<std::shared_ptr<IAsset>> MaterialAssetLoader::Load(const AssetURI& uri)
{
	PLOGV << "Loading material asset from URI '" << uri << "'";

	auto& am = AssetManager::GetInstance();
	auto data = am.GetData(uri);
	
	if (!data.has_value())
	{
		return std::nullopt;
	}

	json materialRecipe = json::parse(data.value());

	if (!materialRecipe.contains("shaderProgram"))
	{
		PLOGE << "Material recipe '" << uri << "' is missing shader program URI";
		return std::nullopt;
	}

	AssetURI shaderProgramURI(materialRecipe["shaderProgram"]);
	auto shaderProgramHandle = am.Require<ShaderProgramAsset>(shaderProgramURI);
	if (!shaderProgramHandle)
	{
		return std::nullopt;
	}

	MaterialPtr material = std::make_shared<Material>(shaderProgramHandle, materialRecipe["params"]);
	if (!material->IsValid())
	{
		return std::nullopt;
	}

	PLOGV << "Material asset from URI '" << uri << "' OK";

	MaterialAssetPtr materialAsset = std::make_shared<MaterialAsset>(material);
	return std::dynamic_pointer_cast<IAsset>(materialAsset);
}