#include "Sprite.h"

#include "Mat4x4.h"

Sprite::Sprite(const Texture & texture) : texture(&texture), rect(0, 0, texture.getWidth(), texture.getHeight())
{
}

Sprite::Sprite(const Texture & texture, const IntRect & rect) : texture(&texture), rect(rect)
{
}

void Sprite::setTexture(const Texture & textureIn, bool resetRect)
{
	texture = &textureIn;
	if (resetRect)
	{
		rect = { 0, 0, texture->getWidth(), texture->getHeight() };
	}
}

void Sprite::setTextureRect(const IntRect & rectIn)
{
	rect = rectIn;
}

const Texture * Sprite::getTexture() const
{
	return texture;
}

const IntRect & Sprite::getTextureRect() const
{
	return rect;
}

FloatRect Sprite::getGlobalBounds() const
{
	float halfWidth = texture->getWidth() / 2.0f;
	float halfHeight = texture->getHeight() / 2.0f;

	auto transform = getTransform();

	Vector2f leftTop = transform * Vector2f{ -halfWidth, -halfHeight };
	Vector2f rightBottom = transform * Vector2f { halfWidth, halfHeight };

	FloatRect result = FloatRect(leftTop.x, leftTop.y, (rightBottom.x - leftTop.x), (rightBottom.y - leftTop.y));

	return result;
}

void Sprite::setPosition(float x, float y)
{
	pos = { x, y };
}

void Sprite::setPosition(const Vector2f & position)
{
	pos = position;
}

void Sprite::setRotation(float angle)
{
	rot = angle;
}

void Sprite::setScale(float factorX, float factorY)
{
	scl = { factorX, factorY };
}

void Sprite::setScale(const Vector2f & factors)
{
	scl = factors;
}

void Sprite::setOrigin(float x, float y)
{
	org = { x, y };
}

void Sprite::setOrigin(const Vector2f & origin)
{
	org = origin;
}

const Vector2f & Sprite::getPosition() const
{
	return pos;
}

float Sprite::getRotation() const
{
	return rot;
}

const Vector2f & Sprite::getScale() const
{
	return scl;
}

const Vector2f & Sprite::getOrigin() const
{
	return org;
}

const Mat4x4 Sprite::getTransform() const
{
	Mat4x4 result;

	result.translate(org);

	result.scale(scl);
	result.rotate(rot);
	result.translate(pos);

	return result;
}
