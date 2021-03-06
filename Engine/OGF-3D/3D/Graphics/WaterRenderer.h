#pragma once

#include <vector>

#include "Core/Types.h"

#include "Core/Graphics/TextureCache.h"
#include "Core/Graphics/FramebufferCache.h"

#include "3D/Graphics/Lighting/LightSource.h"

class ShaderProgramOld;
class Texture;

class WaterRenderer {
private:
	static WaterRenderer* s_instance;

	float32 m_textureTileFactor;

	ShaderProgramOld* m_waterShader;
	
	float32 m_y;

	uint32 m_vao;
	std::vector<uint32> m_vbos;
	uint32 m_numElements;


	Texture* m_waterNormal;
	Texture* m_waterNormal2;
	Texture* m_waterDuDv;

	FramebufRef m_refractionFBO;
	TextureRef m_refractionColorTexture;
	
	WaterRenderer();
	virtual ~WaterRenderer();
public:

	void buildFBO(uint32 width, uint32 height);
	
	void prepare();
	void render(float32 x, float32 z, float32 scale);

	float32 getY() const { return m_y; };
	void bindFBO() const;

	void setLightUniforms(const LightSource& light);

	static void init() { s_instance = new WaterRenderer(); };
	static WaterRenderer* get() { return s_instance; };
	static void destroy() { delete s_instance; };
};

