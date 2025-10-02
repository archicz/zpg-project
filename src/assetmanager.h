#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <optional>
#include "asset.h"

class AssetManager
{
private:
	AssetManager();
	~AssetManager();
public:
	static AssetManager& GetInstance();
public:
	template <typename TAssetLoader>
	void RegisterLoader()
	{
		static_assert(std::is_base_of<IAssetLoader, TAssetLoader>::value, "TAssetLoader is not based on IAssetLoader");

		auto loader = std::make_shared<TAssetLoader>();
		auto baseLoader = std::static_pointer_cast<IAssetLoader>(loader);

		const AssetType type = baseLoader->GetType();
		PLOGV << "Registering asset loader for asset type '" << type << "'";
		
		loaders[type] = std::move(baseLoader);
	}
private:
	std::optional<AssetPtr> Load(const AssetURI& uri, const AssetType type);

	template <typename TAsset>
	std::optional<AssetPtr> Load(const AssetURI& uri)
	{
		static_assert(std::is_base_of<IAsset, TAsset>::value, "TAsset is not based on IAsset");

		auto base = Load(uri, TAsset::GetType());
		if (!base.has_value())
		{
			return std::nullopt;
		}

		auto typed = std::dynamic_pointer_cast<TAsset>(base.value());
		if (!typed)
		{
			return std::nullopt;
		}

		return typed;
	}
public:
	void Mount(const std::string& scheme, const std::string& path);
	std::optional<std::string> ResolvePath(const AssetURI& uri);
public:
	template <typename TAsset>
	AssetHandle<TAsset> Require(const AssetURI& uri)
	{
		static_assert(std::is_base_of<IAsset, TAsset>::value, "TAsset is not based on IAsset");

		auto assetHandle = AssetHandle<TAsset>(uri);
		auto rawHandle = assetHandle.RawHandle();

		auto it = assets.find(rawHandle);
		if (it != assets.end())
		{
			return assetHandle;
		}

		auto type = TAsset::GetType();
		auto asset = Load(uri, type);

		if (asset.has_value())
		{
			assets[rawHandle] = asset.value();
			uriHandle[uri] = rawHandle;
			handleUri[rawHandle] = uri;
		}

		return assetHandle;
	}

	template <typename TAsset>
	std::optional<std::shared_ptr<TAsset>> Get(AssetHandle<TAsset> assetHandle)
	{
		auto rawHandle = assetHandle.RawHandle();
		auto it = assets.find(rawHandle);

		if (it == assets.end())
		{
			return std::nullopt;
		}

		return std::dynamic_pointer_cast<TAsset>(it->second);
	}
private:
	std::unordered_map<AssetHandleRaw, AssetPtr> assets;
	std::unordered_map<AssetURI, AssetHandleRaw> uriHandle;
	std::unordered_map<AssetHandleRaw, AssetURI> handleUri;
	std::unordered_map<std::string, std::string> mounts;
	std::unordered_map<AssetType, AssetLoaderPtr> loaders; 
};

#endif