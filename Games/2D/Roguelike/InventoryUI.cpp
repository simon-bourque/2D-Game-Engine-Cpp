#include "InventoryUI.h"
#if 0
#include "Graphics/RenderSystem.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Renderer/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "Math/Geometry/Rectangle.h"

InventoryUI::InventoryUI() : UIComponent(geo::Rectangle(1,1)), m_width(4), m_height(3) {
	//Equipment
	m_ui_helm = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_helm.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_chest = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_chest.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_glove = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_glove.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_empty = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_empty.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_boot = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_boot.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_amulet = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_amulet.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_bag = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_bag.tx", Texture::Filter::NEAREST_NEIGHBOR);

	//Inventory
	m_ui_tl = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_tl.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_t = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_t.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_tr = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_tr.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_cl = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_cl.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_c = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_c.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_cr = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_cr.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_bl = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_bl.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_b = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_b.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_ui_br = getRenderSystemInstance()->getTextureManager()->createTexture2D("ui_br.tx", Texture::Filter::NEAREST_NEIGHBOR);
}


InventoryUI::~InventoryUI() {}

void InventoryUI::render() {
	const static float32 EQUIPSIZE = 0.1875f;
	const static float32 SIZE = 0.25f;

	float32 xi = ((SIZE * m_width) / -2.0f) + (SIZE / 2.0f);
	float32 yi = ((SIZE * m_height) / 2.0f) - (SIZE / 2.0f);

	for (uint32 row = 0; row < m_height; row++) {
		for (uint32 column = 0; column < m_width; column++) {
			if (row == 0) {
				if (column == 0) {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_tl, false, false);
				}
				else if (column == m_width - 1) {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_tr, false, false);
				}
				else {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_t, false, false);
				}
			}
			else if (row == m_height - 1) {
				if (column == 0) {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_bl, false, false);
				}
				else if (column == m_width - 1) {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_br, false, false);
				}
				else {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_b, false, false);
				}
			}
			else {
				if (column == 0) {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_cl, false, false);
				}
				else if (column == m_width - 1) {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_cr, false, false);
				}
				else {
					getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + (column * SIZE), yi + (row * -SIZE), SIZE, SIZE), m_ui_c, false, false);
				}
			}
		}
	}

	getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f), EQUIPSIZE, EQUIPSIZE), m_ui_helm, false, false);
	getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_chest, false, false);
	getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - 2*EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_glove, false, false);
	getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi - ((SIZE + EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - 3*EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_boot, false, false);
	getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + 4*SIZE - ((SIZE - EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_amulet, false, false);
	getRenderSystemInstance()->getSpriteRenderer()->renderSpriteUI(geo::Rectangle(xi + 4*SIZE - ((SIZE - EQUIPSIZE) / 2.0f), yi + ((SIZE - EQUIPSIZE) / 2.0f) - 2*EQUIPSIZE, EQUIPSIZE, EQUIPSIZE), m_ui_bag, false, false);

}
#endif