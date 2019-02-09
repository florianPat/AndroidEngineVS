#include "TextureAtlas.h"
#include "Ifstream.h"
#include <stdlib.h>
#include "Utils.h"
#include "Rect.h"

TextureAtlas::TextureAtlas(const std::string& filepath, AssetManager* assetManager) : textureAtlas(), fileHeader{}, assetManager(assetManager)
{
	Ifstream file;
	file.open(filepath);

	if (!file)
	{
		utils::logBreak("Cant open file!");
	}

	std::string tempString;

	file.getline(tempString);

	if (!file.eof())
	{
		for (int i = 0; i < FILE_HEADER_LINE_SIZE; ++i)
		{
			std::string lineContent;
			file.getline(lineContent);

			switch (i)
			{
				case 0:
				{
					fileHeader.name = lineContent;
				}	break;
				case 1:
				{
					int width, height;
					std::string widthChar, heightChar;
					
					
					widthChar = getLineContentBetweeen(lineContent, ' ', ',');
					
					size_t nCharsToCopyHeight = lineContent.size();
					heightChar = lineContent.substr(0, nCharsToCopyHeight);

					width = atoi(widthChar.c_str());
					height = atoi(heightChar.c_str());

					fileHeader.size = Vector2i(width, height);
				}	break;
				case 2:
				{
					size_t spacePos = lineContent.find(' ');
					lineContent.erase(0, ++spacePos);
					fileHeader.format = lineContent;
				}	break;
				case 3:
				{
					
					fileHeader.filter[0] = getLineContentBetweeen(lineContent, ' ', ',');
					
					size_t nCharsToCopyFilter1 = lineContent.size();
					fileHeader.filter[1] = lineContent.substr(0, nCharsToCopyFilter1);
				}	break;
				case 4:
				{
					size_t spacePos = lineContent.find(' ');
					lineContent.erase(0, ++spacePos);
					fileHeader.repeat = lineContent;
				}
			}
		}
	}

	while(!file.eof())
	{
		bool isEof = false;
		TextureRegion region;

		region.textureAtlasFileName = fileHeader.name;

		for (int i = 0; i < FILE_LINES_PER_REGION; ++i)
		{
			if (file.eof())
			{
				isEof = true;
				break;
			}

			std::string lineContent;
			file.getline(lineContent);

			switch (i)
			{
				case 0:
				{
					region.filename = lineContent;
				}	break;
				case 1: {}	break;
				case 2:
				{
					region.xy = getLineContentRegionValues(lineContent, 'x');
				}	break;
				case 3:
				{
					region.size = getLineContentRegionValues(lineContent, 's');
				}	break;
				case 4: {}	break;
				case 5: {}	break;
				case 6: {}	break;
			}
		}

		if(!isEof)
			textureAtlas.insert({ region.filename, region });
	}

	file.close();

	for (auto i = textureAtlas.begin(); i != textureAtlas.end(); ++i)
	{
		i->second.initSprite(assetManager);
	}
}

std::unique_ptr<TextureRegion> TextureAtlas::findRegion(const std::string& name) const
{
	auto result = textureAtlas.find(name);
	if (result != textureAtlas.end())
	{
		return std::make_unique<TextureRegion>(result->second);
	}
	else
	{
		return nullptr;
	}
}

std::vector<TextureRegion> TextureAtlas::getRegions()
{
	auto result = std::vector<TextureRegion>();
	int i = 0;
	for (auto it = textureAtlas.begin(); it != textureAtlas.end(); ++it, ++i)
	{
		result.push_back(it->second);
		result[i].initSprite(assetManager);
	}
	return result;
}

void TextureAtlas::addRegion(const TextureRegion & adder)
{
	textureAtlas.insert({ adder.filename, adder });
}

std::string TextureAtlas::getLineContentBetweeen(std::string & lineContent, char first, char secound)
{
	std::string result;

	size_t spacePos = lineContent.find(first);
	lineContent.erase(0, ++spacePos);

	size_t kommaPos = lineContent.find(secound);

	size_t nCharsToCopy = kommaPos;//(lineContent.size() - kommaPos);
	result = lineContent.substr(0, nCharsToCopy);

	lineContent.erase(0, ++kommaPos);

	return result;
}

Vector2i TextureAtlas::getLineContentRegionValues(std::string & lineContent, char firstRealChar)
{
	Vector2i result;

	lineContent.erase(0, lineContent.find(firstRealChar));
	result.x = atoi(getLineContentBetweeen(lineContent, ' ', ',').c_str());
	lineContent.erase(0, 1);
	result.y = atoi(lineContent.c_str());

	return result;
}

void TextureRegion::initSprite(AssetManager* assetManager)
{
	assert(textureAtlasFileName != "" || filename != "");

	atlasTexture = assetManager->getOrAddRes<Texture>(textureAtlasFileName);
	regionSprite = Sprite(atlasTexture, IntRect(xy.x, xy.y, size.x, size.y));
}

void TextureRegion::setRegion(int x, int y, int widht, int height)
{
	if(x >= 0 && y >= 0)
		if ((unsigned int)widht <= regionSprite.getTexture()->getWidth() && (unsigned int)height <= regionSprite.getTexture()->getHeight())
		{
			regionSprite.setTextureRect(IntRect(x, y, widht, height));
			xy.x = x;
			xy.y = y;
			size.x = widht;
			size.y = height;
		}
}

Sprite TextureRegion::getRegion()
{
	return regionSprite;
}
