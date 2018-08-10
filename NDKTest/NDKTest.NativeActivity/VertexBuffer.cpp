#include "VertexBuffer.h"
#include "GLUtils.h"

VertexBuffer::VertexBuffer(const void * data, int size)
{
	CallGL(glGenBuffers(1, &rendererId));
	CallGL(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
	CallGL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	CallGL(glDeleteBuffers(1, &rendererId));
}

void VertexBuffer::bind() const
{
	CallGL(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}

void VertexBuffer::unbind() const
{
	CallGL(glBindBuffer(GL_ARRAY_BUFFER, rendererId));
}
