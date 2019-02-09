#include "TiledMap.h"
#include "Ifstream.h"
#include <stdlib.h>
#include "Utils.h"
#include "TiledMapRenderComponent.h"

TiledMap::TiledMap(const std::string & filepath, GameObjectManager& gom, EventManager& em, RenderWindow& window, std::vector<std::string>&& toGameObjects)
	: tiles(), layers(), objectGroups(), texture(), textureSprite(), assetManager(window.getAssetManager())
{
	Ifstream file;
	file.open(filepath);

	if (!file)
	{
		utils::logBreak("Cant open file!");
	}

	std::string temp;
	file.getline(temp);

	if (!file.eof())
	{
		std::string lineContent;
		file.getline(lineContent);
		assert(utils::isWordInLine("<map", lineContent));

		if (!utils::isWordInLine("orthogonal", lineContent))
		{
			utils::logBreak("Map has to be orthogonal!");
		}

		if (!utils::isWordInLine("right-down", lineContent))
		{
			utils::logBreak("Maps render-order has to be right-down!");
		}


		mapWidth = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
		mapHeight = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

		tileWidth = atoi(getLineContentBetween(lineContent, "tilewidth", '"').c_str());
		tileHeight = atoi(getLineContentBetween(lineContent, "tileheight", '"').c_str());

		lineContent = ParseTiles(file);

		ParseLayer(file, lineContent);

		ParseObjectGroups(file, lineContent);

		if (!utils::isWordInLine("</map>", lineContent))
		{
			utils::logBreak("We should be at the end of the file!");
		}

		MakeRenderTexture(toGameObjects, gom, em, window);
	}
}

std::vector<Physics::Collider> TiledMap::getObjectGroup(const std::string& objectGroupName)
{
	auto result = objectGroups.find(objectGroupName);
	if (result != objectGroups.end())
		return result->second.objects;
	else
	{
		std::vector<Physics::Collider> result;
		InvalidCodePath;
		return result;
	}
}

std::vector<TiledMap::ObjectGroup> TiledMap::getObjectGroups()
{
	std::vector<ObjectGroup> result;

	for (auto it = objectGroups.begin(); it != objectGroups.end(); ++it)
	{
		result.push_back(it->second);
	}

	return result;
}

void TiledMap::draw(RenderWindow& renderWindow)
{
	renderWindow.draw(textureSprite);
}

size_t TiledMap::getEndOfWord(const std::string & word, const std::string & lineContent, bool* result)
{
	size_t o = 0;
	*result = false;
	while (o < lineContent.size() && !(*result))
	{
		std::string searchWord(word);
		auto it = searchWord.begin();
		o = lineContent.find(it[0], o);
		++it;
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				*result = true;
				break;
			}
		}
	}

	return ++o;
}

std::string TiledMap::getLineContentBetween(std::string & lineContent, const std::string & endOfFirst, char secound)
{
		std::string result;

		bool resultValue;
		size_t widthEndPos = getEndOfWord(endOfFirst, lineContent, &resultValue);
		if (resultValue)
		{
			lineContent.erase(0, widthEndPos += 2);

			size_t kommaPos = lineContent.find(secound);

			result = lineContent.substr(0, kommaPos);

			lineContent.erase(0, ++kommaPos);
		}

		return result;
}

void TiledMap::ParseLayer(Ifstream & file, std::string& lineContent)
{
	while (utils::isWordInLine("<layer", lineContent))
	{
		std::string layerName = getLineContentBetween(lineContent, "name", '"');
		int layerWidth = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
		int layerHeight = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

		layers.emplace(layerName, Layer{ layerName, layerWidth, layerHeight, std::vector<Tile>() });

		auto currentLayer = layers.find(layerName);

		file.getline(lineContent); //  <data encoding="csv">
		if (!utils::isWordInLine("csv", lineContent))
		{
			utils::logBreak("Maps encoding has to be \"csv\"");
		}

		file.getline(lineContent); //Begin of encoding

		for (int y = 0; y < layerHeight; ++y)
		{
			for (int x = 0; x < layerWidth; ++x)
			{
				size_t kommaPos = lineContent.find(',');
				int nextTileId = atoi(lineContent.substr(0, kommaPos).c_str());
				lineContent.erase(0, ++kommaPos);

				Tile nextTile{ 0, 0, 0, nullptr };
				if (tiles.find(nextTileId) != tiles.end())
					nextTile = tiles.find(nextTileId)->second;
				currentLayer->second.tiles.push_back(nextTile);
			}
			file.getline(lineContent);
		}
		assert(utils::isWordInLine("</data>", lineContent));
		file.getline(lineContent); // </layer>
		assert(utils::isWordInLine("</layer>", lineContent));
		file.getline(lineContent); //Maybe new <layer>
	}
}

