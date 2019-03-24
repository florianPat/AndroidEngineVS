#pragma once

#include "String.h"
#include <GLES2/gl2.h>
#include <unordered_map>
#include "Mat4x4.h"
#include "android_native_app_glue.h"
#include "Vector.h"

class Shader
{
public:
	Shader() = default;
	Shader(const String& filename, const Vector<String>& attribLocs);
	Shader(const Shader& other) = delete;
	Shader(Shader&& other);
	Shader& operator=(const Shader& rhs) = delete;
	Shader& operator=(Shader&& rhs);
	explicit operator bool() const;
	~Shader();

	void bind() const;
	void unbind() const;
	void setUniform4f(const String& var, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const String& var, const Mat4x4& mat);
private:
	int getUniformLoc(const String& var);
private:
	static constexpr unsigned int NUM_SHADERS = 2;
	GLuint program = 0;
	std::unordered_map<String, GLint> uniformCache;
};