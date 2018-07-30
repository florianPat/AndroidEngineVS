#include "Game.h"
#include "MainLevel.h"

void android_main(android_app* app)
{
	RenderWindow window(app, 900, 600);

	Game game(window, std::make_unique<MainLevel>(window, "levelName.tmx"));

	window.close();
}