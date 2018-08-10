#pragma once

#include "Texture.h"
#include "Vector2.h"
#include "Rect.h"
#include "Mat4x4.h"

//TODO: Texture Rect!!

class Sprite
{
	const Texture* texture = nullptr;
	IntRect rect;
	Vector2f pos, org;
	Vector2f scl = { 1.0f, 1.0f };
	float rot;
public:
	Sprite() = default;
	Sprite(const Texture& texture);
	Sprite(const Texture& texture, const IntRect& rect);
	void setTexture(const Texture& texture, bool resetRect = false);
	void setTextureRect(const IntRect& rect);
	const Texture* getTexture() const;
	const IntRect& getTextureRect() const;
	FloatRect getGlobalBounds() const;
	void setPosition(float x, float y);
	void setPosition(const Vector2f &position);
	void setRotation(float angle);
	void setScale(float factorX, float factorY);
	void setScale(const Vector2f &factors);
	void setOrigin(float x, float y);
	void setOrigin(const Vector2f &origin);
	const Vector2f& getPosition() const;
	float getRotation() const;
	const Vector2f& getScale() const;
	const Vector2f& getOrigin() const;
	const Mat4x4 getTransform() const;
};