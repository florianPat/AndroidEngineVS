#include "Game.h"

Game::Game(RenderWindow& window, std::unique_ptr<Level> level) : window(window), currentLevel(std::move(level))
{
	while (window.isOpen())
	{
		window.processEvents();

		std::unique_ptr<Level> newLevel = currentLevel->Go();

		if (newLevel == nullptr)
		{
			window.close();
			return;
		}

		currentLevel = std::move(newLevel);
	}
}
