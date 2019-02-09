#include "Utils.h"

static int counter = 0;

unsigned int utils::getGUID()
{
	return counter++;
}

bool utils::isWordInLine(const std::string & word, const std::string & lineContent)
{
	size_t o = 0;
	bool result = false;
	while (o < lineContent.size() && !result)
	{
		o = lineContent.find(word[0], o);
		std::string searchWord(word);
		auto it = ++searchWord.begin();
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				result = true;
				break;
			}
		}
	}

	return result;
}

DelegateFunction utils::getDelegateFromFunction(std::function<void(EventData*)>& function)
{
	return DelegateFunction(std::pair<unsigned int, std::function<void(EventData*)>>(getGUID(), function));
}

std::string utils::getWordBetweenChars(const std::string& lineContent, char firstChar, char lastChar)
{
	std::string result;
	size_t first = lineContent.find_first_of(firstChar);
	++first;
	size_t last = lineContent.find_last_of(lastChar);
	result = lineContent.substr(first, last - first);
	return result;
}

float utils::lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}

float utils::degreesToRadians(float degree)
{
	float radians = PiOver180 * degree;
	return radians;
}

float utils::radiansToDegrees(float radians)
{
	float degrees = _180OverPi * radians;
	return degrees;
}

void utils::logF(const char * string, ...)
{
#ifndef NDEBUG
	va_list argList = { 0 };
	va_start(argList, string);
	__android_log_vprint(ANDROID_LOG_INFO, "utilsLog", string, argList);
	va_end(argList);
#endif
}

void utils::logFBreak(const char * string, ...)
{
#ifndef NDEBUG
	va_list argList = { 0 };
	va_start(argList, string);
	__android_log_vprint(ANDROID_LOG_INFO, "utilsLog", string, argList);
	va_end(argList);

	assert(false);
#endif
}

void utils::log(const char * string)
{
#ifndef NDEBUG
	__android_log_write(ANDROID_LOG_INFO, "utilsLog", string);
#endif
}

void utils::logBreak(const char * string)
{
#ifndef NDEBUG
	do { log((string)); assert(false); } while (false);
#endif
}
