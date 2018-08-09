#pragma once

#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include "Mat4x4.h"

class Shader
{
public:
	Shader(const std::string& filename);
	virtual ~Shader();

	void bind() const;
	void unbind() const;
	void setUniform4f(const std::string& var, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& var, const Mat4x4& mat);
private:
	int getUniformLoc(const std::string& var);
private:
	static constexpr unsigned int NUM_SHADERS = 2;
	GLuint program;
	GLuint shaders[NUM_SHADERS];
	std::unordered_map<std::string, GLint> uniformCache;
};