#include <fstream>
#include <plog/Log.h>
#include "assetprovider.h"

FileAssetProvider::FileAssetProvider(const std::filesystem::path& rootPath):
	root(rootPath)
{
}

std::optional<std::stringstream> FileAssetProvider::GetData(const AssetURI& uri)
{
	auto path = ResolvePath(uri);
	if (!path.has_value())
	{
		return std::nullopt;
	}

	std::ifstream file(*path);
	std::stringstream fileData;
	fileData << file.rdbuf();

	return fileData;
}

std::optional<AssetStat> FileAssetProvider::GetStat(const AssetURI& uri)
{
	auto path = ResolvePath(uri);
	if (!path.has_value())
	{
		return std::nullopt;
	}

	AssetStat stat;
	stat.source = (*path).u8string();
	stat.modifiedTime = std::filesystem::last_write_time(*path);
	stat.size = std::filesystem::file_size(*path);

	return stat;
}

std::optional<std::filesystem::path> FileAssetProvider::ResolvePath(const AssetURI& uri) const
{
	std::filesystem::path uriPath(uri.GetPath());
	std::filesystem::path finalPath = root.empty() ? uriPath : (root / uriPath);

	if (std::filesystem::exists(finalPath))
	{
		return finalPath;
	}

	PLOGE << "Can't resolve file path '" << finalPath.u8string() << "' for URI '" << uri << "'";
	return std::nullopt;
}
