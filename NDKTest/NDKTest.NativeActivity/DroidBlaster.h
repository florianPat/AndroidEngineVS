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

	void onStart() override { utilsLog("onStart"); }
	void onResume() override { utilsLog("onResume"); }
	void onPause() override { utilsLog("onPause"); }
	void onStop() override { utilsLog("onStop"); }
	void onDestroy() override { utilsLog("onDestroy"); }

	void onSaveInstanceState(void** pData, size_t* pSize) override { utilsLog("onSaveInstanceState"); }
	void onConfigurationChanged() override { utilsLog("onConfigurationChanged"); }
	void onLowMemory() override { utilsLog("onLowMemory"); }

	void onCreateWindow() override 
	{
		utilsLog("onCreateWindow");
		if (onActivate() != STATUS::OK)
		{
			utilsLogBreak("onActivate error!");
		}
	}
	void onDestroyWindow() override 
	{
		utilsLog("onDestroyWindow");
		onDeactivate();
	}
	void onGainFocus() override { utilsLog("onGainFocus"); }
	void onLoseFocus() override { utilsLog("onLoseFocus"); }
private:
	EventLoop eventLoop;
	GraphicsManager graphicsManager;
	Ship ship;
};