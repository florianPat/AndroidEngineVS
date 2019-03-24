#pragma once

#include "Vector.h"
#include "String.h"

class Sound
{
#define RIFF_CODE(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

	enum class ChunkId
	{
		FMT = RIFF_CODE('f', 'm', 't', ' '),
		DATA = RIFF_CODE('d', 'a', 't', 'a'),
		RIFF = RIFF_CODE('R', 'I', 'F', 'F'),
		WAVE = RIFF_CODE('W', 'A', 'V', 'E')
	};

#pragma pack(push, 1)
	struct FileHeader
	{
		uint riffId;
		uint size;
		uint waveId;
	};

	struct Chunk
	{
		uint id;
		uint size;
	};

	struct Fmt
	{
		uint16_t wFormatTag;
		uint16_t nChannles;
		uint nSamplesPerSecond;
		uint nAvgBytesPerSec;
		uint16_t nBlockAlign;
		uint16_t wBitsPerSample;
		uint16_t cbSize;
		uint16_t wValidBitsPerSample;
		uint dwChannelMask;
		uint16_t subFormat[16];
	};
#pragma pack(pop)

	class RiffIt
	{
		uint8_t* at;
		uint8_t* stop;
	public:
		RiffIt(void* at, void* stop);
		explicit operator bool() const;
		RiffIt& operator++();
		void* getChunkData() const;
		uint getType() const;
		uint getChunkDataSize() const;
	};
private:
	Vector<Vector<short>> samples;
	int nChannels = 0;
	int nSamples = 0;
public:
	bool loadFromFile(const String& filename);
	bool reloadFromFile(const String& filename);
public:
	Sound();
	const Vector<Vector<short>>& getSamples() const;
	const int getNSamples() const;
	long long getSize() const;
	const short* getBuffer() const;
	explicit operator bool() const;
};