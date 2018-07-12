#include "GraphicsManager.h"
#include "Log.h"

GraphicsManager::GraphicsManager(android_app * app) : app(app)
{
}

GraphicsManager::~GraphicsManager()
{
	for (int i = 0; i < elementCount; ++i)
	{
		delete elements[i];
	}
}

GraphicsElement * GraphicsManager::registerElement(int32_t width, int32_t height)
{
	elements[elementCount] = new GraphicsElement(width, height);
	return elements[elementCount++];
}

STATUS GraphicsManager::start()
{
	ANativeWindow_Buffer windowBuffer;
	if (ANativeWindow_setBuffersGeometry(app->window, 0, 0, WINDOW_FORMAT_RGBX_8888) < 0)
	{
		utilsLogBreak("Error while setting buffer geometry");
		return STATUS::KO;
	}

	if (ANativeWindow_lock(app->window, &windowBuffer, NULL) >= 0)
	{
		renderWidth = windowBuffer.width;
		renderHeight = windowBuffer.height;

		ANativeWindow_unlockAndPost(app->window);
	}
	else
	{
		utilsLogBreak("Error while locking window");
		return STATUS::KO;
	}

	return STATUS::OK;
}

STATUS GraphicsManager::update()
{
	ANativeWindow_Buffer windowBuffer;
	if (ANativeWindow_lock(app->window, &windowBuffer, NULL) < 0)
	{
		utilsLogBreak("Error while locking window in update");
		return STATUS::KO;
	}

	//Clear
	//stride is the full width`y, you know
	memset(windowBuffer.bits, 0, windowBuffer.stride * windowBuffer.height * sizeof(uint32_t));

	//Draw
	uint32_t* pixels = (uint32_t*)windowBuffer.bits;

	for (int i = 0; i < elementCount; ++i)
	{
		GraphicsElement* it = elements[i];

		assert(it != NULL);
		
		int y = it->loc.y;
		if (y < 0)
			y = 0;
		
		int x = it->loc.x;
		if (x < 0)
			x = 0;

		for (; y < windowBuffer.height || (y - it->loc.y) < it->height; ++y)
		{
			for (; x < windowBuffer.width || (x - it->loc.x) < it->width; ++x)
			{
				pixels[y * windowBuffer.stride + x] = 0xFF;
			}
		}
	}

	ANativeWindow_unlockAndPost(app->window);
	return STATUS::OK;
}
