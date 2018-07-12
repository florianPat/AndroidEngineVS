#pragma once

#include "GraphicsManager.h"

class Ship
{
public:
	Ship(GraphicsManager& graphicsManager);
private:
	static constexpr float INITIAL_X = 100;
	static constexpr float INITIAL_Y = 300;
	static constexpr int32_t SIZE = 640;
};