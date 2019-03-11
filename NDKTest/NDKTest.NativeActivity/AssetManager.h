#pragma once

#include <memory>
#include <unordered_map>
#include "AssetLoader.h"
#include "Utils.h"
#include "android_native_app_glue.h"
#include "Sound.h"
#include "Texture.h"

class AssetManager
{
	friend class RenderWindow;
	friend class Ifstream;

	static constexpr long long maxSize = Gigabyte(1);
	long long currentSize = 0;
	std::unordered_map<std::string, std::unique_ptr<char[]>> ressourceCache;
	Vector<std::string> timeOfInsertCache;
	static AAssetManager* aassetManager;
	std::unordered_map<std::string, AssetLoader> assetLoaderCache;
private:
	AssetManager(AAssetManager* aassetManager);
public:
	AssetManager() = delete;
	template <typename T>
	T* getOrAddRes(const std::string& filename);
	bool unloadNotUsedRes(const std::string& filename);
	void clear();
	bool isLoaded(const std::string& filename);
	void reloadAllRes();
	void read(AAsset* asset, void * buffer, size_t size);
	void registerAssetLoader(const std::string& fileExt, const AssetLoader& assetLoader);
};

template<typename T>
T * AssetManager::getOrAddRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
		T* asset = (T*) res->second.get();
		return asset;
	}
	else
	{
		std::unique_ptr<char[]> asset = std::make_unique<char[]>(sizeof(T));
		T* tP = (T*) asset.get();
		*tP = T();
		std::string ext = filename.substr(filename.length() - 3);
		assert(assetLoaderCache.find(ext) != assetLoaderCache.end());
		AssetLoader assetLoader = assetLoaderCache.at(ext);

		//(TODO: Think about how to let the user also construct a asset from a stream)
		if (!assetLoader.loadFromFile(asset.get(), filename))
		{
			utils::logBreak("Could not load asset!");
			return nullptr;
		}

		currentSize += assetLoader.getSize(asset.get());
		if (currentSize > maxSize)
		{
			do
			{
				auto id = timeOfInsertCache.begin();
				auto it = ressourceCache.find(*id);
				assert(it != ressourceCache.end());
				AssetLoader aL = assetLoaderCache.at(it->first.substr(it->first.length() - 3));
				currentSize -= aL.getSize(it->second.get());
				it->second.release();
				ressourceCache.erase(it);
				timeOfInsertCache.erase(id);
			} while (currentSize > maxSize);
		}

		auto result = ressourceCache.emplace(std::make_pair(filename, std::move(asset)));
		timeOfInsertCache.push_back(filename);
		assert(result.second);
		T* returnAsset = (T*) result.first->second.get();
		return returnAsset;
	}
}