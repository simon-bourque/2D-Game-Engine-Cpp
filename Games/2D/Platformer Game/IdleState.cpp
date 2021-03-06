#include "IdleState.h"

#include "PlayerController.h"

#include "2D/Object/GameObject.h"

#include "Core/Core.h"
#include "2D/Event.h"


IdleState::IdleState(PlayerController* pc) : PlayerState(pc) {}

IdleState::~IdleState() {}

void IdleState::onEnter() {
	const string str("IDLE");
	m_pc->getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
}

void IdleState::onExit() {

}

void IdleState::tick(float32 delta) {

}