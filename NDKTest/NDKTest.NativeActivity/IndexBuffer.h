#pragma once

#include <GLES2/gl2.h>

class IndexBuffer
{
	GLuint rendererId;
	int count;
public:
	IndexBuffer(unsigned int* indices, int count);
	~IndexBuffer();
	void bind() const;
	void unbind() const;
	int getCount() const;
};