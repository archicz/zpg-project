#include <plog/Log.h>
#include "assetmanager.h"

AssetManager::AssetManager()
{
	Mount("base", "");
}

AssetManager::~AssetManager()
{
}

AssetManager& AssetManager::GetInstance()
{
	static AssetManager instance;
	return instance;
}

std::optional<std::shared_ptr<IAsset>> AssetManager::Load(const AssetURI& uri, const AssetType type)
{
	PLOGV << "Loading asset from URI '" << uri << "', asset type: '" << type << "'";

    if (!uri.IsValid())
	{
        PLOGE << "Invalid URI '" << uri << "'";
        return std::nullopt;
    }

	auto it = loaders.find(type);
	if (it == loaders.end())
	{
		PLOGE << "Can't find asset loader for asset type '" << type << "'";
		return std::nullopt;
	}

	auto resolvedPath = ResolvePath(uri);
	if (!resolvedPath.has_value())
	{
		return std::nullopt;
	}

	auto loader = it->second;
	auto asset = loader->Load(uri);

	if (!asset.has_value())
	{
		return std::nullopt;
	}

    auto assetPtr = asset.value();
    assetPtr->meta.type = type;
    assetPtr->meta.uri  = uri;
    assetPtr->meta.resolvedPath = resolvedPath.value_or("");
	
	return assetPtr;
}

void AssetManager::Mount(const std::string& scheme, const std::string& path)
{
	mounts[scheme] = path;
}

std::optional<std::string> AssetManager::ResolvePath(const AssetURI& uri)
{
	auto scheme = uri.GetScheme();
	auto it = mounts.find(scheme);
	if (it == mounts.end())
	{
		PLOGE << "Could not resolve path from URI '" << uri << "'";
		return std::nullopt;
	}

	auto schemeRoot = it->second;
	return (schemeRoot.empty() ? "" : (schemeRoot + "/")) + uri.GetPath();
}
