#pragma once

#include "ActivityHandler.h"
#include "Log.h"
#include "EventLoop.h"
#include "GraphicsManager.h"
#include "Ship.h"

struct DroitBlaster : public ActivityHandler
{
	DroitBlaster(android_app* app) : eventLoop(app, *this), graphicsManager(app), ship(graphicsManager)
	{
		utilsLog("Creating DroidBlaster");
	}

	void run()
	{
		eventLoop.run();
	}

	STATUS onActivate() override
	{
		utilsLog("onActivate");

		if (graphicsManager.start() != STATUS::OK)
			return STATUS::KO;
		else
			return STATUS::OK;
	}

	void onDeactivate() override
	{
		utilsLog("deactivate");
	}

	STATUS onStep() override
	{
		utilsLog("Step started");

		STATUS result = graphicsManager.update();

		//sleep in microseconds
		//60 frames a second!
		usleep(16666);
		utilsLog("Step ended");
		return result;
	}
private:
	EventLoop eventLoop;
	GraphicsManager graphicsManager;
	Ship ship;
};