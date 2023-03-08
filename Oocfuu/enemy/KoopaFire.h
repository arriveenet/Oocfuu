#pragma once
#include "Rect.h"
#include "TextureManager.h"
#include <glm/glm.hpp>

#define KOOPA_FIRE_SPEED	2.0f

class KoopaFire : public Rect {
	bool m_enable;
	TEXTURE m_texture;
	glm::vec2 m_speed;
	float m_height;

public:
	KoopaFire();
	virtual ~KoopaFire();

	void start(glm::vec2 _position, float _height, int _direction);
	void reset();
	void update();
	void draw();
	bool intersect(Rect const& _rect) override;

	bool isEnable()
	{
		return m_enable;
	};
};