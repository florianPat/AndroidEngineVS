#include "VertexArray.h"

#include "Renderer.h"

int VertexArray::sizeofType(GLenum type) const
{
	switch (type)
	{
		case GL_FLOAT:
		{
			return sizeof(GLfloat);
		}
		default:
		{
			InvalidCodePath;
			return 0;
		}
	}
}

VertexArray::VertexArray() : vertexLayouts()
{
	Call(glGenVertexArrays(1, &rendererId));
	Call(glBindVertexArray(rendererId));
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &rendererId);
}

void VertexArray::setVertexBuffer(const VertexBuffer * vb)
{
	this->vb = vb;
}

void VertexArray::setIndexBuffer(const IndexBuffer * ib)
{
	this->ib = ib;
}

void VertexArray::addAttribute(GLint size, GLenum type)
{
	VertexLayout vl = {};
	vl.size = size;
	vl.type = type;
	vl.normalized = false;

	stride += size * sizeofType(type);

	vertexLayouts.push_back(vl);
}

void VertexArray::bind() const
{
	assert(vb);
	assert(ib);
	assert(vertexLayouts.size() > 0);

	Call(glBindVertexArray(rendererId));
	vb->bind();

	int offset = 0;

	for (size_t i = 0; i < vertexLayouts.size(); ++i)
	{
		auto& it = vertexLayouts[i];

		Call(glEnableVertexAttribArray(i));
		Call(glVertexAttribPointer(i, it.size, it.type, it.normalized, stride, (void*)offset));

		offset += it.size * sizeofType(it.type);
	}

	ib->bind();
}

void VertexArray::unbind() const
{
	Call(glBindVertexArray(0));
}

int VertexArray::getIboCount() const
{
	return ib->getCount();
}