void TiledMap::ParseObjectGroups(Ifstream & file, std::string & lineContent)
{
	//ObjectGroup
	while (utils::isWordInLine("<objectgroup", lineContent))
	{
		std::string objectGroupName = getLineContentBetween(lineContent, "name", '"');
		file.getline(lineContent);

		std::vector<Physics::Collider> objectVector;
		while (!utils::isWordInLine("</objectgroup>", lineContent))
		{
			assert(utils::isWordInLine("<object", lineContent));

			int x = atoi(getLineContentBetween(lineContent, "x", '"').c_str());
			int y = atoi(getLineContentBetween(lineContent, "y", '"').c_str());
			int width = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
			int height = atoi(getLineContentBetween(lineContent, "height", '"').c_str());

			objectVector.push_back(FloatRect((float)x, (float)y, (float)width, (float)height));

			file.getline(lineContent);
		}
		objectGroups.emplace(objectGroupName, ObjectGroup{ objectGroupName, objectVector });

		file.getline(lineContent);
	}
}

void TiledMap::MakeRenderTexture(std::vector<std::string>& toGameObjects, GameObjectManager& gom, EventManager& em, RenderWindow& window)
{
	if (texture.create(mapWidth*tileWidth, mapHeight*tileHeight, window.getSpriteShader()))
	{
		texture.clear();

		for (auto it = layers.begin(); it != layers.end(); ++it)
		{
			//NOTE: posY is needed here, because renderTexture 0 is bottom, but here it is top...
			for (int y = 0, posY = mapHeight - 1; y < mapHeight; ++y, --posY)
			{
				for (int x = 0; x < mapWidth; ++x)
				{
					Layer currentLayer = it->second;
					Texture* source = currentLayer.tiles[mapWidth * y + x].source;
					if (source == nullptr)
						continue;
					Sprite sprite(source);
					sprite.setPosition((float)x * tileWidth, (float)posY * tileHeight);

					if(toGameObjects.empty())
						texture.draw(sprite);
					else
					{
						bool toGO = false;
						for (auto toGOIt = toGameObjects.begin(); toGOIt != toGameObjects.end(); ++toGOIt)
						{
							if ((*toGOIt) == it->second.name)
							{
								Actor* actorP = gom.addActor();
								actorP->addComponent(std::make_unique<TiledMapRenderComponent>(sprite, window, em, actorP));
								toGO = true;
								break;
							}
						}
						if(!toGO)
							texture.draw(sprite);
					}
				}
			}
		}
		texture.display();

		textureSprite = Sprite(&texture.getTexture());
	}
	else
	{
		utils::logBreak("Could not create Render Texture");
	}
}

std::string TiledMap::ParseTiles(Ifstream & file)
{
	std::string lineContent;
	file.getline(lineContent);

	bool gridInFile = false;
	std::string temp;
	file.getline(temp); // <grid...
	if (utils::isWordInLine("<grid", temp))
	{
		gridInFile = true;
	}

	while (utils::isWordInLine("<tileset", lineContent))
	{
		int firstgrid = atoi(getLineContentBetween(lineContent, "firstgid", '"').c_str());
		int tileCount = atoi(getLineContentBetween(lineContent, "tilecount", '"').c_str());
		for (int i = 0; i < tileCount; ++i)
		{
			if (gridInFile)
			{
				file.getline(lineContent);
			}
			else
			{
				lineContent = temp;
				//NOTE: Just set it here, because in the next it`s it will get the next line
				gridInFile = true;
			}
			assert(utils::isWordInLine("<tile", lineContent));
			int id = atoi(getLineContentBetween(lineContent, "id", '"').c_str()) + firstgrid;

			file.getline(lineContent);
			assert(utils::isWordInLine("<image", lineContent));
			int width = atoi(getLineContentBetween(lineContent, "width", '"').c_str());
			int height = atoi(getLineContentBetween(lineContent, "height", '"').c_str());
			std::string source = getLineContentBetween(lineContent, "source", '"');
			tiles.emplace(id, Tile{ id, width, height, assetManager->getOrAddRes<Texture>(source) });

			file.getline(lineContent); //</tile>
			assert(utils::isWordInLine("</tile>", lineContent));
		}
		file.getline(lineContent); //</tileset>
		assert(utils::isWordInLine("</tileset>", lineContent));
		file.getline(lineContent); //Maybe new <tileset>...
	}

	return lineContent;
}
