#include "RenderWindow.h"
#include "TouchInput.h"
#include "Utils.h"
#include "Types.h"
#include "Utils.h"
#include "Ifstream.h"
#include "GLUtils.h"

void RenderWindow::AppEventCallback(android_app * app, int32_t command)
{
	RenderWindow& renderWindow = *(RenderWindow*)app->userData;
	renderWindow.processAppEvent(command);
}

int RenderWindow::processInputEvent(AInputEvent * event)
{
	if (!initFinished)
		return 0;

	int eventType = AInputEvent_getType(event);

	switch (eventType)
	{
		case AINPUT_EVENT_TYPE_MOTION:
		{
			int source = AInputEvent_getSource(event);
			switch (source)
			{
				case AINPUT_SOURCE_TOUCHSCREEN:
				{
					int action = AMotionEvent_getAction(event);
					if (action == AMOTION_EVENT_ACTION_MOVE || action == AMOTION_EVENT_ACTION_DOWN)
					{
						//Needs conversion because coord system is from topLeft, but game uses bottomLeft and other window dimensions
						float x = AMotionEvent_getX(event, 0);
						float y = (AMotionEvent_getY(event, 0) - screenHeight) * -1.0f;

						x = x / screenWidth * renderWidth;
						y = y / screenHeight * renderHeight;

						TouchInput::setPosition(x, y);
						TouchInput::setTouched(true);
					}
					else if (action == AMOTION_EVENT_ACTION_UP)
					{
						float x = AMotionEvent_getX(event, 0);
						float y = (AMotionEvent_getY(event, 0) - screenHeight) * -1.0f;

						x = x / screenWidth * renderWidth;
						y = y / screenHeight * renderHeight;

						TouchInput::setPosition(x, y);
						TouchInput::setTouched(true);
						TouchInput::setShouldUp(true);
					}
					else if (action == AMOTION_EVENT_ACTION_CANCEL)
					{
						TouchInput::setTouched(false);
					}
				}
				default:
				{
					break;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}

	return 0;
}

int RenderWindow::InputEventCallback(android_app * app, AInputEvent * event)
{
	RenderWindow& renderWindow = *(RenderWindow*)app->userData;
	return renderWindow.processInputEvent(event);
}

RenderWindow::RenderWindow(android_app * app, int width, int height) : app(app), timeManager(), renderWidth(width), renderHeight(height),
																	   assetManager(app->activity->assetManager),
																	   view(), orhtoProj(view.getOrthoProj())
{
	app->userData = this;
	app->onAppCmd = AppEventCallback;
	app->onInputEvent = InputEventCallback;

	processEvents();
}

void RenderWindow::processEvents()
{
	if (TouchInput::getShouldUp())
	{
		TouchInput::setShouldUp(false);
		TouchInput::setTouched(false);
	}

	int32_t events;
	android_poll_source* source;

	while (1)
	{
		while ((ALooper_pollAll(initFinished ? 0 : -1, 0, &events, (void**)&source)) >= 0)
		{
			if (source != nullptr)
			{
				source->process(app, source);
			}
			if (app->destroyRequested)
			{
				utilsLog("exit event loop!");
				running = false;
			}
		}
		if (initFinished && running)
		{
			return;
		}
	}
}

bool RenderWindow::isOpen() const
{
	return running;
}

void RenderWindow::close()
{
	utilsLog("Window close");
	running = false;
	ANativeActivity_finish(app->activity);
}

void RenderWindow::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderWindow::draw(const Sprite & sprite)
{
	const Texture* texture = sprite.getTexture();
	assert(*texture);

	texture->bind();

	float texRectLeft = sprite.getTextureRect().left / texture->getWidth();
	float texRectTop = sprite.getTextureRect().top / texture->getHeight();
	float texRectRight = sprite.getTextureRect().getRight() / texture->getWidth();
	float texRectBottom = sprite.getTextureRect().getBottom() / texture->getHeight();

	Vector2f texCoord[4] = { { texRectLeft, texRectTop },
							 { texRectRight, texRectTop },
							 { texRectRight, texRectBottom },
							 { texRectLeft, texRectBottom } };

	float vertices[] = {0.0f, 0.0f, texCoord[0].x, texCoord[0].y,
						1.0f, 0.0f, texCoord[1].x, texCoord[1].y,
						1.0f, 1.0f, texCoord[2].x, texCoord[2].y,
						0.0f, 1.0f, texCoord[3].x, texCoord[3].y };

	unsigned int indices[] = { 0, 2, 3, 0, 1, 2 };

	VertexBuffer vb = VertexBuffer(vertices, sizeof(vertices));
	IndexBuffer ib = IndexBuffer(indices, arrayCount(indices));

	vb.bind();
	
	VertexLayouts va;
	va.addAttribute(2, GL_FLOAT);
	va.addAttribute(2, GL_FLOAT);
	va.set();
	
	ib.bind();

	Mat4x4 mvp = orhtoProj * sprite.getTransform();

	Color c = sprite.getColor();

	shaderSprite->bind();
	shaderSprite->setUniformMat4f("u_mvp", mvp);
	shaderSprite->setUniform4f("u_color", c.r / 255, c.g / 255, c.b / 255, c.a / 255);

	CallGL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0));
}

void RenderWindow::draw(const RectangleShape & rect)
{
	float vertices[] = { 0.0f, 0.0f,
						 1.0f, 0.0f,
						 1.0f, 1.0f,
						 0.0f, 1.0f };

	unsigned int indices[] = { 0, 2, 3, 0, 1, 2 };

	VertexBuffer vb = VertexBuffer(vertices, sizeof(vertices));
	IndexBuffer ib = IndexBuffer(indices, arrayCount(indices));

	vb.bind();

	VertexLayouts va;
	va.addAttribute(2, GL_FLOAT);
	va.set();

	ib.bind();

	Mat4x4 mvp = orhtoProj * rect.getTransform();

	Color c = rect.getFillColor();

	shaderRectShape->bind();
	shaderRectShape->setUniformMat4f("u_mvp", mvp);
	shaderRectShape->setUniform4f("u_color", c.r / 255, c.g / 255, c.b / 255, c.a / 255);

	CallGL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0));
}

