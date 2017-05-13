#include "AABBColliderComponent.h"

#include "Object/GameObject.h"

#include "Core/Game.h"
#include "Core/Event.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Physics/Collision/CollisionSystem.h"

#ifdef DEBUG_BUILD
#include "Core/Debug.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Color.h"
#include "Graphics/Renderer/ShapeRenderer.h"
#endif

void AABBColliderComponent::emptyIntersectResponse(const Manifold& manifold, GameObject* other) {}

AABBColliderComponent::AABBColliderComponent(GameObject* parentObject, const Rectangle& rectangle, float32 xOffset, float32 yOffset) : ObjectComponent(parentObject),
	m_rectangle(rectangle),
	m_xOffset(xOffset),
	m_yOffset(yOffset)
{
	m_response = new Delegate<AABBColliderComponent, const Manifold&, GameObject*>(this, &AABBColliderComponent::emptyIntersectResponse);
}


AABBColliderComponent::~AABBColliderComponent() {
	delete m_response;
}

void AABBColliderComponent::tick(float32 delta) {
	resetPosition();
	SceneManager::get()->getCurrentScene().getCollisionSystem()->addCollider(this);
}

void AABBColliderComponent::resetPosition() {
	const Transform& t = getParentObject()->getTransform();

	m_rectangle.setX(t.xPos + m_xOffset);
	m_rectangle.setY(t.yPos + m_yOffset);
}

void AABBColliderComponent::onIntersectLevel(const Manifold& manifold) {
	
	Vector2f delta = manifold.direction * manifold.depth;
	getParentObject()->getTransform().translate(delta.x, delta.y);
	getParentObject()->broadcastEvent(Event(Event::Type::COLLISION_LEVEL, static_cast<const void*>(&manifold)));
}

void AABBColliderComponent::onIntersectObject(const Manifold& manifold, GameObject* other) {
	(*m_response)(manifold, other);
}

void AABBColliderComponent::receiveEvent(const Event& event) {}

#ifdef DEBUG_BUILD
void AABBColliderComponent::debugRender() {
	if (Debug::get()->shouldRenderColliders()) {
		RenderSystem::get()->getShapeRenderer()->drawRectangle(m_rectangle, Color::MAGENTA, false);
	}
}
#endif