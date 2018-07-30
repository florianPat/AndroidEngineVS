#pragma once

#include "Level.h"

class MainLevel: public Level
{
	virtual void eventLevelReloadHandler(EventData* eventData) override;
public:
	MainLevel(sf::RenderWindow& window, std::string tiledMapName);
};