//TODO: FIX!
void RenderWindow::draw(const CircleShape & circle)
{
	//Code by: https://gist.github.com/linusthe3rd/803118
	static constexpr int triangleAmount = 20;

	float twicePi = 2.0f * PIf;

	float radius = circle.getRadius();

	float vertices[triangleAmount * 2];
	for (int i = 0, vertCounter = 0; i < triangleAmount; i++)
	{
		vertices[vertCounter++] = radius * cosf(i * twicePi / triangleAmount);
		vertices[vertCounter++] = radius * sinf(i * twicePi / triangleAmount);
	}

	VertexBuffer vb = VertexBuffer(vertices, sizeof(vertices));
	vb.bind();
	VertexLayouts va;
	va.addAttribute(2, GL_FLOAT);
	va.set();

	Mat4x4 mvp = orhtoProj * circle.getTransform();

	Color c = circle.getFillColor();

	shaderRectShape->bind();
	shaderRectShape->setUniformMat4f("u_mvp", mvp);
	shaderRectShape->setUniform4f("u_color", c.r / 255, c.g / 255, c.b / 255, c.a / 255);

	CallGL(glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(vertices)));
}

void RenderWindow::render()
{
	EGLBoolean result;
	CallGL((result = eglSwapBuffers(display, surface)));
	assert(result == EGL_TRUE);

	if (view.updated())
		orhtoProj = view.getOrthoProj();
}

TextureAssetManager * RenderWindow::getAssetManager()
{
	return &assetManager;
}

View & RenderWindow::getDefaultView()
{
	return view;
}

int RenderWindow::getScreenWdidth() const
{
	return screenWidth;
}

int RenderWindow::getScreenHeight() const
{
	return screenHeight;
}

int RenderWindow::getRenderWidth() const
{
	return renderWidth;
}

int RenderWindow::getRenderHeight() const
{
	return renderHeight;
}

void RenderWindow::deactivate()
{
	if (initFinished)
	{
		shaderSprite.reset();
		shaderRectShape.reset();

		stopGfx();
		initFinished = false;
	}
}

void RenderWindow::processAppEvent(int32_t command)
{
	switch (command)
	{
		case APP_CMD_CONFIG_CHANGED:
		{
			break;
		}
		case APP_CMD_INIT_WINDOW:
		{
			if ((!initFinished) && (app->window != NULL))
			{
				running = true;
				if (!startGfx())
				{
					running = false;
					deactivate();
					ANativeActivity_finish(app->activity);
				}
				else
				{
					//NOTE: Init graphics things here!
					shaderSprite = std::make_unique<Shader>("ShaderSprite", app->activity->assetManager, std::vector<std::string>{ "position", "texCoord" });
					shaderRectShape = std::make_unique<Shader>("ShaderRectShape", app->activity->assetManager, std::vector<std::string>{ "position" });
				}
			}

			//clock.reset();

			initFinished = true;
			break;
		}
		case APP_CMD_DESTROY:
		{
			break;
		}
		case APP_CMD_GAINED_FOCUS:
		{
			break;
		}
		case APP_CMD_LOST_FOCUS:
		{
			break;
		}
		case APP_CMD_LOW_MEMORY:
		{
			break;
		}
		case APP_CMD_PAUSE:
		{
			break;
		}
		case APP_CMD_RESUME:
		{
			break;
		}
		case APP_CMD_SAVE_STATE:
		{
			break;
		}
		case APP_CMD_START:
		{
			break;
		}
		case APP_CMD_STOP:
		{
			break;
		}
		case APP_CMD_TERM_WINDOW:
		{
			deactivate();
		}
		default:
		{
			break;
		}
	}
}

