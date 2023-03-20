#pragma once
#include "Rect.h"
#include "TextureManager.h"
#include <glm/glm.hpp>

#define KOOPA_FIRE_SPEED	1.8f
#define KOOPA_FIRE_LOW		144.0f
#define KOOPA_FIRE_MIDDLE	128.0f
#define KOOPA_FIRE_HIGH		112.0f

enum FIRE_HEIGHT {
	FIRE_HEIGHT_LOW,
	FIRE_HEIGHT_MIDDLE,
	FIRE_HEIGHT_HIGH,
	FIRE_HEIGHT_MAX
};

class KoopaFire : public Rect {
	bool m_enable;
	TEXTURE m_texture;
	glm::vec2 m_speed;
	FIRE_HEIGHT m_height;

public:
	KoopaFire();
	virtual ~KoopaFire();

	void start(glm::vec2 _position, FIRE_HEIGHT _height, int _direction);
	void reset();
	void update();
	void draw();
	bool intersect(Rect const& _rect) override;

	bool isEnable()
	{
		return m_enable;
	};
};