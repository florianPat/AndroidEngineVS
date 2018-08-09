#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(unsigned int * indices, int count) : count(count)
{
	Call(glGenBuffers(1, &rendererId));
	Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
	Call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	Call(glDeleteBuffers(1, &rendererId));
}

void IndexBuffer::bind() const
{
	Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId));
}

void IndexBuffer::unbind() const
{
	Call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

int IndexBuffer::getCount() const
{
	return count;
}
