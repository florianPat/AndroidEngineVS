#pragma once

#include <string>
#include <vector>
#include "TextureAtlas.h"
#include "Animation.h"
#include "EventManager.h"

#undef assert

#ifndef NDEBUG
#define utilsLog(string) __android_log_write(ANDROID_LOG_INFO, __FUNCTION__, string);
#define assert(exp) if(!((exp) && true)) __android_log_assert(nullptr, "ASSERT", #exp);
#define utilsLogBreak(string) do { utilsLog((string)); assert(false); } while(false)
#else
#define utilsLog(string)
#define utilsLogBreak(string)
#define assert(exp)
#endif
#define arrayCount(x) sizeof(x) / sizeof(x[0])

#define InvalidCodePath assert(!"InvalidCodePath")

#define Kilobyte(x) x * 1024ll
#define Megabyte(x) Kilobyte(x) * 1024ll
#define Gigabyte(x) Megabyte(x) * 1024ll

namespace utils
{
	unsigned int getGUID();
	bool isWordInLine(const std::string & word, const std::string & lineContent);
	DelegateFunction getDelegateFromFunction(std::function<void(EventData*)>& function);
	std::string getWordBetweenChars(const std::string& lineContent, char first, char last);
	float lerp(float v0, float v1, float t);
}