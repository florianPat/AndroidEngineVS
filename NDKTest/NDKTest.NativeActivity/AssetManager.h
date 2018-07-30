#pragma once

#include <memory>
#include <unordered_map>
#include "SFML\Graphics.hpp"
#include "Utils.h"

class TextureAssetManager
{
	static constexpr long long maxSize = Gigabyte(4);
	long long currentSize = 0;
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> ressourceCache;
	std::vector<std::string> timeOfInsertCache;
public:
	sf::Texture* getOrAddRes(const std::string& filename);
	bool unloadNotUsedRes(const std::string& filename);
	void clear();
	bool isLoaded(const std::string& filename);
};