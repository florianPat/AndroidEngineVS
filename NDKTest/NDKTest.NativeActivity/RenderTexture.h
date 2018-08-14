#pragma once

#include "Texture.h"
#include "Sprite.h"
#include "Texture.h"
#include "Mat4x4.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <memory>

class RenderTexture
{
	GLuint renderTexture;
	Texture texture;
	Mat4x4 orhtoProj;
	Shader* shaderSprite = nullptr;
public:
	RenderTexture() = default;
	bool create(uint width, uint height, Shader* shaderSprite);
	void clear();
	const Texture& getTexture() const;
	void display();
	void draw(const Sprite& sprite);
};