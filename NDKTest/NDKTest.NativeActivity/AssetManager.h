#pragma once

#include <memory>
#include <unordered_map>
#include "Texture.h"
#include "Utils.h"
#include "android_native_app_glue.h"

class TextureAssetManager
{
	static constexpr long long maxSize = Gigabyte(4);
	long long currentSize = 0;
	std::unordered_map<std::string, std::unique_ptr<Texture>> ressourceCache;
	std::vector<std::string> timeOfInsertCache;
	AAssetManager* aassetManager = nullptr;
public:
	TextureAssetManager(AAssetManager* aassetManager);
	Texture* getOrAddRes(const std::string& filename);
	bool unloadNotUsedRes(const std::string& filename);
	void clear();
	bool isLoaded(const std::string& filename);
};