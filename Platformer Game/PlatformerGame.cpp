#include "PlatformerGame.h"

#include "Core/Core.h"

#include "Graphics/Renderer/ShapeRenderer.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Window.h"
#include "Graphics/Color.h"
#include "Graphics/Background.h"

#include "Math/Geometry/Rectangle.h"
#include "Math/Vector2f.h"

#include "Input/Input.h"

#include "Scene/SceneManager.h"

#include "Object/Component/AABBColliderComponent.h"
#include "Object/Component/ComponentType.h"
#include "Object/Component/RigidBodyComponent.h"

#include "Physics/Collision/CollisionSystem.h"
#include "Physics/Collision/QuadTree.h"

#include "Player.h"

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {}

void PlatformerGame::init() {
	Input::get()->addKeyListener(this, &PlatformerGame::onKeyPress);
	SceneManager::get()->loadTileLevel("level_0.lvl");
	SceneManager::get()->getCurrentScene().setGravity(9.8f * 4.0f);

	// Load background
	Texture* bgTexture = RenderSystem::get()->getTextureManager()->createTexture2D("uncolored_forest.tx", Texture::Filter::NEAREST_NEIGHBOR);
	SceneManager::get()->getCurrentScene().addBackground(new Background(bgTexture));

	// load player
	Player* player = new Player(this, Vector2f(0.5f, -10.8f));
	SceneManager::get()->getCurrentScene().addGameObject(player);
	m_player = player;
}

void PlatformerGame::tick(float32 delta) {
	Game::tick(delta);
}

void PlatformerGame::render() {
	Game::render();
}

void PlatformerGame::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Input::KEY_ESCAPE && action == Input::PRESS) {
		shutdown();
	}
	if (key == Input::KEY_R && action == Input::PRESS) {
		ObjectComponent* component = m_player->findComponent(RIGIDBODY_COMPONENT);
		if (component) {
			RigidBodyComponent* rb = static_cast<RigidBodyComponent*>(component);
			rb->stop();
		}

		m_player->getTransform().xPos = 0.5f;
		m_player->getTransform().yPos = -10.8f;
	}
}

