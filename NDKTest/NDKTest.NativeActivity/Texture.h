#pragma once

#include <GLES2/gl2.h>
#include "android_native_app_glue.h"
#include <string>
#include "Vector2.h"

class Texture
{
	int width = 0;
	int height = 0;
	GLuint texture = 0;
public:
	Texture() = default;
	bool loadFromFile(const std::string& filename, AAssetManager* assetManager, bool pixeld = false);
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	Vector2i getSize() const { return Vector2i{ width, height }; }
private:
	void read(AAsset* asset, void * buffer, size_t size);
};