#pragma once

#include <memory>
#include <unordered_map>
#include "Asset.h"
#include "Utils.h"
#include "android_native_app_glue.h"
#include "Sound.h"
#include "Texture.h"

class AssetManager
{
	static constexpr long long maxSize = Gigabyte(4);
	long long currentSize = 0;
	std::unordered_map<std::string, std::unique_ptr<Asset>> ressourceCache;
	std::vector<std::string> timeOfInsertCache;
	AAssetManager* aassetManager = nullptr;
public:
	AssetManager(AAssetManager* aassetManager);
	template <typename T>
	T* getOrAddRes(const std::string& filename);
	bool unloadNotUsedRes(const std::string& filename);
	void clear();
	bool isLoaded(const std::string& filename);
	AAssetManager* getAAssetManager();
	void reloadAllRes();
private:
	template<typename T>
	T* returnRightValue(Asset* asset);
};

template<typename T>
T * AssetManager::getOrAddRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
		Asset* asset = res->second.get();
		return returnRightValue<T>(asset);
	}
	else
	{
		std::unique_ptr<Asset> texture = nullptr;
		texture = std::make_unique<T>();
		//NOTE: Only used to really verify that it is ok what I am doing. RTTI should be switched off in release mode
#ifndef NDEBUG
		assert(dynamic_cast<Asset*>(texture.get()) != nullptr);
#endif
		if (!texture->loadFromFile(filename, aassetManager))
		{
			utilsLogBreak("Could not load texture!");
			return nullptr;
		}

		currentSize += texture->getSize();
		if (currentSize > maxSize)
		{
			do
			{
				auto id = timeOfInsertCache.begin();
				auto it = ressourceCache.find(*id);
				assert(it != ressourceCache.end());
				currentSize -= it->second->getSize();
				ressourceCache.erase(it);
				timeOfInsertCache.erase(id);
			} while (currentSize > maxSize);
		}

		auto result = ressourceCache.emplace(std::pair<std::string, std::unique_ptr<Asset>>{ filename, std::move(texture) });
		timeOfInsertCache.push_back(filename);
		assert(result.second);
		Asset* asset = result.first->second.get();
		return returnRightValue<T>(asset);
	}
}

template<typename T>
T * AssetManager::returnRightValue(Asset * asset)
{
	switch (asset->assetId)
	{
		case Asset::AssetId::TEXTURE:
		{
			Texture* result = (Texture*)asset;
			assert(typeid(result) == typeid(T*));
			break;
		}
		case Asset::AssetId::SOUND:
		{
			Sound* result = (Sound*)asset;
			assert(typeid(result) == typeid(T*));
			break;
		}
		default:
		{
			utilsLogBreak("AssetId");
			return nullptr;
		}
	}
	//NOTE: Only used to really verify that it is ok what I am doing. RTTI should be switched off in release mode
#ifndef NDEBUG
	assert(dynamic_cast<T*>(asset) != nullptr);
#endif
	return (T*)asset;
}
