#pragma once
#if 0
#include "UI/UIComponent.h"

class Texture;

class InventoryUI : public UIComponent {
private:
	Texture* m_ui_helm;
	Texture* m_ui_chest;
	Texture* m_ui_glove;
	Texture* m_ui_empty;
	Texture* m_ui_boot;
	Texture* m_ui_amulet;
	Texture* m_ui_bag;
	
	Texture* m_ui_tl;
	Texture* m_ui_t;
	Texture* m_ui_tr;
	
	Texture* m_ui_cl;
	Texture* m_ui_c;
	Texture* m_ui_cr;

	Texture* m_ui_bl;
	Texture* m_ui_b;
	Texture* m_ui_br;

	uint32 m_width;
	uint32 m_height;
public:
	InventoryUI();
	virtual ~InventoryUI();

	virtual void render() override;
};

#endif