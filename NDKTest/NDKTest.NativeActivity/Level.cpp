#include "Level.h"
#include "EventLevelReload.h"
#include "TouchInput.h"

/*void Level::eventLevelReloadHandler(EventData* eventData)
{
	newLevel = std::make_unique<Level>(window, levelName);
	endLevel = true;
}*/

void Level::updateModel()
{
	float dt = clock.getTime().asSeconds();
	__android_log_print(ANDROID_LOG_INFO, "FPS", "%f", dt);

	gom.updateActors(dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	window.clear();

	map.draw(window);
	gom.sortActors();
	gom.drawActors();

	physics.debugRenderBodies(window);

	//window.draw(c);

	if (TouchInput::isTouched())
	{
		//r.setPosition(TouchInput::getPosition());
		sprite.setPosition(TouchInput::getPosition());
	}

	window.draw(r);
	window.draw(sprite);

	window.render();
}

Level::Level(RenderWindow & window, std::string tiledMapName) : window(window), physics(),
gom(), clock(), eventManager(), map(tiledMapName, gom, eventManager, window), levelName(tiledMapName),
r()
{
	c.setFillColor(Colors::Yellow);
	//c.setPosition(300.0f, 300.0f);
	c.setRadius(50.0f);

	r.setSize(30.0f, 10.0f);
	r.setPosition({ 0.0f, window.getRenderHeight() - r.getSize().y });

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
