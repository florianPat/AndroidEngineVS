#pragma once

#include <string>

class Asset
{
public:
	//TODO: Do this better with compile time guid!
	enum class AssetId
	{
		TEXTURE,
		SOUND
	};
	const AssetId assetId;
protected:
	friend class AssetManager;

	virtual bool loadFromFile(const std::string& filename, AAssetManager* assetManager) = 0;
	virtual bool reloadFromFile(const std::string& filename, AAssetManager* assetManager) = 0;
	void read(AAsset* asset, void * buffer, size_t size);
	Asset(AssetId assetId);
public:
	Asset() = delete;
	virtual long long getSize() const = 0;
};