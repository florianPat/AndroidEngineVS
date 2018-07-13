#pragma once

#include "Types.h"
#include "android_native_app_glue.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>

struct GraphicsElement
{
	GraphicsElement(int32_t width, int32_t height) : loc(), width(width), height(height) {}
public:
	Location loc;
	int32_t	width, height;
};

class GraphicsManager
{
public:
	GraphicsManager(android_app* app);
	~GraphicsManager();

	int32_t getRenderWidth() const { return renderWidth; }
	int32_t getRenderHeight() const { return renderHeight; }

	GraphicsElement* registerElement(int32_t width, int32_t height);

	STATUS start();
	void stop();
	STATUS update();
private:
	android_app * app;
	int32_t renderWidth = 0, renderHeight = 0;
	GraphicsElement* elements[1024] = {};
	int32_t elementCount = 0;

	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
};