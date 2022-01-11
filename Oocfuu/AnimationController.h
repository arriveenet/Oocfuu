#pragma once

struct AnimationController {
	int m_animation;
	int m_count;
	int m_time;

	void update();
	void setAnimation(int _animation);
};