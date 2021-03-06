#include "SpriteRenderer.h"

#include "Core/Graphics/Texture.h"
#include "Core/Graphics/Shader/ShaderProgram.h"
#include "Core/Graphics/Memory/VertexArrayObject.h"
#include "Core/Graphics/Memory/Buffer.h"
#include "Core/Graphics/GraphicsContext.h"

#include "Core/Math/Geometry/Rectangle.h"

#include "2D/Object/Transform.h"

#include "Core/Resource/Resources.h"

#include "2D/Graphics/Graphics2D.h"

#include <GL/glew.h>

const float32 SpriteRenderer::SPRITE_VERTS[8] = {
	-0.5f,-0.5f,
	-0.5f,0.5f,
	0.5f,-0.5f,
	0.5f,0.5f
};

const float32 SpriteRenderer::SPRITE_UV[8] = { 
	0.0f,1.0f,
	0.0f,0.0f,
	1.0f,1.0f,
	1.0f,0.0f
};

const int32 SpriteRenderer::SPRITE_NUM_VERTICES = 4;

SpriteRenderer::SpriteRenderer() {
	m_spriteShaderProgram.reset(new ShaderProgram(loadSrc("default_vert.shader"), loadSrc("sprite_frag.shader")));
	m_animSpriteShaderProgram.reset(new ShaderProgram(loadSrc("default_vert.shader"), loadSrc("animsprite_frag.shader")));
	m_spriteBatchShaderProgram.reset(new ShaderProgram(loadSrc("instance_vert.shader"), loadSrc("instance_frag.shader")));

	m_spriteVAO.reset(new VertexArrayObject());
	m_spriteVAO->bind();
	m_spriteVAO->addArrayBuffer(0, SPRITE_VERTS, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_spriteVAO->addArrayBuffer(1, SPRITE_UV, sizeof(float32) * 8, 2, GL_FLOAT, Buffer::Usage::STATIC_DRAW);
	m_spriteVAO->unbind();
}


SpriteRenderer::~SpriteRenderer() {}

void SpriteRenderer::renderSprite(const Transform* transform, TextureRef textureRef, bool hFlip, bool vFlip) const {
	glUseProgram(m_spriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	Texture* texture = getGraphicsContextInstance()->getTextureCache()->getTexture(textureRef);
	texture->bind(Texture::Unit::UNIT_0);

	Matrix3f finalMatrix = getGraphics2DInstance()->getCamera().getViewProjectionMatrix() * transform->toMatrix();

	glUniformMatrix3fv(m_spriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);
	glUniform1i(m_spriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("sprite").getLocation(), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);

}

void SpriteRenderer::renderSpriteUI(const geo::Rectangle& bounds, TextureRef textureRef, bool hFlip, bool vFlip) const {
	glUseProgram(m_spriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	Texture* texture = getGraphicsContextInstance()->getTextureCache()->getTexture(textureRef);
	texture->bind(Texture::Unit::UNIT_0);

	Matrix3f finalMatrix = Matrix3f::translation(bounds.getX(), bounds.getY()) * Matrix3f::scale(bounds.getWidth(), bounds.getHeight());
	//Matrix3f finalMatrix = m_rs->getCamera().getViewProjectionMatrix() * transform->toMatrix();

	glUniformMatrix3fv(m_spriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);
	glUniform1i(m_spriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_spriteShaderProgram->getUniform("sprite").getLocation(), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);

}

void SpriteRenderer::renderAnimationFrame(const Transform& transform, uint32 frame, TextureRef textureRef, bool hFlip, bool vFlip) const {
	glUseProgram(m_animSpriteShaderProgram->getProgramId());

	m_spriteVAO->bind();

	Matrix3f finalMatrix = getGraphics2DInstance()->getCamera().getViewProjectionMatrix() * transform.toMatrix();

	// Diffuse Texture
	Texture* texture = getGraphicsContextInstance()->getTextureCache()->getTexture(textureRef);
	texture->bind(Texture::Unit::UNIT_0);

	glUniformMatrix3fv(m_animSpriteShaderProgram->getUniform("mvpMatrix").getLocation(), 1, true, finalMatrix.values);
	glUniform1i(m_animSpriteShaderProgram->getUniform("diffuseTextureAtlas").getLocation(), 0);
	glUniform1i(m_animSpriteShaderProgram->getUniform("currentFrame").getLocation(), frame);
	glUniform1i(m_animSpriteShaderProgram->getUniform("horizontalFlip").getLocation(), (hFlip) ? GL_TRUE : GL_FALSE);
	glUniform1i(m_animSpriteShaderProgram->getUniform("verticalFlip").getLocation(), (vFlip) ? GL_TRUE : GL_FALSE);


	glDrawArrays(GL_TRIANGLE_STRIP, 0, SPRITE_NUM_VERTICES);
}
