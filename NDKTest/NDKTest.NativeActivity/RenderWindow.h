#pragma once

#include "android_native_app_glue.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Clock.h"
#include <string>

void clearErrors();
void checkErrors(const std::string& func);

#define CallGL(x) do { clearErrors(); x; checkErrors(#x); } while(false)

class RenderWindow
{
	android_app * app;
	bool enabled = false;
	bool quit = false;
	Clock timeManager;

	int32_t renderWidth = 0, renderHeight = 0;
	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
	GLuint shaderProgram;
public:
	RenderWindow(android_app* app, int width, int height);
	void processEvents();

	bool isOpen() const;
	void close();
	void clear();
	void render();
private:
	void deactivate();
	void processAppEvent(int32_t command);
	static void AppEventCallback(android_app* app, int32_t command);
	bool startGfx();
	void stopGfx();
	GLuint compileShader(const std::string& shaderFilename, const GLuint type);
	void linkShaders(GLuint vertexShader, GLuint fragmentShader);
	void loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
};