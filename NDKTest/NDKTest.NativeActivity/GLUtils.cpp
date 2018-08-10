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
		std::cout << "OpenGL error: [" << errorCode << "] occured in function: " << func << ", line: " << __LINE__ << ", file: " __FILE__ << '\n';
	}

	if (errorOccured)
		assert(true);
}