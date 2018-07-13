#pragma once

#include "ActivityHandler.h"
#include "Log.h"
#include "EventLoop.h"
#include "GraphicsManager.h"
#include "Ship.h"
#include "TimeManager.h"

struct DroitBlaster : public ActivityHandler
{
	DroitBlaster(android_app* app) : eventLoop(app, *this), graphicsManager(app), ship(graphicsManager), timeManager()
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

		graphicsManager.stop();
	}

	STATUS onStep() override
	{
		utilsLog("Step started");

		STATUS result = graphicsManager.update();

		assert(result == STATUS::OK);

		timeManager.update();
		float elapsed = timeManager.getElapsed();
		__android_log_print(ANDROID_LOG_INFO, "FPS", "%f", elapsed);

		if (elapsed < wantedGameHzMicroseconds)
		{
			ulong timeToSleep = wantedGameHzMicroseconds - elapsed;
			if (usleep(timeToSleep) != 0)
			{
				utilsLogBreak("usleep error!");
			}
		}

		timeManager.update();
		elapsed = timeManager.getElapsed();
		__android_log_print(ANDROID_LOG_INFO, "FPS", "%f", elapsed);

		//sleep in microseconds
		//60 frames a second
		//usleep(16666);
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
	TimeManager timeManager;
	static constexpr int wantedGameHzMicroseconds = 16666;
};