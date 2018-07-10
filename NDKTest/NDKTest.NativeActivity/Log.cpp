#include "Log.h"
#include <android/log.h>

void Log::info(const char * pMessage)
{
	__android_log_write(ANDROID_LOG_INFO, "NDK-Test", pMessage);
}
