#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void * data, int size)
{
	Call(glGenBuffers(1, &rendererId));
	Call(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
	Call(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	Call(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::bind() const
{
	Call(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::unbind() const
{
	Call(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}
