#include "Ship.h"

Ship::Ship(GraphicsManager & graphicsManager)
{
	GraphicsElement* element = graphicsManager.registerElement(SIZE, SIZE);
	element->loc.x = INITIAL_X;
	element->loc.y = INITIAL_Y;
}
