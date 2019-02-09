#include "Level.h"
#include "EventLevelReload.h"
#include "TouchInput.h"
#include "AssetLoader.h"

/*void Level::eventLevelReloadHandler(EventData* eventData)
{
	newLevel = std::make_unique<Level>(window, levelName);
	endLevel = true;
}*/

void Level::updateModel()
{
	float dt = clock.getTime().asSeconds();
	utils::logF("%f", dt);

	gom.updateActors(dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	//map.draw(window);
	gom.sortActors();
	gom.drawActors();

	physics.debugRenderBodies(window);

	// -- test code
	if (TouchInput::isTouched())
	{
		sprite.setPosition(TouchInput::getPosition());
	}

	window.draw(r);
	window.draw(sprite);
	// -- end test code
}

Level::Level(RenderWindow & window, std::string tiledMapName) : window(window), physics(),
gom(), clock(window.getClock()), eventManager(), map(tiledMapName, gom, eventManager, window), levelName(tiledMapName),
r()
{
	// -- test code
	c.setFillColor(Colors::Yellow);
	c.setRadius(50.0f);

	r.setSize(30.0f, 10.0f);
	r.setPosition({ window.getRenderWidth() - r.getSize().x, window.getRenderHeight() - r.getSize().y });

	//window.play(sound);
	// -- end test code

	eventManager.addListener(EventLevelReload::eventId, delegateLevelReload);
}

bool Level::Go()
{
	updateModel();
	window.clear();
	composeFrame();
	window.render();

	return endLevel;
}

std::unique_ptr<Level> Level::getNewLevel()
{
	return std::move(newLevel);
}
