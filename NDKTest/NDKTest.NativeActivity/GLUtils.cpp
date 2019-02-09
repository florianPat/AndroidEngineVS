#include "GLUtils.h"
#include "Utils.h"
#include <GLES2/gl2.h>

void clearErrors()
{
	while (glGetError() != GL_NO_ERROR); //not 0
}

void checkErrors(const std::string& func)
{
	bool errorOccured = false;
	while (GLenum errorCode = glGetError())
	{
		errorOccured = true;
		utils::logF("OpenGL error: [%d] occured in function: %s, line: %d, file: %s '\n'", errorCode, func.c_str(), __LINE__, __FILE__);
	}

	if (errorOccured)
		InvalidCodePath;
}