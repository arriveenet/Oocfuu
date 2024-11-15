#include "KuriboStates.h"
#include "Game.h"
#include "world/CourseManager.h"
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
	if (Game::m_count % 12 == 0) {
		const int flip = _pKuribo->isFlipX() ? RECT_FLIP_NONE : RECT_FLIP_HORIZONTAL;
		_pKuribo->setFlip(flip);
	}


	// Squish
	bool squished = false;
	for (int i = 0; i < KURIBO_TOP_POINT_COUNT; i++) {
		if (g_player.intersect(_pKuribo->m_topPoints[i]) && !g_player.m_dead) {
			squished = true;
			g_pSound->play(SOUND_TYPE_SE_SQUISH);
			g_player.jump();
			Game::getInstance()->addScore(100);

			vec2 scorePosition = { _pKuribo->m_position.x, _pKuribo->m_position.y - KURIBO_HEIGHT - 8 };
			EffectScore score(scorePosition, EffectScore::SCORE_100);
			CourseEffectManager* courseEffectMgr = CourseEffectManager::instance();
			courseEffectMgr->addScore(score);

			_pKuribo->m_state = KURIBO_STATE_SQUISH;
			_pKuribo->m_pStateMachine->changeState(KuriboStateSquish::instance());
			break;
		}
	}

	_pKuribo->m_hitRightPoint = vec2(_pKuribo->m_position.x + _pKuribo->m_size.x, _pKuribo->m_position.y + 8);
	_pKuribo->m_hitLeftPoint = vec2(_pKuribo->m_position.x, _pKuribo->m_position.y + 8);

	// bool player dead
	if (g_player.intersect(_pKuribo->m_hitRightPoint)
		|| g_player.intersect(_pKuribo->m_hitLeftPoint)
		&& (!squished)
		) {
		g_player.kill();
		//_pKuribo->m_pStateMachine->changeState(KuriboStateDie::instance());
	}

	// Kuribo falling dead
	if (_pKuribo->m_position.y > SCREEN_HEIGHT)
		_pKuribo->m_state = KURIBO_STATE_DEAD;

	_pKuribo->m_position += _pKuribo->m_speed;


	_pKuribo->m_topPoints[0] = _pKuribo->m_position + vec2(PLAYER_SIZE / 2, 0);
	_pKuribo->m_topPoints[1] = _pKuribo->m_position + vec2(2, 5);
	_pKuribo->m_topPoints[2] = _pKuribo->m_position + vec2(14, 5);

	//if (g_courseManager.intersect(_pKuribo->m_rightPoint)) {
	//	_pKuribo->turn();
	//}

	//if (g_courseManager.intersect(_pKuribo->m_leftPoint)) {
	//	_pKuribo->turn();
	//}

	_pKuribo->Enemy::update();
	_pKuribo->Enemy::intersectEnemy();
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
	_pKuribo->m_state = KURIBO_STATE_SQUISH;
	_pKuribo->m_texture = TEXTURE_KURIBO_SQUISH;
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
	_pKuribo->m_state = KURIBO_STATE_DEAD;
	_pKuribo->m_texture = TEXTURE_KURIBO_RUN;
	_pKuribo->setFlip(RECT_FLIP_VERTICAL);

	g_pSound->play(SOUND_TYPE_SE_KICK);
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
