#pragma once

#include <assert.h>

#undef assert

#ifndef NDEBUG
	#define utilsLog(string) __android_log_write(ANDROID_LOG_INFO, __FUNCTION__, string);
	#define utilsLogBreak(string) __assert(__FUNCTION__, __LINE__, string)
	#define assert(exp) if(!(exp)) utilsLogBreak("Assert failure!")
#else
	#define utilsLog(string)
	#define utilsLogBreak(string)
#endif