#pragma once

#include "Texture.h"
#include "Sprite.h"

//TODO: Implement!
class RenderTexture
{
public:
	RenderTexture() = default;
	bool create(uint width, uint height);
	void clear();
	const Texture& getTexture() const;
	void display();
	void draw(const Sprite& sprite);
};