#include "Game.h"
#include "MainLevel.h"

void android_main(android_app* app)
{
	RenderWindow window(app, 900, 600, RenderWindow::ViewportType::EXTEND);

	Game game(window, std::make_unique<MainLevel>(window, "testLevel.tmx"));

	window.close();
}