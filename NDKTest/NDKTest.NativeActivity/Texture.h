#pragma once

#include <GLES2/gl2.h>
#include "android_native_app_glue.h"
#include <string>
#include "Vector2.h"
#include "Asset.h"

class Texture : public Asset
{
public:
	static constexpr Asset::AssetId assetId = Asset::AssetId::TEXTURE;
private:
	friend class RenderTexture;

	static constexpr bool pixeld = false;

	int width = 0;
	int height = 0;
	GLuint texture = 0;
private:
	bool loadFromFile(const std::string& filename, AAssetManager* assetManager) override;
	bool reloadFromFile(const std::string& filename, AAssetManager* assetManager) override;
public:
	Texture();
	~Texture();
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	long long getSize() const override { return width * height * sizeof(int32_t); }
	explicit operator bool() const;
	void bind(int slot = 0) const;
};