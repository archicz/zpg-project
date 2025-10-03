#include <plog/Log.h>
#include "assetmanager.h"

AssetManager::AssetManager()
{
	Mount("base", std::make_shared<FileAssetProvider>());
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

	auto loader = it->second;
	auto asset = loader->Load(uri);

	if (!asset.has_value())
	{
		return std::nullopt;
	}

    auto assetPtr = asset.value();
    assetPtr->meta.type = type;
    assetPtr->meta.uri  = uri;
	
	return assetPtr;
}

void AssetManager::Mount(const std::string& scheme, AssetProviderPtr provider)
{
	PLOGV << "Mounting asset provider for schema '" << scheme << "'";
	providers[scheme] = provider;
}

std::optional<AssetProviderPtr> AssetManager::ResolveProvider(const AssetURI& uri)
{
	auto scheme = uri.GetScheme();
	auto it = providers.find(scheme);

	if (it == providers.end())
	{
		PLOGE << "No provider found for scheme '" << scheme << "' in URI '" << uri << "'";
		return std::nullopt;
	}

	return it->second;
}

std::optional<std::stringstream> AssetManager::GetData(const AssetURI& uri)
{
	auto provider = ResolveProvider(uri);
	
	if (!provider.has_value())
	{
		return std::nullopt;
	}

	return (*provider)->GetData(uri);
}

std::optional<AssetStat> AssetManager::GetStat(const AssetURI& uri)
{
	auto provider = ResolveProvider(uri);
	
	if (!provider.has_value())
	{
		return std::nullopt;
	}

	return (*provider)->GetStat(uri);
}
