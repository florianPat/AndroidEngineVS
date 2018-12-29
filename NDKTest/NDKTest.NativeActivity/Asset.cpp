#include "Asset.h"
#include "Utils.h"

void Asset::read(AAsset * asset, void * buffer, size_t size)
{
	assert(AAsset_read(asset, buffer, size) == size);
}

Asset::Asset(AssetId assetId) : assetId(assetId)
{
}
