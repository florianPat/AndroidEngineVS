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
#include "RectangleShape.h"
#include <memory>
#include "CircleShape.h"
#include "View.h"
#include <SLES/OpenSLES.h>

class RenderWindow
{
public:
	enum class ViewportType
	{
		FIT,
		EXTEND
	};
private:
	android_app* app = nullptr;
	bool initFinished = false;
	bool running = true;
	Clock timeManager;

	int32_t renderWidth = 0, renderHeight = 0;
	int screenWidth = 0, screenHeight = 0;
	int viewportWidth = 0, viewportHeight = 0;
	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
	TextureAssetManager assetManager;
	std::unique_ptr<Shader> shaderSprite = nullptr;
	std::unique_ptr<Shader> shaderRectShape = nullptr;
	View view;
	Mat4x4 orhtoProj;
	ViewportType viewportType;
	
	SLObjectItf engineObj = 0;
	SLEngineItf engine = 0;
	SLObjectItf outputMix = 0;
public:
	RenderWindow(android_app* app, int width, int height, ViewportType viewportType);
	void processEvents();

	bool isOpen() const;
	void close();
	void clear();
	void draw(const Sprite& sprite);
	void draw(const RectangleShape& rect);
	void draw(const CircleShape& circle);
	void render();
	TextureAssetManager* getAssetManager();
	View& getDefaultView();
	int getViewportWidth() const;
	int getViewportHeight() const;
	int getRenderWidth() const;
	int getRenderHeight() const;

	Shader* getSpriteShader() const;
private:
	void deactivate();
	void processAppEvent(int32_t command);
	static void AppEventCallback(android_app* app, int32_t command);
	int processInputEvent(AInputEvent* event);
	static int InputEventCallback(android_app* app, AInputEvent* event);
	bool startGfx();
	void stopGfx();
	void getAndSetTouchInputPos(AInputEvent* event);
	bool startSnd();
	void stopSnd();
};