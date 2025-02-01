#pragma once

struct AnimationController {
	int m_animation;
	int m_count;
	int m_time;
	bool m_isPlaying = true;

	void update();
	void setAnimation(int _animation);
	void startAnimation();
	void stopAnimation();
};