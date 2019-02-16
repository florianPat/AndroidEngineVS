#include "Level.h"
#include "EventLevelReload.h"
#include "TouchInput.h"
#include "AssetLoader.h"
#include "Benchmark.h"

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
		//sprite.setPosition(TouchInput::getPosition());
	}

	window.draw(r);
	//window.draw(sprite);
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

	Benchmark& benchmark = Benchmark::getBenchmark();
	AssetManager* assetManager = window.getAssetManager();
	std::srand(5);

	benchmark.start("Asset loading");
	for (int i = 0; i < NUM_TEXTURES; ++i)
	{
		texture[i] = assetManager->getOrAddRes<Texture>(textureNames[std::rand() % 7]);
	}
	sound[0] = assetManager->getOrAddRes<Sound>("nice.wav");
	benchmark.stop();

	benchmark.start("One asset loading");
	for (int i = 1; i < (100 - NUM_TEXTURES); ++i)
	{
		sound[i] = assetManager->getOrAddRes<Sound>("nice.wav");
	}
	benchmark.stop();

	benchmark.start("Is asset loaded");
	for (int i = 0; i < 30; ++i)
	{
		int index = std::rand() % 10;
		if (index < 8)
			assetManager->isLoaded(textureNames[index]);
		else
			assetManager->isLoaded("hahah.png");
	}
	benchmark.stop();

	benchmark.start("Reload all assets");
	assetManager->reloadAllRes();
	benchmark.stop();

	benchmark.start("Clear assets");
	assetManager->clear();
	benchmark.stop();

	/*Results:
		Asset loading: has taken: 0.046008
		One asset loading: has taken: 0.000069
		Is asset loaded: has taken: 0.000077
		Reload all assets: has taken: 0.125085
		Clear assets: has taken: 0.000063
	*/

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
