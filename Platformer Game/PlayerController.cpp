#include "PlayerController.h"

#include "Core/Event.h"

#include "Input/Input.h"

#include "Object/GameObject.h"

#include "Math/Vector2f.h"

#include "Object/State/StateMachine.h"
#include "Object/State/StateTransition.h"

#include "PlayerState.h"
#include "IdleState.h"
#include "WalkState.h"
#include "InAirState.h"

const float32 PlayerController::SPEED = 9.0f;
const float32 PlayerController::JUMP_POWER = 1050.0f;

PlayerController::PlayerController(GameObject* parentObject) : ObjectComponent(parentObject),
	m_leftAction(false),
	m_rightAction(false),
	m_upAction(false),
	m_downAction(false),
	m_toggleFly(false),
	m_canJump(true),
	m_grounded(false),
	m_lastDirection(0)
{
	Input::get()->addKeyListener(this, &PlayerController::onKey);

	m_sm = new StateMachine();

	m_sm->createState<IdleState>("player_idle", this);
	m_sm->createState<WalkState>("player_walk", this);
	m_sm->createState<InAirState>("player_in_air", this);

	// Idle transitions
	m_sm->createTransition("idle_to_walk", "player_idle", "player_walk", StateTransition::createCondition(this, &PlayerController::isMovingLeftOrRight));
	m_sm->createTransition("idle_to_in_air", "player_idle", "player_in_air", StateTransition::createCondition(this, &PlayerController::isInAir));

	// Walk transitions
	m_sm->createTransition("walk_to_idle", "player_walk", "player_idle", StateTransition::createCondition(this, &PlayerController::isStandingStill));
	m_sm->createTransition("walk_to_in_air", "player_walk", "player_in_air", StateTransition::createCondition(this, &PlayerController::isInAir));

	// In Air transitions
	m_sm->createTransition("in_air_to_idle", "player_in_air", "player_idle", StateTransition::createCondition(this, &PlayerController::isOnGround));

	m_sm->setCurrentState("player_idle");
}


PlayerController::~PlayerController() {
	delete m_sm;
}

void PlayerController::tick(float32 delta) {
	m_sm->tick(delta);

	int32 direction = getDirection();

	if (direction != m_lastDirection) {
		switch (direction) {
		case -1:
			getParentObject()->broadcastEvent(Event(Event::Type::FLIP_SPRITE, true));
			break;
		case 1:
			getParentObject()->broadcastEvent(Event(Event::Type::FLIP_SPRITE, false));
			break;
		default:
			break;
		}
	}

	m_lastDirection = direction;

	m_toggleFly = false;
	m_grounded = false;
}

int32 PlayerController::getDirection() const {
	if (m_leftAction && !m_rightAction) {
		return -1;
	}
	if (!m_leftAction && m_rightAction) {
		return 1;
	}
	return 0;
}

void PlayerController::receiveEvent(const Event& event) {
	switch (event.type) {
	case Event::Type::HIT_GROUND: {
		m_grounded = event.param.asBool;
		break;
	}
	default:
		break;
	}
}

void PlayerController::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Input::KEY_A) {
		if (action == Input::PRESS)
			m_leftAction = true;
		if (action == Input::RELEASE)
			m_leftAction = false;
	}
	if (key == Input::KEY_D) {
		if (action == Input::PRESS)
			m_rightAction = true;
		if (action == Input::RELEASE)
			m_rightAction = false;
	}
	if (key == Input::KEY_S) {
		if (action == Input::PRESS)
			m_downAction = true;
		if (action == Input::RELEASE)
			m_downAction = false;
	}
	if (key == Input::KEY_W) {
		if (action == Input::PRESS)
			m_upAction = true;
		if (action == Input::RELEASE)
			m_upAction = false;
	}
	if (key == Input::KEY_SPACE && action == Input::PRESS && m_canJump) {
		static const Vector2f impulse(0, JUMP_POWER);
		getParentObject()->broadcastEvent(Event(Event::Type::APPLY_IMPULSE, &impulse));
		m_grounded = false;
	}
	if (key == Input::KEY_V) {
		if (action == Input::PRESS)
			m_toggleFly = true;
		if (action == Input::RELEASE)
			m_toggleFly = false;
	}
}

#ifdef DEBUG_BUILD
void PlayerController::debugRender() {}
#endif