bool RenderWindow::startGfx()
{
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
		return false;
	}

	if (!eglInitialize(display, 0, 0))
	{
		utilsLogBreak("egInitialize failed!");
		return false;
	}

	if (!eglChooseConfig(display, DISPLAY_ATTRIBS, &config, 1, &numConfigs) || (numConfigs <= 0))
	{
		utilsLogBreak("eglChooseConfig failed!");
		return false;
	}

	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format))
	{
		utilsLogBreak("eglGetConfigAttrib failed!");
		return false;
	}

	if (ANativeWindow_setBuffersGeometry(app->window, 0, 0, format) != 0)
	{
		utilsLogBreak("ANativeWindow_setBufferGeometry failed!");
		return false;
	}

	surface = eglCreateWindowSurface(display, config, app->window, nullptr);
	if (surface == EGL_NO_SURFACE)
	{
		utilsLogBreak("eglCreateWindowSurface failed!");
		return false;
	}

	context = eglCreateContext(display, config, 0, CONTEXT_ATTRIBS);
	if (context == EGL_NO_CONTEXT)
	{
		utilsLogBreak("eglCreateContext failed!");
		return false;
	}

	if (!eglMakeCurrent(display, surface, surface, context) ||
		!eglQuerySurface(display, surface, EGL_WIDTH, &screenWidth) || !eglQuerySurface(display, surface, EGL_HEIGHT, &screenHeight) ||
		(screenWidth <= 0) || (screenHeight <= 0))
	{
		utilsLogBreak("eglMakeCurrent failed!");
		return false;
	}

	if (eglSwapInterval(display, 1) == EGL_FALSE)
	{
		utilsLogBreak("eglSwapInteral failed!");
		return false;
	}

	CallGL(glViewport(0, 0, screenWidth, screenHeight));

	view = View(screenWidth, screenHeight);
	orhtoProj = view.getOrthoProj();

	CallGL(glDisable(GL_DEPTH_TEST));
	CallGL(glEnable(GL_BLEND));
	CallGL(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));

	return true;
}

void RenderWindow::stopGfx()
{
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

Shader * RenderWindow::getSpriteShader() const
{
	return shaderSprite.get();
}

//GLuint RenderWindow::compileShader(const std::string& shaderFilename, const GLuint type)
//{
//	GLint result;
//	char log[256];
//	GLuint shader;
//
//	Ifstream shaderFile(app->activity->assetManager);
//	shaderFile.open(shaderFilename);
//
//	char* shaderSource = new char[shaderFile.getSize()];
//	shaderFile.getFullData(shaderSource);
//
//	CallGL(shader = glCreateShader(type));
//	CallGL(glShaderSource(shader, 1, &shaderSource, nullptr));
//
//	delete[] shaderSource;
//	shaderFile.close();
//
//	CallGL(glCompileShader(shader));
//	CallGL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
//	if (result == GL_FALSE)
//	{
//		CallGL(glGetShaderInfoLog(shader, sizeof(log), 0, log));
//		utilsLog("shader compilation failed!");
//		__android_log_assert(nullptr, __FUNCTION__, "%s", log);
//
//		if (shader > 0) CallGL(glDeleteShader(shader));
//	}
//
//	return shader;
//}
//
//void RenderWindow::linkShaders(GLuint vertexShader, GLuint fragmentShader)
//{
//	GLint result;
//	char log[256];
//
//	CallGL(shaderProgram = glCreateProgram());
//	CallGL(glAttachShader(shaderProgram, vertexShader));
//	CallGL(glAttachShader(shaderProgram, fragmentShader));
//	CallGL(glLinkProgram(shaderProgram));
//	CallGL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result));
//
//	if (result == GL_FALSE)
//	{
//		CallGL(glGetProgramInfoLog(shaderProgram, sizeof(log), nullptr, log));
//		utilsLog("ShaderProgram linking failed!");
//		__android_log_assert(nullptr, __FUNCTION__, "%s", log);
//	}
//
//	CallGL(glDetachShader(shaderProgram, vertexShader));
//	CallGL(glDeleteShader(vertexShader));
//	CallGL(glDetachShader(shaderProgram, fragmentShader));
//	CallGL(glDeleteShader(fragmentShader));
//}
//
//void RenderWindow::loadShaders(const std::string & vertexShaderFilename, const std::string & fragmentShaderFilename)
//{
//	GLuint vertexShader = compileShader(vertexShaderFilename, GL_VERTEX_SHADER);
//	GLuint fragmentShader = compileShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
//
//	linkShaders(vertexShader, fragmentShader);
//
//	CallGL(glUseProgram(shaderProgram));
//}
