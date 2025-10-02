#ifndef ASSET_H
#define ASSET_H
#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <optional>

enum class AssetType
{
	Unknown = 0,
	ShaderProgram,
	VertexShader,
	FragmentShader,
	Material,
	MaterialDefinition,
	Texture,
	Mesh,
	Model,
	Scene,
	Max
};

std::ostream& operator<<(std::ostream& os, const AssetType& type);

class AssetURI
{
public:
	AssetURI();
	AssetURI(const std::string& uri);
	~AssetURI() = default;
public:
	operator std::string() const;
	const std::string& GetScheme() const;
	const std::string& GetPath() const;

	bool IsValid() const
	{ 
		return (!scheme.empty() && !path.empty()); 
	}
	
	bool operator==(const AssetURI& other) const
	{
		return (scheme == other.scheme && path == other.path);
	}
private:
	std::string scheme;
	std::string path;
};

namespace std
{
	template<>
	struct hash<AssetURI>
	{
		size_t operator()(const AssetURI& uri) const noexcept
		{
			return std::hash<std::string>()(static_cast<std::string>(uri));
		}
	};
}

struct AssetMetadata
{
	AssetType type = AssetType::Unknown;
	AssetURI uri;
	std::string resolvedPath;
	// uint64_t lastModified = 0;
};

class IAsset
{
public:
	friend class AssetManager;
public:
	virtual ~IAsset() = default;
public:
	static constexpr AssetType GetType()
	{
		return AssetType::Unknown;
	}
private:
	AssetMetadata meta;
};

using AssetHandleRaw = size_t;

template <typename TAsset>
class AssetHandle
{
public:
	AssetHandle():
		hash(0)
	{
	}

	AssetHandle(const std::string& uri)
	{
		hash = std::hash<std::string>()(uri);
	}

	~AssetHandle() = default;
public:
	AssetHandleRaw RawHandle() const
	{
		return hash;
	}
	
	bool operator==(const AssetHandle& other) const
	{
		return (hash == other.hash);
	}

	explicit operator bool() const
	{
		return (hash != 0);
	}
private:
	AssetHandleRaw hash;
};

namespace std
{
	template <typename TAsset>
	struct hash<AssetHandle<TAsset>>
	{
		size_t operator()(const AssetHandle<TAsset>& h) const noexcept
		{
			return h.RawHandle();
		}
	};
}

class IAssetLoader
{
public:
	virtual ~IAssetLoader() = default;
	virtual AssetType GetType() const = 0;
	virtual std::optional<std::shared_ptr<IAsset>> Load(const AssetURI& uri) = 0;
};

using AssetPtr = std::shared_ptr<IAsset>;
using AssetLoaderPtr = std::shared_ptr<IAssetLoader>;

#endif