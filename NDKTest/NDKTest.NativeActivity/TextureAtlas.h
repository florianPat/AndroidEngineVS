#pragma once

#include "Vector.h"
#include <string>
#include "Texture.h"
#include <memory>
#include <unordered_map>
#include "Vector2.h"
#include "Sprite.h"
#include "AssetManager.h"

class TextureRegion
{
	friend class TextureAtlas;

	std::string textureAtlasFileName;
	std::string filename;
	Vector2i xy;
	Vector2i size;

	Texture* atlasTexture;
	Sprite regionSprite;
private:
	TextureRegion() = default;
	void initSprite(AssetManager* assetManager);
public:
	std::string getAtlasFileName() { return textureAtlasFileName; }
	std::string getRegionName() { return filename; }
	Vector2i getXY() { return xy; }
	Vector2i getSize() { return size; }

	void setRegion(int x, int y, int widht, int height);
	Sprite getRegion();
};

class TextureAtlas
{
	struct FileHeader
	{
		std::string name;
		Vector2i size;
		std::string format;
		std::string filter[2];
		std::string repeat;
	};
public:
	TextureAtlas(const std::string& filepath, AssetManager* assetManger);

	std::unique_ptr<TextureRegion> findRegion(const std::string& name) const;
	Vector<TextureRegion> getRegions();
	void addRegion(const TextureRegion& adder);
private:
	std::string getLineContentBetweeen(std::string& lineContent, char first, char secound);
	Vector2i getLineContentRegionValues(std::string& lineContent, char firstRealChar);
private:
	std::unordered_map<std::string, TextureRegion> textureAtlas;
	static constexpr int FILE_HEADER_LINE_SIZE = 5;
	static constexpr int FILE_LINES_PER_REGION = 7;
	FileHeader fileHeader;
	AssetManager* assetManager = nullptr;
};