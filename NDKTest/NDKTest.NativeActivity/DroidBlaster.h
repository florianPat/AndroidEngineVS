#pragma once

#include "ActivityHandler.h"
#include "Log.h"
#include "EventLoop.h"

struct DroitBlaster : public ActivityHandler
{
	DroitBlaster(android_app* app) : eventLoop(app, *this)
	{
		Log::info("Creating DroidBlaster");
	}

	void run()
	{
		eventLoop.run();
	}

	STATUS onActivate() override
	{
		Log::info("onActivate");
		return STATUS::OK;
	}

	void onDeactivate() override
	{
		Log::info("deactivate");
	}

	STATUS onStep() override
	{
		Log::info("Step started");
		//sleep in microseconds
		usleep(1000000);
		Log::info("Step ended");
		return STATUS::OK;
	}
private:
	EventLoop eventLoop;
};