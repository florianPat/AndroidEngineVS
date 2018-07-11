#pragma once

#include "GraphicsManager.h"

class Ship
{
public:
	Ship(GraphicsManager& graphicsManager);
private:
	static constexpr float INITIAL_X = 0.5f;
	static constexpr float INITIAL_Y = 0.25f;
	static constexpr int32_t SIZE = 64;
};