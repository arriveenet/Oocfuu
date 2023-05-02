#include "KuriboStates.h"
#include "Game.h"
#include "world/Course.h"
#include "world/CourseEffect.h"
#include "sound/Sound.h"
#include <glm/glm.hpp>


using namespace glm;
using std::vector;

//================================
// Kuribo run state class.
//================================
KuriboStateRun* KuriboStateRun::instance()
{
	static KuriboStateRun instance;
	return &instance;
}

void KuriboStateRun::enter(Kuribo* _pKuribo)
{
}

void KuriboStateRun::execute(Kuribo* _pKuribo)
{
	if (Game::m_count % 12 == 0)
		_pKuribo->m_flip ^= 1;


	if (_pKuribo->m_falling)
		_pKuribo->m_speed.y += KUROBO_FALL_SPEED;

	// Squish
	bool squished = false;
	for (int i = 0; i < KURIBO_TOP_POINT_COUNT; i++) {
		if (g_player.intersect(_pKuribo->m_topPoints[i]) && !g_player.m_dead) {
			squished = true;
			g_pSound->play(SOUND_SE_SQUISH);
			g_player.jump();
			g_game.addScore(100);

			vec2 scorePosition = { _pKuribo->m_position.x, _pKuribo->m_position.y - KURIBO_HEIGHT - 8 };
			EffectScore score(scorePosition, EffectScore::SCORE_100);
			CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
			courseEffectMgr->addScore(score);

			_pKuribo->m_state = KURIBO_STATE_SQUISH;
			_pKuribo->m_pStateMachine->changeState(KuriboStateSquish::instance());
			break;
		}
	}

	// bool player dead
	if (g_player.intersect(_pKuribo->m_rightPoint)
		|| g_player.intersect(_pKuribo->m_leftPoint)
		&& (!squished)
		) {
		g_player.kill();
		//_pKuribo->m_pStateMachine->changeState(KuriboStateDie::instance());
	}

	// Kuribo falling dead
	if (_pKuribo->m_position.y > SCREEN_HEIGHT)
		_pKuribo->m_state = KURIBO_STATE_DEAD;

	_pKuribo->m_position += _pKuribo->m_speed;

	_pKuribo->m_bottomPoints.clear();

	_pKuribo->m_topPoints[0] = _pKuribo->m_position + vec2(PLAYER_SIZE / 2, 0);
	_pKuribo->m_topPoints[1] = _pKuribo->m_position + vec2(2, 5);
	_pKuribo->m_topPoints[2] = _pKuribo->m_position + vec2(14, 5);
	_pKuribo->m_rightPoint = vec2(_pKuribo->m_position.x + _pKuribo->m_size.x, _pKuribo->m_position.y + 8);
	_pKuribo->m_leftPoint = vec2(_pKuribo->m_position.x, _pKuribo->m_position.y + 8);
	_pKuribo->m_bottomPoints.push_back(_pKuribo->m_position + vec2(1, _pKuribo->m_size.y));
	_pKuribo->m_bottomPoints.push_back(_pKuribo->m_position + vec2(_pKuribo->m_size.x - 1, _pKuribo->m_size.y));

	if (g_courseManager.intersect(_pKuribo->m_rightPoint)) {
		_pKuribo->turn();
	}

	if (g_courseManager.intersect(_pKuribo->m_leftPoint)) {
		_pKuribo->turn();
	}

	_pKuribo->m_falling = true;
	if (_pKuribo->m_speed.y >= 0)
		for (vector<vec2>::iterator iter = _pKuribo->m_bottomPoints.begin();
			iter != _pKuribo->m_bottomPoints.end();
			iter++) {
		if (g_courseManager.intersect(*iter)) {
			vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
			_pKuribo->m_position.y = bottom.y - PLAYER_SIZE;
			_pKuribo->m_speed.y = 0;
			_pKuribo->m_falling = false;
			break;
		}
	}
}

void KuriboStateRun::exit(Kuribo* _pKuribo)
{
}


//================================
// Kuribo squish state class.
//================================
KuriboStateSquish* KuriboStateSquish::instance()
{
	static KuriboStateSquish instance;
	return &instance;
}

void KuriboStateSquish::enter(Kuribo* _pKuribo)
{
	_pKuribo->m_counter = 0;
}

void KuriboStateSquish::execute(Kuribo* _pKuribo)
{
	if (_pKuribo->m_counter > 32) {
		_pKuribo->m_counter = 0;
		_pKuribo->m_state = KURIBO_STATE_DEAD;
		_pKuribo->m_dead = true;
	}
}

void KuriboStateSquish::exit(Kuribo* _pKuribo)
{
}

//================================
// Kuribo die state class.
//================================
KuriboStateDie* KuriboStateDie::instance()
{
	static KuriboStateDie instance;
	return &instance;
}

void KuriboStateDie::enter(Kuribo* _pKuribo)
{
	_pKuribo->m_flip = RECT_FLIP_VERTICAL;
	g_pSound->play(SOUND_SE_KICK);
}

void KuriboStateDie::execute(Kuribo* _pKuribo)
{
	if (_pKuribo->m_position.y < SCREEN_HEIGHT) {
		_pKuribo->m_position.y += 2.4f;
	} else {
		_pKuribo->m_visible = false;
		_pKuribo->m_dead = true;
	}
}

void KuriboStateDie::exit(Kuribo* _pKuribo)
{
}
