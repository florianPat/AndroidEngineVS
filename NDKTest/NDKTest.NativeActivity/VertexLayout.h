#pragma once

#include <vector>

class VertexLayouts
{
	struct VertexLayout
	{
		GLint size;
		GLenum type;
		GLboolean normalized;
	};
	GLsizei stride = 0;

	std::vector<VertexLayout> vertexLayouts;
private:
	int sizeofType(GLenum type) const;
public:
	VertexLayouts() = default;
	void addAttribute(GLint size, GLenum type);
	void set() const;
};