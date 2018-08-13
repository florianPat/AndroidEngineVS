#pragma once

#include <GLES2/gl2.h>

class VertexBuffer
{
	GLuint rendererId;
public:
	VertexBuffer(const void* data, int size);
	//NOTE: Till now no need for copy assigment / constructor
	~VertexBuffer();
	void bind() const;
	void unbind() const;
};