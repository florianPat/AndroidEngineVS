#include "MainLevel.h"

void android_main(android_app* app)
{
	RenderWindow window(app, 900, 600, RenderWindow::ViewportType::EXTEND);

	std::unique_ptr<Level> currentLevel = std::make_unique<MainLevel>(window, "testLevel.tmx");

	while (window.processEvents())
	{
		if (currentLevel->Go())
		{
			currentLevel = currentLevel->getNewLevel();
			if (currentLevel == nullptr)
				return;
		}
	}

	//Game game(window, std::make_unique<MainLevel>(window, "testLevel.tmx"));
}