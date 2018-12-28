#include "AssetManager.h"
#include "Utils.h"

TextureAssetManager::TextureAssetManager(AAssetManager * aassetManager) : ressourceCache(), timeOfInsertCache(), aassetManager(aassetManager)
{
}

Texture* TextureAssetManager::getOrAddRes(const std::string & filename)
{
	auto res = ressourceCache.find(filename);
	if (res != ressourceCache.end())
		return res->second.get();
	else
	{
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();
		if(!texture->loadFromFile(filename, aassetManager))
		{
			utilsLogBreak("Could not load texture!");
			return nullptr;
		}

		currentSize += texture->getSize().x * texture->getSize().y * sizeof(int);
		if (currentSize > maxSize)
		{
			do
			{
				auto id = timeOfInsertCache.begin();
				auto it = ressourceCache.find(*id);
				assert(it != ressourceCache.end());
				currentSize -= it->second->getSize().x * it->second->getSize().y * sizeof(int);
				ressourceCache.erase(it);
				timeOfInsertCache.erase(id);
			} while (currentSize > maxSize);
		}

		auto result = ressourceCache.emplace(std::pair<std::string, std::unique_ptr<Texture>>{ filename, std::move(texture) });
		timeOfInsertCache.push_back(filename);
		assert(result.second);
		return result.first->second.get();
	}
}

bool TextureAssetManager::unloadNotUsedRes(const std::string & filename)
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

void TextureAssetManager::clear()
{
	ressourceCache.clear();
}

bool TextureAssetManager::isLoaded(const std::string & filename)
{
	auto i = ressourceCache.find(filename);
	return i != ressourceCache.end();
}

AAssetManager * TextureAssetManager::getAAssetManager()
{
	return aassetManager;
}

void TextureAssetManager::reloadAllRes()
{
	for (auto it = ressourceCache.begin(); it != ressourceCache.end(); ++it)
	{
		if(!it->second->reloadFromFile(it->first, aassetManager))
		{
			utilsLogBreak("Could not reload texture!");
		}
	}
}
