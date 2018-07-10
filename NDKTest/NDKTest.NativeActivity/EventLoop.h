#pragma once

#include "android_native_app_glue.h"
#include "ActivityHandler.h"

class EventLoop
{
public:
	EventLoop(android_app* pApp, ActivityHandler& activityHandler);
	void run();
private:
	void activate();
	void deactivate();

	void processAppEvent(int32_t command);

	static void AppEventCallback(android_app* app, int32_t command);
private:
	android_app * app;
	bool enabled = false;
	bool quit = false;

	ActivityHandler& activityHandler;
};