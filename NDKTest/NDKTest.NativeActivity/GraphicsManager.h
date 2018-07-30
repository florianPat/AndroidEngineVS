#pragma once

#include "Types.h"
#include "android_native_app_glue.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "Resource.h"

struct TexturePropeties
{
	Resource* resource;
	GLuint texture;
	int width;
	int height;
};

struct GraphicsComponent
{
	virtual STATUS load() = 0;
	virtual void draw() = 0;
};

class GraphicsManager
{
public:
	GraphicsManager(android_app* app);
	~GraphicsManager();

	int32_t getRenderWidth() const { return renderWidth; }
	int32_t getRenderHeight() const { return renderHeight; }
	GLfloat* getProjectionMatrix() const { return (GLfloat*) &projectionMatrix[0]; }

	void registerComponent(GraphicsComponent* component);

	STATUS start();
	void stop();
	STATUS update();

	TexturePropeties* loadTexture(Resource& resource);
	GLuint loadShader(const char* vertexShader, const char* fragmentShader);
private:
	struct RenderVertex
	{
		GLfloat x, y, u, v;
	};

	android_app * app;
	int32_t renderWidth = 0, renderHeight = 0;

	TexturePropeties textures[32] = {};
	int32_t textureCount = 0;

	GLfloat projectionMatrix[4][4] = {};

	GLuint shaders[32];
	int32_t shaderCount = 0;

	GraphicsComponent* components[32];
	int32_t componentCount = 0;

	EGLDisplay display = EGL_NO_DISPLAY;
	EGLSurface surface = EGL_NO_SURFACE;
	EGLContext context = EGL_NO_CONTEXT;
};