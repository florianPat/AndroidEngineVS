#include "EventLoop.h"
#include "Log.h"
#include "Types.h"

EventLoop::EventLoop(android_app * pApp) : app(pApp)
{
}

void EventLoop::run()
{
	int result, events;
	android_poll_source* source;

	Log::info("Starting event loop");
	while (1)
	{
		result = ALooper_pollAll(-1, 0, &events, (void**)&source);
		if (result >= 0)
		{
			if (source != 0)
			{
				source->process(app, source);
			}
			if (app->destroyRequested)
			{
				Log::info("exit event loop");
				return;
			}
		}
	}
}