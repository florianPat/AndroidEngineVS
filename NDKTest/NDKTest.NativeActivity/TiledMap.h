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
		String name;
		int width, height;
		Vector<Tile> tiles;
	};
	struct ObjectGroup
	{
		String name;
		Vector<Physics::Collider> objects;
	};
	std::unordered_map<int, Tile> tiles;
	std::unordered_map<String, Layer> layers;
	std::unordered_map<String, ObjectGroup> objectGroups;
	int mapWidth = 0, mapHeight = 0, tileWidth = 0, tileHeight = 0;

	RenderTexture texture;
	Sprite textureSprite;
	AssetManager* assetManager = nullptr;
public:
	TiledMap(const String& filepath, GameObjectManager& gom, EventManager& em, RenderWindow& window, Vector<String>&& toGameObjects = Vector<String>{});
	Vector<Physics::Collider> getObjectGroup(const String& objectGroupName);
	Vector<ObjectGroup> getObjectGroups();
	void draw(RenderWindow& renderWindow);
private:
	size_t getEndOfWord(const String& word, const String& lineContent, bool* result);
	String getLineContentBetween(String& lineContent, const String& endOfFirst, char secound);

	String ParseTiles(Ifstream& file);
	void ParseLayer(Ifstream& file, String& lineContent);
	void ParseObjectGroups(Ifstream& file, String& lineContent);
	void MakeRenderTexture(Vector<String>& toGameObjects, GameObjectManager& gom, EventManager& em, RenderWindow& window);
};