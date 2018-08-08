#include "Ifstream.h"
#include "Utils.h"

Ifstream::Ifstream(const std::string & filename, AAssetManager * assetManager) : assetManager(assetManager)
{
	open(filename);
}

Ifstream::Ifstream(AAssetManager * assetManager) : assetManager(assetManager)
{
}

Ifstream::~Ifstream()
{
	close();
}

Ifstream::operator bool() const
{
	return fail;
}

bool Ifstream::operator!() const
{
	return !(operator bool());
}

bool Ifstream::eof()
{
	return (AAsset_getRemainingLength64(asset) == 0);
}

void Ifstream::getline(std::string & line)
{
	line = "";
	char c;
	while (!eof())
	{
		c = get();

		if (c == '\n')
		{
			if(!eof())
				c = get();
			
			break;
		}

		line += c;
	}
}

long long Ifstream::getSize()
{
	return AAsset_getLength64(asset);
}

void Ifstream::open(const std::string & filename)
{
	asset = AAssetManager_open(assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);

	if (!asset)
	{
		fail = true;
		return;
	}
}

void Ifstream::getFullData(void * s)
{
	s = (void*)AAsset_getBuffer(asset);
	assert(s != nullptr);
}

void Ifstream::close()
{
	if (asset != nullptr)
	{
		AAsset_close(asset);
		asset = nullptr;
	}
}

void Ifstream::read(void * s, std::streamsize n)
{
	assert(AAsset_read(asset, s, n) > 0);
}

char Ifstream::get()
{
	char c;
	read(&c, 1);
	return c;
}

std::streampos Ifstream::tellg()
{
	return std::streampos(getSize() - AAsset_getRemainingLength64(asset));
}

void Ifstream::seekg(std::streampos pos)
{
	assert(AAsset_seek64(asset, pos, SEEK_SET) != -1);
}

void Ifstream::seekg(std::streamoff off, std::ios_base::seekdir way)
{
	switch (way)
	{
		case std::ios_base::seekdir::_S_beg:
		{
			assert(AAsset_seek64(asset, off, SEEK_SET) != -1);
			break;
		}
		case std::ios_base::seekdir::_S_cur:
		{
			assert(AAsset_seek64(asset, off, SEEK_CUR) != -1);
			break;
		}
		case std::ios_base::seekdir::_S_end:
		{
			assert(AAsset_seek64(asset, off, SEEK_END) != -1);
			break;
		}
		case std::ios_base::seekdir::_S_ios_seekdir_end:
		{
			assert(AAsset_seek64(asset, off, SEEK_END) != -1);
			break;
		}
		default:
		{
			InvalidCodePath;
			break;
		}
	}
}
