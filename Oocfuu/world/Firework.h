#pragma once
#include "Rect.h"
#include "animation\AnimationController.h"

class Firework : Rect{
	AnimationController m_ac;
	unsigned int m_textures[3];
	unsigned int m_texture;
	int m_countLeft;
public:
	bool m_explosion;

	Firework();
	int init();
	void start(glm::vec2 const& _position);
	void update();
	void draw();
};
 
extern Firework g_firework;
