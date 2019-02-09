#include "AssetManager.h"
#include "Utils.h"

AAssetManager* AssetManager::aassetManager = nullptr;

AssetManager::AssetManager(AAssetManager * aassetManager) : ressourceCache(), timeOfInsertCache()
{
	this->aassetManager = aassetManager;
}

bool AssetManager::unloadNotUsedRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
		res->second.release();
		ressourceCache.erase(res);
		return true;
	}
	else
		return false;
}

void AssetManager::clear()
{
	ressourceCache.clear();
}

bool AssetManager::isLoaded(const std::string & filename)
{
	auto i = ressourceCache.find(filename);
	return i != ressourceCache.end();
}

void AssetManager::reloadAllRes()
{
	for (auto it = ressourceCache.begin(); it != ressourceCache.end(); ++it)
	{
		AssetLoader assetLoader = assetLoaderCache.at(it->first.substr(it->first.find_last_of('.') + 1));

		if(!assetLoader.reloadFromFile(it->second.get(), it->first))
		{
			utils::logBreak("Could not reload asset!");
		}
	}
}

void AssetManager::read(AAsset * asset, void * buffer, size_t size)
{
	assert(AAsset_read(asset, buffer, size) == size);
}

void AssetManager::registerAssetLoader(const std::string & fileExt, const AssetLoader & assetLoader)
{
	assert(assetLoaderCache.find(fileExt) == assetLoaderCache.end());

	assetLoaderCache.emplace(std::make_pair(fileExt, assetLoader));
}
