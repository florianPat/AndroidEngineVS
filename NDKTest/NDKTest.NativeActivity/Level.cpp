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
	map.draw(window);
	gom.sortActors();
	gom.drawActors();

	physics.debugRenderBodies(window);

	if (TouchInput::isTouched())
	{
		sprite.setPosition(TouchInput::getPosition());
	}

	window.draw(r);
	window.draw(sprite);
}

Level::Level(RenderWindow & window, std::string tiledMapName) : window(window), physics(),
gom(), clock(window.getClock()), eventManager(), map(tiledMapName, gom, eventManager, window), levelName(tiledMapName),
r()
{
	c.setFillColor(Colors::Yellow);
	c.setRadius(50.0f);

	r.setSize(30.0f, 10.0f);
	r.setPosition({ window.getRenderWidth() - r.getSize().x, window.getRenderHeight() - r.getSize().y });

	//window.play(sound);

	eventManager.addListener(EventLevelReload::eventId, delegateLevelReload);
}

std::unique_ptr<Level> Level::Go()
{
	while (!endLevel && window.isOpen())
	{
		window.processEvents();

		updateModel();
		bool shouldContinue = window.clear();
		if (shouldContinue)
		{
			composeFrame();
			window.render();
		}
		else
		{
			window.recoverFromContextLoss();
		}
	}

	if (!window.isOpen())
	{
		window.close();
	}

	return std::move(newLevel);
}
