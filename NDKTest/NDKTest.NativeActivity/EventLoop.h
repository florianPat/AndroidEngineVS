#pragma once

#include "android_native_app_glue.h"

class EventLoop
{
public:
	EventLoop(android_app* pApp);
	void run();
private:
	android_app * app;
};