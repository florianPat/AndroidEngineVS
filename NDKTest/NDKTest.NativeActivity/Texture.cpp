#include "Texture.h"
#include <png.h>
#include "Utils.h"

bool Texture::loadFromFile(const std::string & filename, AAssetManager * assetManager, bool pixeld)
{
	bool result = true;

	AAsset* asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);

	if (!asset)
		result = false;

	GLint format;
	png_byte header[8];
	png_structp png = nullptr;
	png_infop info = nullptr;
	png_byte* image = nullptr;
	png_bytep* rowPtrs = nullptr;
	png_int_32 rowSize;
	bool transparency;

	read(asset, header, sizeof(header));
	if (png_sig_cmp(header, 0, 8) != 0)
		result = false;

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png)
		result = false;
	info = png_create_info_struct(png);
	if (!info)
		result = false;

	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	png_int_32 depth, colorType;
	png_uint_32 width, height;
	png_get_IHDR(png, info, &width, &height, &depth, &colorType, nullptr, nullptr, nullptr);

	transparency = false;
	if (png_get_valid(png, info, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png);
		transparency = true;
	}

	if (depth < 8)
	{
		png_set_packing(png);
	}
	else if (depth == 16)
	{
		png_set_strip_16(png);
	}

	switch (colorType)
	{
		case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(png);
			format = transparency ? GL_RGBA : GL_RGB;
			break;
		}
		case PNG_COLOR_TYPE_RGB:
		{
			format = transparency ? GL_RGBA : GL_RGB;
			break;
		}
		case PNG_COLOR_TYPE_RGBA:
		{
			format = GL_RGBA;
			break;
		}
		case PNG_COLOR_TYPE_GRAY:
		{
			png_set_expand_gray_1_2_4_to_8(png);
			format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			break;
		}
		case PNG_COLOR_TYPE_GA:
		{
			png_set_expand_gray_1_2_4_to_8(png);
			format = GL_LUMINANCE_ALPHA;
			break;
		}
	}
	png_read_update_info(png, info);

	rowSize = png_get_rowbytes(png, info);
	if (rowSize <= 0)
		result = false;

	image = new png_byte[rowSize * height];
	rowPtrs = new png_bytep[height];

	for (uint32_t i = 0; i < height; ++i)
	{
		rowPtrs[height - (i + 1)] = image + i * rowSize;
	}
	png_read_image(png, rowPtrs);

	png_destroy_read_struct(&png, &info, nullptr);
	delete[] rowPtrs;
	if (asset != nullptr)
	{
		AAsset_close(asset);
		asset = nullptr;
	}

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixeld ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixeld ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	delete[] image;

	return result;
}

void Texture::read(AAsset * asset, void * buffer, size_t size)
{
	int resultSizeRead = AAsset_read(asset, buffer, size);
	assert(resultSizeRead == size);
}
