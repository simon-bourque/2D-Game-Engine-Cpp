#include "SceneManager.h"

#include "Resource/Resources.h"

#include "Scene/TileScene.h"

SceneManager* SceneManager::s_instance = nullptr;

SceneManager::SceneManager() {
	m_currentScene.reset(new Scene(Rectangle(100,100)));
}


SceneManager::~SceneManager() {}

void SceneManager::loadTileLevel(const string& file) {
	TileScene* scene = ::loadTileLevel(file);
	m_currentScene.reset(static_cast<Scene*>(scene));
}

void SceneManager::tickCurrentScene(float32 delta) {
	m_currentScene->tick(delta);
}

void SceneManager::renderCurrentScene() {
	m_currentScene->render();
}
