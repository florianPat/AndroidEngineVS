#pragma once

#include "Types.h"
#include "android_native_app_glue.h"

class Resource
{
public:
	Resource(android_app* app, const char* path);
	const char* getPath() const;
	STATUS open();
	void close();
	STATUS read(void* buffer, size_t size);
	bool operator==(const Resource& other);
private:
	const char* path;
	AAssetManager* assetManager;
	AAsset* asset = nullptr;
};