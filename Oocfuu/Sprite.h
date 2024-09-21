#pragma once
#include "Actor.h"
#include "Rect.h"
#include "animation/AnimationController.h"

/**
 * スプライトクラス
 */
class Sprite : public Actor {
public:
	AnimationController m_animationController;

	Sprite();
	Sprite(const glm::vec2 _size, const glm::vec2 _position);
	virtual ~Sprite();

	void update() override;
	void draw() override;

	bool intersect(const glm::vec2& point);
	bool intersect(const Rect& rect);

	const Rect& getRect() const { return m_rect; }

	void setFlip(int flip);

	Rect m_rect;
};