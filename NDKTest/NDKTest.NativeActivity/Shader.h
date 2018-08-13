#pragma once

#include <string>
#include <GLES2/gl2.h>
#include <unordered_map>
#include "Mat4x4.h"
#include "android_native_app_glue.h"
#include <vector>

class Shader
{
public:
	//NOTE: I just use it as a unique_ptr, because othervise copy / destructor stuff is not nice
	Shader() = default;
	Shader(const std::string& filename, AAssetManager* assetManager, const std::vector<std::string>& attribLocs);
	~Shader();

	void bind() const;
	void unbind() const;
	void setUniform4f(const std::string& var, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& var, const Mat4x4& mat);
private:
	int getUniformLoc(const std::string& var);
private:
	static constexpr unsigned int NUM_SHADERS = 2;
	GLuint program = 0;
	GLuint shaders[NUM_SHADERS];
	std::unordered_map<std::string, GLint> uniformCache;
};