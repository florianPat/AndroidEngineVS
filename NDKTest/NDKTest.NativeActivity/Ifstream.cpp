#include "Ifstream.h"
#include "Utils.h"
#include "AssetManager.h"

Ifstream::Ifstream(const std::string & filename) : aassetManager(AssetManager::aassetManager)
{
	open(filename);
}

Ifstream::Ifstream() : aassetManager(AssetManager::aassetManager)
{
}

Ifstream::Ifstream(Ifstream && other) : asset(std::exchange(other.asset, nullptr)), aassetManager(std::exchange(other.aassetManager, nullptr)),
										good(std::exchange(other.good, false))
{
}

Ifstream & Ifstream::operator=(Ifstream && rhs)
{
	close();

	asset = std::exchange(rhs.asset, nullptr);
	aassetManager = std::exchange(rhs.aassetManager, nullptr);
	good = std::exchange(rhs.good, false);

	return *this;
}

Ifstream::~Ifstream()
{
	close();
}

Ifstream::operator bool() const
{
	return good;
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
			break;

		line += c;
	}
}

long long Ifstream::getSize()
{
	return AAsset_getLength64(asset);
}

void Ifstream::open(const std::string & filename)
{
	asset = AAssetManager_open(aassetManager, filename.c_str(), AASSET_MODE_UNKNOWN);

	if (!asset)
	{
		good = false;
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

void Ifstream::read(void * s, uint n)
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

void Ifstream::seekg(uint pos)
{
	assert(AAsset_seek64(asset, pos, SEEK_SET) != -1);
}

void Ifstream::seekg(uint off, SeekDir way)
{
	switch (way)
	{
		case SeekDir::beg:
		{
			assert(AAsset_seek64(asset, off, SEEK_SET) != -1);
			break;
		}
		case SeekDir::cur:
		{
			assert(AAsset_seek64(asset, off, SEEK_CUR) != -1);
			break;
		}
		case SeekDir::end:
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
