#pragma once

#include "Component.h"
#include "Sprite.h"
#include "RenderWindow.h"
#include "Utils.h"

class TiledMapRenderComponent : public Component
{
	const Sprite sprite;
	RenderWindow& renderTarget;
public:
	TiledMapRenderComponent(Sprite& sprite, RenderWindow& renderTarget, EventManager& em, Actor* owner) : sprite(sprite), renderTarget(renderTarget), Component(utils::getGUID(), &em, owner) {};
	void update(float dt) override {};
	void draw() override { renderTarget.draw(sprite); };
	gomSort::SortKey sort() override { return gomSort::SortKey{ 0, sprite.getPosition().y }; }
};