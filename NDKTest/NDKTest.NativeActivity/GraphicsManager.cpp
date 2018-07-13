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
	utilsLog("starting GraphicsManager");

	EGLint format, numConfigs;
	EGLConfig config;

	constexpr EGLint DISPLAY_ATTRIBS[] = {
		EGL_RENDERABLE_TYPE,
		EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	constexpr EGLint CONTEXT_ATTRIBS[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY)
	{
		utilsLogBreak("eglGetDisplay failed!");
		stop();
		return STATUS::KO;
	}

	if (!eglInitialize(display, 0, 0))
	{
		utilsLogBreak("egInitialize failed!");
		stop();
		return STATUS::KO;
	}

	if (!eglChooseConfig(display, DISPLAY_ATTRIBS, &config, 1, &numConfigs) || (numConfigs <= 0))
	{
		utilsLogBreak("eglChooseConfig failed!");
		stop();
		return STATUS::KO;
	}

	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format))
	{
		utilsLogBreak("eglGetConfigAttrib failed!");
		stop();
		return STATUS::KO;
	}

	if (ANativeWindow_setBuffersGeometry(app->window, 0, 0, format) != 0)
	{
		utilsLogBreak("ANativeWindow_setBufferGeometry failed!");
		stop();
		return STATUS::KO;
	}

	surface = eglCreateWindowSurface(display, config, app->window, nullptr);
	if (surface == EGL_NO_SURFACE)
	{
		utilsLogBreak("eglCreateWindowSurface failed!");
		stop();
		return STATUS::KO;
	}

	context = eglCreateContext(display, config, 0, CONTEXT_ATTRIBS);
	if (context == EGL_NO_CONTEXT)
	{
		utilsLogBreak("eglCreateContext failed!");
		stop();
		return STATUS::KO;
	}

	if (!eglMakeCurrent(display, surface, surface, context) || 
		!eglQuerySurface(display, surface, EGL_WIDTH, &renderWidth) || !eglQuerySurface(display, surface, EGL_HEIGHT, &renderHeight) ||
		(renderWidth <= 0) || (renderHeight <= 0))
	{
		utilsLogBreak("eglMakeCurrent failed!");
		stop();
		return STATUS::KO;
	}

	glViewport(0, 0, renderWidth, renderHeight);
	glDisable(GL_DEPTH_TEST);

	return STATUS::OK;
}

void GraphicsManager::stop()
{
	utilsLog("stopping GraphicsManager");

	if (display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(display, context);
			context = EGL_NO_CONTEXT;
		}

		if (surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(display, surface);
			surface = EGL_NO_SURFACE;
		}

		eglTerminate(display);
		display = EGL_NO_DISPLAY;
	}
}

STATUS GraphicsManager::update()
{
	static float clearColor = 0.0f;
	clearColor += 0.1f;
	glClearColor(clearColor, clearColor, clearColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (eglSwapBuffers(display, surface) != EGL_TRUE)
	{
		__android_log_assert(nullptr, __FUNCTION__, "Error %d swapping buffers!", eglGetError());
		return STATUS::KO;
	}
	else
		return STATUS::OK;
}
