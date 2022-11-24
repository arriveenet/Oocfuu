#pragma once
#include "Rect.h"
#include "animation/AnimationController.h"

class Sprite : public Rect{
public:
	AnimationController m_animeCtr;

	Sprite();
	Sprite(glm::vec2 _size, glm::vec2 _position);
	virtual ~Sprite();

	virtual void update();
	virtual void draw();
};