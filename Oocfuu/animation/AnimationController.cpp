#include "AnimationController.h"
#include "Animation.h"

void AnimationController::update()
{
	if (!m_isPlaying) return;

	m_count++;
	Animation& a = g_animations[m_animation];
	m_time = m_count / a.m_rate;
	if (m_time >= a.m_keys.size()) {
		if(a.m_next == ANIMATION_NONE){
			m_count--;
			m_time = (int)a.m_keys.size() - 1;
		} else {
			setAnimation(a.m_next);
		}
	}
}

void AnimationController::setAnimation(int _animation)
{
	m_animation = _animation;
	m_count = 
	m_time = 0;
}

void AnimationController::startAnimation()
{
	m_isPlaying = true;
}

void AnimationController::stopAnimation()
{
	m_isPlaying = false;
}
