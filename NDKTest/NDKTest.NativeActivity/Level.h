#pragma once

#include "RenderWindow.h"
#include "TiledMap.h"
#include "Physics.h"
#include <iostream>
#include "GameObjectManager.h"
#include "EventManager.h"
#include "Utils.h"
#include "Clock.h"
#include "Sprite.h"
#include "CircleShape.h"
#include "RectangleShape.h"
#include "Texture.h"
#include "Sound.h"

class Level
{
protected:
	RenderWindow& window;
	Physics physics;
	GameObjectManager gom;
	Clock& clock;
	EventManager eventManager;
	TiledMap map;

	std::string levelName;

	std::unique_ptr<Level> newLevel = nullptr;
	bool endLevel = false;
	//Test
	CircleShape c;
	RectangleShape r;
	Texture* texture = window.getAssetManager()->getOrAddRes<Texture>("ship.png");
	Sprite sprite = Sprite(texture);
	Sound* sound = window.getAssetManager()->getOrAddRes<Sound>("nice.wav");
protected:
	std::function<void(EventData*)> eventLevelReloadFunction = std::bind(&Level::eventLevelReloadHandler, this, std::placeholders::_1);
	DelegateFunction delegateLevelReload = utils::getDelegateFromFunction(eventLevelReloadFunction);
protected:
	//TODO: Just pure virtual because you have to give it the class to recreate. Have not found how to not do it
	virtual void eventLevelReloadHandler(EventData* eventData) = 0;
protected:
	virtual void updateModel();
	virtual void composeFrame();
public:
	Level(RenderWindow& window, std::string tiledMapName);
	std::unique_ptr<Level> Go();
};