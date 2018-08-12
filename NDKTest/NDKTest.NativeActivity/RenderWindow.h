#pragma once

#include "android_native_app_glue.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Clock.h"
#include <string>
#include "Sprite.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"
#include "Shader.h"
#include "Mat4x4.h"
#include "AssetManager.h"

class RenderWindow
{
	android_app* app = nullptr;
	bool initFinished = false;
	bool running = true;
	Clock timeManager;

	int32_t renderWidth = 0, renderHeight = 0;
	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
	TextureAssetManager assetManager;
	Shader shader;
	Mat4x4 orhtoProj;
public:
	RenderWindow(android_app* app, int width, int height);
	void processEvents();

	bool isOpen() const;
	void close();
	void clear();
	void draw(const Sprite& sprite);
	void render();
	TextureAssetManager* getAssetManager();
private:
	void deactivate();
	void processAppEvent(int32_t command);
	static void AppEventCallback(android_app* app, int32_t command);
	bool startGfx();
	void stopGfx();
};