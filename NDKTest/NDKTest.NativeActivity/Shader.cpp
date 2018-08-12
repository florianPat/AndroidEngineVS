#include "Shader.h"
#include "Ifstream.h"
#include "Utils.h"
#include <iostream>
#include "GLUtils.h"

std::string loadShader(const std::string& filename, AAssetManager* assetManager)
{
	Ifstream file(assetManager);
	file.open(filename);
	assert(file);

	std::string result = "";
	while (!file.eof())
	{
		std::string line = "";
		file.getline(line);

		result += line + '\n';
	}

	return result;
}

void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = {};

	if (isProgram)
	{
		CallGL(glGetProgramiv(shader, flag, &success));
	}
	else
	{
		CallGL(glGetShaderiv(shader, flag, &success));
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			CallGL(glGetProgramInfoLog(shader, sizeof(error), nullptr, error));
		}
		else
		{
			CallGL(glGetShaderInfoLog(shader, sizeof(error), nullptr, error));
		}

		__android_log_write(ANDROID_LOG_ERROR, "", errorMessage.c_str());
		__android_log_print(ANDROID_LOG_ERROR, __FUNCTION__, "%s", error);
		InvalidCodePath;
	}
}

GLuint createShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = 0;
	CallGL(shader = glCreateShader(shaderType));

	if (shader == 0)
	{
		utilsLogBreak("glCreateShader failed!");
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStringLengths[0] = text.length();
	shaderSourceStrings[0] = text.c_str();

	CallGL(glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths));
	CallGL(glCompileShader(shader));

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed!");

	return shader;
}

Shader::Shader(const std::string & filename, AAssetManager* assetManager) : uniformCache()
{
	CallGL(program = glCreateProgram());

	shaders[0] = createShader(loadShader(filename + ".vs", assetManager), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(filename + ".fs", assetManager), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
	{
		CallGL(glAttachShader(program, shaders[i]));
	}

	CallGL(glBindAttribLocation(program, 0, "position"));

	CallGL(glLinkProgram(program));
	checkShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed");

	CallGL(glValidateProgram(program));
	checkShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program validating failed!");

	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
	{
		CallGL(glDetachShader(program, shaders[i]));
		CallGL(glDeleteShader(shaders[i]));
	}

	bind();
}

Shader::~Shader()
{
	CallGL(glDeleteProgram(program));
}

void Shader::bind() const
{
	CallGL(glUseProgram(program));
}

void Shader::unbind() const
{
	CallGL(glUseProgram(0));
}

void Shader::setUniform4f(const std::string & var, float v0, float v1, float v2, float v3)
{
	int loc = getUniformLoc(var);

	CallGL(glUniform4f(loc, v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string & var, const Mat4x4 & mat)
{
	int loc = getUniformLoc(var);

	CallGL(glUniformMatrix4fv(loc, 1, GL_FALSE, mat.matrix));
}

int Shader::getUniformLoc(const std::string & var)
{
	auto it = uniformCache.find(var);
	if (it != uniformCache.end())
	{
		return it->second;
	}
	else
	{
		GLint loc = 0;
		CallGL(loc = glGetUniformLocation(program, var.c_str()));
		uniformCache.emplace(std::make_pair<>(var, loc));
		return loc;
	}
}
