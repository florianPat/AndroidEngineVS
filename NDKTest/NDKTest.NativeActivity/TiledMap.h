#pragma once

#include <unordered_map>
#include "Texture.h"
#include <memory>
#include "Physics.h"
#include "GameObjectManager.h"
#include "RenderTexture.h"
#include "AssetManager.h"
#include "Ifstream.h"

class TiledMap
{
	struct Tile
	{
		int id;
		int width, height;
		Texture* source;
	};
	struct Layer
	{
		std::string name;
		int width, height;
		std::vector<Tile> tiles;
	};
	struct ObjectGroup
	{
		std::string name;
		std::vector<Physics::Collider> objects;
	};
	std::unordered_map<int, Tile> tiles;
	std::unordered_map<std::string, Layer> layers;
	std::unordered_map<std::string, ObjectGroup> objectGroups;
	int mapWidth = 0, mapHeight = 0, tileWidth = 0, tileHeight = 0;

	RenderTexture texture;
	Sprite textureSprite;
	TextureAssetManager* assetManager = nullptr;
public:
	TiledMap(const std::string& filepath, GameObjectManager& gom, EventManager& em, RenderWindow& window, std::vector<std::string>&& toGameObjects = std::vector<std::string>{});
	std::vector<Physics::Collider> getObjectGroup(const std::string& objectGroupName);
	std::vector<ObjectGroup> getObjectGroups();
	void draw(RenderWindow& renderWindow);
private:
	size_t getEndOfWord(const std::string& word, const std::string& lineContent, bool* result);
	std::string getLineContentBetween(std::string& lineContent, const std::string& endOfFirst, char secound);

	std::string ParseTiles(Ifstream& file);
	void ParseLayer(Ifstream& file, std::string& lineContent);
	void ParseObjectGroups(Ifstream& file, std::string& lineContent);
	void MakeRenderTexture(std::vector<std::string>& toGameObjects, GameObjectManager& gom, EventManager& em, RenderWindow& window);
};