#pragma once

#include "Level.h"
#include <memory>
#include "RenderWindow.h"

class Game
{
	RenderWindow& window;
	std::unique_ptr<Level> currentLevel;
public:
	Game(RenderWindow& window, std::unique_ptr<Level> level);
};