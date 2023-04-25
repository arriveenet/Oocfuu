#pragma once
#include "Rect.h"
#include "common/Entity.h"
#include "animation/AnimationController.h"

class Sprite : public Rect, Entity{
public:
	AnimationController m_animationController;

	Sprite();
	Sprite(const glm::vec2 _size, const glm::vec2 _position);
	virtual ~Sprite();

	virtual void update() override;
	virtual void draw() override;

};