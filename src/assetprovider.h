#ifndef ASSETPROVIDER_H
#define ASSETPROVIDER_H
#pragma once

#include <string>
#include <optional>
#include <sstream>
#include <filesystem>
#include "asset.h"

struct AssetStat
{
	std::string source;
	std::filesystem::file_time_type modifiedTime;
	size_t size;
};

class IAssetProvider
{
public:
	virtual std::optional<std::stringstream> GetData(const AssetURI& uri) = 0;
	virtual std::optional<AssetStat> GetStat(const AssetURI& uri) = 0;
};

using AssetProviderPtr = std::shared_ptr<IAssetProvider>;


class FileAssetProvider : public IAssetProvider
{
public:
	FileAssetProvider() = default;
	FileAssetProvider(const std::filesystem::path& rootPath);
public:
	virtual std::optional<std::stringstream> GetData(const AssetURI& uri) override;
	virtual std::optional<AssetStat> GetStat(const AssetURI& uri) override;
private:
	std::optional<std::filesystem::path> ResolvePath(const AssetURI& uri) const;
private:
	std::filesystem::path root;
};

using FileAssetProviderPtr = std::shared_ptr<FileAssetProvider>;

#endif