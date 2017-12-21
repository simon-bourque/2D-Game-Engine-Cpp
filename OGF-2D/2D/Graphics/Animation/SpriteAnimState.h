#pragma once

#include "Core/Core.h"

#include "2D/Graphics/Animation/AnimState.h"

class Texture;
class AnimatorComponent;

class SpriteAnimState : public AnimState {
public:
	SpriteAnimState(Texture* texture);
	virtual ~SpriteAnimState();

	virtual void onEnter() override;
	virtual void tick(float32 delta) override;
	virtual void render(const AnimatorComponent& component) override;
};