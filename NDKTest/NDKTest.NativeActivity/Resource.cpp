#include "Resource.h"

Resource::Resource(android_app * app, const char * path) : path(path), assetManager(app->activity->assetManager)
{
}

const char * Resource::getPath() const
{
	return path;
}

STATUS Resource::open()
{
	asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
	return asset != nullptr ? STATUS::OK : STATUS::KO;
}

void Resource::close()
{
	if (asset != nullptr)
	{
		AAsset_close(asset);
		asset = nullptr;
	}
}

STATUS Resource::read(void * buffer, size_t size)
{
	int resultSizeRead = AAsset_read(asset, buffer, size);
	return resultSizeRead == size ? STATUS::OK : STATUS::KO;
}

bool Resource::operator==(const Resource & other)
{
	return (strcmp(path, other.path) == 0);
}
