#include "TextureCacheOld.h"

#include "3D/Graphics/Texture.h"

#define STB_IMAGE_STATIC
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

TextureCacheOld::TextureCacheOld() {}


TextureCacheOld::~TextureCacheOld() {
	for (const auto& pair : m_textures) {
		glDeleteTextures(1, &pair.second->m_texture);
		delete pair.second;
	}
	m_textures.clear();
}

Texture* TextureCacheOld::loadTexture2D(const std::string& name, const std::string& imgPath) {
	// Load image
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;
	unsigned char* data = stbi_load(imgPath.c_str(), &width, &height, &channels, 4);

	if (!data) {
		const char* reason = stbi_failure_reason();
		throw std::runtime_error("Failed to load image \'" + imgPath + "\': " + reason);
	}

	GLuint texture = 0;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	m_textures[name] = new Texture(texture, Texture::PixelDataType::TEXTURE_2D);
	return m_textures[name];
}

Texture* TextureCacheOld::loadTexture2DArray(const std::string& name, uint32 numTiles, const std::string& imgPath) {
	// Load image
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;
	unsigned char* data = stbi_load(imgPath.c_str(), &width, &height, &channels, 4);

	if (!data) {
		const char* reason = stbi_failure_reason();
		throw std::runtime_error("Failed to load image \'" + imgPath + "\': " + reason);
	}

	int32 tileHeight = height / numTiles;
	int32 tileWidth = width;

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, tileWidth, tileHeight, numTiles/*depth*/, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	stbi_image_free(data);

	m_textures[name] = new Texture(texture, Texture::PixelDataType::TEXTURE_2D_ARRAY);
	return m_textures[name];
}

Texture* TextureCacheOld::loadTextureCubeMap(const std::string& name, const CubeMapPaths& imgPaths) {
	// Load images
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;

	unsigned char* data[6];

	data[0] = stbi_load(imgPaths.bk.c_str(), &width, &height, &channels, 4);
	data[1] = stbi_load(imgPaths.ft.c_str(), &width, &height, &channels, 4);
	data[2] = stbi_load(imgPaths.up.c_str(), &width, &height, &channels, 4);
	data[3] = stbi_load(imgPaths.dn.c_str(), &width, &height, &channels, 4);
	data[4] = stbi_load(imgPaths.lf.c_str(), &width, &height, &channels, 4);
	data[5] = stbi_load(imgPaths.rt.c_str(), &width, &height, &channels, 4);

	// Error checking
	for (int32 i = 0; i < 6; i++) {
		if (!data[i]) {
			const char* reason = stbi_failure_reason();
			throw std::runtime_error("Failed to cube map image: " + std::string(reason));
		}
	}

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[5]);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Free stb data
	for (int32 i = 0; i < 6; i++) {
		stbi_image_free(data[i]);
	}

	m_textures[name] = new Texture(texture, Texture::PixelDataType::TEXTURE_CUBE_MAP);
	return m_textures[name];
}