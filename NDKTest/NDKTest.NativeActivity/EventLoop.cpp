#include "EventLoop.h"
#include "Log.h"
#include "Types.h"

EventLoop::EventLoop(android_app * pApp, ActivityHandler& activityHandler) : app(pApp), activityHandler(activityHandler)
{
	app->userData = this;
	app->onAppCmd = AppEventCallback;
}

void EventLoop::run()
{
	int32_t events;
	android_poll_source* source;

	utilsLog("Starting event loop");
	while (1)
	{
		while ((ALooper_pollAll(enabled ? 0 : -1, 0, &events, (void**)&source)) >= 0)
		{
			if (source != 0)
			{
				source->process(app, source);
			}
			if (app->destroyRequested)
			{
				utilsLog("exit event loop");
				return;
			}
		}
		if (enabled && (!quit))
		{
			if (activityHandler.onStep() != STATUS::OK)
			{
				quit = true;
				ANativeActivity_finish(app->activity);
			}
		}
	}
}

void EventLoop::activate()
{
	if ((!enabled) && (app->window != NULL))
	{
		quit = false;
		enabled = true;
		if (activityHandler.onActivate() != STATUS::OK)
		{
			quit = true;
			deactivate();
			ANativeActivity_finish(app->activity);
		}
	}
}

void EventLoop::deactivate()
{
	if (enabled)
	{
		activityHandler.onDeactivate();
		enabled = false;
	}
}

void EventLoop::processAppEvent(int32_t command)
{
	switch (command)
	{
		case APP_CMD_CONFIG_CHANGED:
		{
			activityHandler.onConfigurationChanged();
			break;
		}
		case APP_CMD_INIT_WINDOW:
		{
			activityHandler.onCreateWindow();
			enabled = true;
			break;
		}
		case APP_CMD_DESTROY:
		{
			activityHandler.onDestroy();
			break;
		}
		case APP_CMD_GAINED_FOCUS:
		{
			activityHandler.onGainFocus();
			break;
		}
		case APP_CMD_LOST_FOCUS:
		{
			activityHandler.onLoseFocus();
			break;
		}
		case APP_CMD_LOW_MEMORY:
		{
			activityHandler.onLowMemory();
			break;
		}
		case APP_CMD_PAUSE:
		{
			activityHandler.onPause();
			break;
		}
		case APP_CMD_RESUME:
		{
			activityHandler.onResume();
			break;
		}
		case APP_CMD_SAVE_STATE:
		{
			activityHandler.onSaveInstanceState(&app->savedState, &app->savedStateSize);
			break;
		}
		case APP_CMD_START:
		{
			activityHandler.onStart();
			break;
		}
		case APP_CMD_STOP:
		{
			activityHandler.onStop();
			break;
		}
		case APP_CMD_TERM_WINDOW:
		{
			activityHandler.onDestroyWindow();
			deactivate();
			enabled = false;
			break;
		}
		default:
		{
			break;
		}
	}
}

void EventLoop::AppEventCallback(android_app * app, int32_t command)
{
	EventLoop& eventLoop = *(EventLoop*)app->userData;
	eventLoop.processAppEvent(command);
}
