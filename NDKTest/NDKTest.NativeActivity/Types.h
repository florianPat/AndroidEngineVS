#pragma once

#include <stdint.h>

typedef uint32_t uint;
typedef uint16_t ushort;
typedef uint8_t uchar;
typedef uint64_t ulong;

enum class STATUS
{
	OK = 0,
	KO = -1,
	EXIT = -2
};

struct Location
{
	Location() = default;
public:
	float x = 0.0f, y = 0.0f;
};