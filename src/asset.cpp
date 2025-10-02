#include "asset.h"

std::ostream& operator<<(std::ostream& os, const AssetType& type)
{
   os << static_cast<std::underlying_type<AssetType>::type>(type);
   return os;
}

AssetURI::AssetURI():
	scheme(), path()
{
}

AssetURI::AssetURI(const std::string &uri)
{
	size_t pos = uri.find("://");

	if (pos != std::string::npos)
	{
		scheme = uri.substr(0, pos);
		path = uri.substr(pos + 3);
	}
}

AssetURI::operator std::string() const
{
	return scheme + "://" + path;
}

const std::string& AssetURI::GetScheme() const
{
	return scheme;
}

const std::string& AssetURI::GetPath() const
{
	return path;
}