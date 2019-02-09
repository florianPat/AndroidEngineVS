#pragma once

#include <string>
#include <functional>
#include "Texture.h"

struct AssetLoader
{
	std::function<bool(char* thiz, const std::string& filename)> loadFromFile;
	std::function<bool(char* thiz, const std::string& filename)> reloadFromFile;
	std::function<long long(char* thiz)> getSize;
public:
	template <typename T>
	static AssetLoader initLoader()
	{
		AssetLoader result = { 0 };
		result.loadFromFile = [](char* thiz, const std::string& filename) {return ((T*)thiz)->loadFromFile(filename); };
		result.reloadFromFile = [](char* thiz, const std::string& filename) {return ((T*)thiz)->reloadFromFile(filename); };
		result.getSize = [](char* thiz) {return ((T*)thiz)->getSize(); };
		return result;
	}
};