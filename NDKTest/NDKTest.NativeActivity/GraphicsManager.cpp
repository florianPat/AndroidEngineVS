#include "GraphicsManager.h"
#include "Log.h"

#include <png.h>

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

	for (int i = 0; i < textureCount; ++i)
	{
		glDeleteTextures(1, &textures[i].texture);
	}
	textureCount = 0;

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

void callback_readPng(png_structp truct, png_bytep data, png_size_t size)
{
	Resource* resource = (Resource*)png_get_io_ptr(truct);
	if (resource->read(data, size) != STATUS::OK)
	{
		resource->close();
		utilsLogBreak("Resource could not be loaded!");
	}
}

TexturePropeties * GraphicsManager::loadTexture(Resource & resource)
{
	for (int i = 0; i < textureCount; ++i)
	{
		if (resource == *textures[i].resource)
		{
			utilsLog("Found texture in cache!");
			return &textures[i];
		}
	}

	utilsLog("Loading texture");
	TexturePropeties* texturePropeties = nullptr;
	GLuint texture;
	GLint format;
	png_byte header[8];
	png_structp png = nullptr;
	png_infop info = nullptr;
	png_byte* image = nullptr;
	png_bytep* rowPtrs = nullptr;
	png_int_32 rowSize;
	bool transparency;

	if (resource.open() != STATUS::OK)
	{
		utilsLogBreak("Failed to open resource");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}
	if (resource.read(header, sizeof(header)) != STATUS::OK)
	{
		utilsLogBreak("Failed to read header!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}
	if (png_sig_cmp(header, 0, 8) != 0)
	{
		utilsLogBreak("False signiture!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png)
	{
		utilsLogBreak("png_create_read_struct failed!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}
	info = png_create_info_struct(png);
	if (!info)
	{
		utilsLogBreak("png_create_info_struct failed!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}

	png_set_read_fn(png, &resource, callback_readPng);

	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	png_int_32 depth, colorType;
	png_uint_32 width, height;
	png_get_IHDR(png, info, &width, &height, &depth, &colorType, nullptr, nullptr, nullptr);

	transparency = false;
	if (png_get_valid(png, info, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png);
		transparency = true;
	}

	if (depth < 8)
	{
		png_set_packing(png);
	}
	else if (depth == 16)
	{
		png_set_strip_16(png);
	}

	switch (colorType)
	{
		case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(png);
			format = transparency ? GL_RGBA : GL_RGB;
			break;
		}
		case PNG_COLOR_TYPE_RGB:
		{
			format = transparency ? GL_RGBA : GL_RGB;
			break;
		}
		case PNG_COLOR_TYPE_RGBA:
		{
			format = GL_RGBA;
			break;
		}
		case PNG_COLOR_TYPE_GRAY:
		{
			png_set_expand_gray_1_2_4_to_8(png);
			format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			break;
		}
		case PNG_COLOR_TYPE_GA:
		{
			png_set_expand_gray_1_2_4_to_8(png);
			format = GL_LUMINANCE_ALPHA;
			break;
		}
	}
	png_read_update_info(png, info);

	rowSize = png_get_rowbytes(png, info);
	if (rowSize <= 0)
	{
		utilsLogBreak("png_get_rowbytes failed!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}

	image = new png_byte[rowSize * height];
	if (!image)
	{
		utilsLogBreak("image creation failed!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}

	rowPtrs = new png_bytep[height];
	if (!rowPtrs)
	{
		utilsLogBreak("rowPtrs creation failed!");
		resource.close();
		delete[] rowPtrs; delete[] image;
		if (png != nullptr) {
			png_infop* infoP = info != nullptr ? &info : nullptr;
			png_destroy_read_struct(&png, infoP, nullptr);
		}
		return nullptr;
	}

	for (uint32_t i = 0; i < height; ++i)
	{
		rowPtrs[height - (i + 1)] = image + i * rowSize;
	}
	png_read_image(png, rowPtrs);

	resource.close();
	png_destroy_read_struct(&png, &info, nullptr);
	delete[] rowPtrs;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set-up texture properties.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);

	// Loads image data into OpenGL.
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
		GL_UNSIGNED_BYTE, image);
	// Finished working with the texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] image;
	if (glGetError() != GL_NO_ERROR)
	{
		utilsLogBreak("OpenGl error occured!");
	}

	texturePropeties = &textures[textureCount++];
	texturePropeties->texture = texture;
	texturePropeties->resource = &resource;
	texturePropeties->width = width;
	texturePropeties->height = height;
	
	return texturePropeties;
}