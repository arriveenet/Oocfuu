#pragma once
#include "Rect.h"
#include "common/Entity.h"
#include "animation/AnimationController.h"

/**
 * スプライトクラス
 */
class Sprite : public Rect, Entity{
public:
	AnimationController m_animationController;

	Sprite();
	Sprite(const glm::vec2 _size, const glm::vec2 _position);
	virtual ~Sprite();

	void update() override;
	void draw() override;

};