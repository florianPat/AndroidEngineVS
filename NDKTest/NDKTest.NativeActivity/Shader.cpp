#include "Shader.h"
#include <fstream>
#include <assert.h>
#include <iostream>
#include "Renderer.h"

std::string loadShader(const std::string& filename)
{
	std::ifstream file(filename);
	assert(file);

	std::string result = "";
	while (!file.eof())
	{
		std::string line = "";
		std::getline(file, line);

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
		Call(glGetProgramiv(shader, flag, &success));
	}
	else
	{
		Call(glGetShaderiv(shader, flag, &success));
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			Call(glGetProgramInfoLog(shader, sizeof(error), nullptr, error));
		}
		else
		{
			Call(glGetShaderInfoLog(shader, sizeof(error), nullptr, error));
		}

		std::cerr << errorMessage << ": " << error << '\n';
	}
}

GLuint createShader(const std::string& text, GLenum shaderType)
{
	Call(GLuint shader = glCreateShader(shaderType));

	if (shader == 0)
	{
		std::cerr << "glCreateShader failed!" << '\n';
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStringLengths[0] = text.length();
	shaderSourceStrings[0] = text.c_str();

	Call(glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths));
	Call(glCompileShader(shader));

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed!");

	return shader;
}

Shader::Shader(const std::string & filename) : uniformCache()
{
	Call(program = glCreateProgram());

	shaders[0] = createShader(loadShader(filename + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = createShader(loadShader(filename + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
	{
		Call(glAttachShader(program, shaders[i]));
	}

	Call(glBindAttribLocation(program, 0, "position"));

	Call(glLinkProgram(program));
	checkShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed");

	Call(glValidateProgram(program));
	checkShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program validating failed!");

	bind();
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
	{
		Call(glDetachShader(program, shaders[i]));
		Call(glDeleteShader(shaders[i]));
	}

	Call(glDeleteProgram(program));
}

void Shader::bind() const
{
	Call(glUseProgram(program));
}

void Shader::unbind() const
{
	Call(glUseProgram(0));
}

void Shader::setUniform4f(const std::string & var, float v0, float v1, float v2, float v3)
{
	int loc = getUniformLoc(var);

	Call(glUniform4f(loc, v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string & var, const Mat4x4 & mat)
{
	int loc = getUniformLoc(var);

	Call(glUniformMatrix4fv(loc, 1, GL_FALSE, mat.matrix));
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
		Call(GLint loc = glGetUniformLocation(program, var.c_str()));
		uniformCache.emplace(std::make_pair<>(var, loc));
		return loc;
	}
}
