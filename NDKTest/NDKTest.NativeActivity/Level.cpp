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
	__android_log_print(ANDROID_LOG_INFO, "FPS", "%f", dt);

	static float counter = 0.0f;
	counter += 0.1f;

	sprite.setPosition(sprite.getPosition().x + 10.0f * dt, sprite.getPosition().y + 10.0f * dt);
	sprite.setRotation(sprite.getRotation() + 70.0f * dt);
	sprite.setScale(sinf(counter));
	sprite.setColor(Color::Red());

	gom.updateActors(dt);

	physics.update(dt);
}

void Level::composeFrame()
{
	window.clear();

	//map.draw(window);
	gom.sortActors();
	gom.drawActors();

	//physics.debugRenderBodies(window);

	window.draw(sprite);

	window.render();
}

Level::Level(RenderWindow & window, std::string tiledMapName) : window(window), physics(),
gom(), clock(), eventManager(), /*map(tiledMapName, gom, eventManager, window),*/ levelName(tiledMapName), 
texture(window.getAssetManager()->getOrAddRes("ship.png")), sprite(texture)
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
