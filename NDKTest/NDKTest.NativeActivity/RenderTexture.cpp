#include "RenderTexture.h"
#include "GLUtils.h"
#include "Utils.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

bool RenderTexture::create(uint width, uint height, Shader* shaderSpriteIn)
{
	shaderSprite = shaderSpriteIn;

	CallGL(glGenFramebuffers(1, &renderTexture));
	CallGL(glBindFramebuffer(GL_FRAMEBUFFER, renderTexture));

	CallGL(glGenTextures(1, &texture.texture));
	CallGL(glActiveTexture(GL_TEXTURE0));
	CallGL(glBindTexture(GL_TEXTURE_2D, texture.texture));

	CallGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	CallGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	CallGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	CallGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	CallGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

	CallGL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.texture, 0));
	
	texture.width = width;
	texture.height = height;
	orhtoProj = Mat4x4::orthoProj(-1.0f, 1.0f, 0.0f, 0.0f, (float)texture.width, (float)texture.height);

	GLenum result;
	CallGL(result = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	CallGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	return true;
}

void RenderTexture::clear()
{
	CallGL(glBindFramebuffer(GL_FRAMEBUFFER, renderTexture));

	CallGL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	CallGL(glClear(GL_COLOR_BUFFER_BIT));

	CallGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

const Texture & RenderTexture::getTexture() const
{
	return texture;
}

void RenderTexture::display()
{
}

void RenderTexture::draw(const Sprite & sprite)
{
	CallGL(glBindFramebuffer(GL_FRAMEBUFFER, renderTexture));

	const Texture* texture = sprite.getTexture();
	assert(*texture);

	texture->bind();

	float texRectLeft = sprite.getTextureRect().left / texture->getWidth();
	float texRectTop = sprite.getTextureRect().top / texture->getHeight();
	float texRectRight = sprite.getTextureRect().getRight() / texture->getWidth();
	float texRectBottom = sprite.getTextureRect().getBottom() / texture->getHeight();

	Vector2f texCoord[4] = { { texRectLeft, texRectTop },
	{ texRectRight, texRectTop },
	{ texRectRight, texRectBottom },
	{ texRectLeft, texRectBottom } };

	float vertices[] = { -0.5f, -0.5f, texCoord[0].x, texCoord[0].y,
		0.5f, -0.5f, texCoord[1].x, texCoord[1].y,
		0.5f, 0.5f, texCoord[2].x, texCoord[2].y,
		-0.5f, 0.5f, texCoord[3].x, texCoord[3].y };

	unsigned int indices[] = { 0, 2, 3, 0, 1, 2 };

	VertexBuffer vb = VertexBuffer(vertices, sizeof(vertices));
	IndexBuffer ib = IndexBuffer(indices, arrayCount(indices));

	vb.bind();

	VertexLayouts va;
	va.addAttribute(2, GL_FLOAT);
	va.addAttribute(2, GL_FLOAT);
	va.set();

	ib.bind();

	Mat4x4 mvp = orhtoProj * sprite.getTransform();

	Color c = sprite.getColor();

	shaderSprite->bind();
	shaderSprite->setUniformMat4f("u_mvp", mvp);
	shaderSprite->setUniform4f("u_color", c.r / 255, c.g / 255, c.b / 255, c.a / 255);

	CallGL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0));

	CallGL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
