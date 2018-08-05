#include "Level.h"
#include "EventLevelReload.h"

/*void Level::eventLevelReloadHandler(EventData* eventData)
{
	newLevel = std::make_unique<Level>(window, levelName);
	endLevel = true;
}*/

void Level::updateModel()
{
	float dt = clock.getTime().asSeconds();

	gom.updateActors(dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	window.clear();

	map.draw(window);
	gom.sortActors();
	gom.drawActors();

	//physics.debugRenderBodies(window);

	window.render();
}

Level::Level(RenderWindow & window, std::string tiledMapName) : window(window), physics(), levelName(tiledMapName),
clock(), gom(), eventManager(), map(tiledMapName, gom, eventManager, window)
{
	eventManager.addListener(EventLevelReload::eventId, delegateLevelReload);
}

std::unique_ptr<Level> Level::Go()
{
	while (!endLevel && window.isOpen())
	{
		window.processEvents();

		updateModel();
		composeFrame();
	}

	if (!window.isOpen())
	{
		window.close();
	}

	return std::move(newLevel);
}