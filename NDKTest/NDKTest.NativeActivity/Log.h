#pragma once

class Log
{
public:
	static void info(const char* pMessage);
};

#ifndef NDEBUG
#define log_debug(pMessage) Log::debug(pMessage)
#else
#define log_debug(pMessage)
#endif