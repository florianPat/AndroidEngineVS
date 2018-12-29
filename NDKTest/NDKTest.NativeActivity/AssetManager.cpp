#include "AssetManager.h"
#include "Utils.h"

AssetManager::AssetManager(AAssetManager * aassetManager) : ressourceCache(), timeOfInsertCache(), aassetManager(aassetManager)
{
}

bool AssetManager::unloadNotUsedRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
	{
		//NOTE: I think I do not have to do that
		//res->second.reset();
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

AAssetManager * AssetManager::getAAssetManager()
{
	return aassetManager;
}

void AssetManager::reloadAllRes()
{
	for (auto it = ressourceCache.begin(); it != ressourceCache.end(); ++it)
	{
		if(!it->second->reloadFromFile(it->first, aassetManager))
		{
			utilsLogBreak("Could not reload texture!");
		}
	}
}
