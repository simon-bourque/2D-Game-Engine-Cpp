#include "RoguelikeGame.h"

#include "Core/Math/Geometry/Rectangle.h"
#include "2D/Scene/SceneManager2D.h"
#include "Core/Graphics/GraphicsContext.h"
#include "2D/Graphics/Camera.h"
#include "2D/Object/Transform.h"

#include "Core/Graphics/Text/Font.h"
#include "Core/Graphics/Text/FontManager.h"
#include "Core/Graphics/Text/TextManager.h"
#include "Core/Graphics/Text/Text.h"
#include "Core/Graphics/Renderer/TextRenderer.h"
#include "Core/Graphics/Color.h"
#include "Core/Graphics/Texture.h"

#include "RoguePlayer.h"

#include "Core/Input/Input.h"

#if 0
#include "UI/UserInterface.h"
#include "UI/UIComponent.h"
#include "HealthBar.h"
#include "InventoryUI.h"
#endif

RoguelikeGame::RoguelikeGame() : Game2D("Roguelike", 720, 576, geo::Rectangle(20,20)) {}


RoguelikeGame::~RoguelikeGame() {
	delete m_ui;
}

void RoguelikeGame::init() {
	getInputInstance()->addKeyListener(KeyListener::create<RoguelikeGame, &RoguelikeGame::onKey>(this));

	getSceneManager2DInstance()->loadTileLevel("level_test2.lvl");

	m_player = new RoguePlayer();
	m_player->transform.translate(6.5f, -89);
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_player);

	m_mainFont = getGraphicsContextInstance()->getFontManager()->createFont("const");
	m_testText = getGraphicsContextInstance()->getTextManager()->createText("test_text_69", "abcdefghijklmnopqrstuvwxyz", m_mainFont, Text::Usage::STATIC);

#if 0
	m_ui = new UserInterface();
	m_invUI = new InventoryUI();
	m_invUI->setVisible(false);
	m_ui->addUIComponent(new HealthBar(m_player));
	m_ui->addUIComponent(m_invUI);
#endif
	//SoundEngine::get()->playMusic("res/sound/wily.ogg", true, musicType::FOREGROUND);
}

void RoguelikeGame::render() {
	Game2D::render();
	getGraphicsContextInstance()->getTextRenderer()->renderText(m_testText, -0.9,0.9, Color::WHITE);
#if 0
	m_ui->render();
#endif
}

void RoguelikeGame::tick(float32 delta) {
	Game2D::tick(delta);
}

void RoguelikeGame::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Keys::KEY_MINUS && action == Actions::PRESS) {
		m_player->setCurrentHealth(m_player->getCurrentHealth() - 1);
	}
	if (key == Keys::KEY_I && action == Actions::PRESS) {
		// Toggle inventory
#if 0
		m_invUI->setVisible(!m_invUI->isVisible());
#endif
		//SoundEngine::get()->playSound("res/sound/inventory.wav");
		getSceneManager2DInstance()->setPaused(!getSceneManager2DInstance()->isPaused());
	}
}