#pragma once

#include <GLES2/gl2.h>

class VertexBuffer
{
	GLuint rendererId;
public:
	VertexBuffer(const void* data, int size);
	virtual ~VertexBuffer();
	void bind() const;
	void unbind() const;
};