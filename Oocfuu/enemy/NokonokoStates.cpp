#include "NokonokoStates.h"
#include "EnemyManager.h"
#include "player/Player.h"
#include "animation/Animation.h"
#include "sound/Sound.h"
#include "common/common.h"
#include <glm/glm.hpp>

#define NOKONOKO_SPIN_SPEED	2.4f

using namespace glm;
using std::vector;

/**
* Nokonoko run state class.
*/
NokonokoStateRun* NokonokoStateRun::instance()
{
	static NokonokoStateRun instance;
	return &instance;
}

void NokonokoStateRun::enter(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_state = NOKONOKO_STATE_RUN;
	_pNokonoko->m_size = NOKONOKO_RUN_SIZE;
	_pNokonoko->m_speed = { -ENEMY_SPEED, 0.0f };
	_pNokonoko->m_flip = RECT_FLIP_NONE;
	_pNokonoko->m_animationController.setAnimation(ANIMATION_NOKONOKO_RUN);
}

void NokonokoStateRun::execute(Nokonoko* _pNokonoko)
{
	// スピードの影響を与える
	_pNokonoko->m_position += _pNokonoko->m_speed;

	// 親クラスの更新をする
	_pNokonoko->Enemy::update();
	_pNokonoko->Enemy::intersectEnemy();
}

void NokonokoStateRun::exit(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_counter = 0;
}


/**
* Nokonoko shell state class.
*/
NokonokoStateShell* NokonokoStateShell::instance()
{
	static NokonokoStateShell instance;
	return &instance;
}

void NokonokoStateShell::enter(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_state = NOKONOKO_STATE_SHELL;
	_pNokonoko->m_size = NOKONOKO_SHELL_SIZE;
	_pNokonoko->m_speed = { 0.0f, 0.0f };
	//_pNokonoko->m_position += vec2(0.f, 8.f);
	_pNokonoko->m_animationController.setAnimation(ANIMATION_NOKONOKO_SHELL);

	g_pSound->play(SOUND_TYPE_SE_SQUISH);
}

void NokonokoStateShell::execute(Nokonoko* _pNokonoko)
{
	if (_pNokonoko->m_counter++ > 240) {
		_pNokonoko->m_pStateMachine->changeState(NokonokoStateReturn::instance());
	}


	// 左か右のポイントがプレイヤーと当たればスピン状態に移行
	if (g_player.intersect(_pNokonoko->m_leftPoint)) {
		_pNokonoko->m_speed.x = NOKONOKO_SPIN_SPEED;
		_pNokonoko->m_pStateMachine->changeState(NokonokoStateSpin::instance());
	}

	if (g_player.intersect(_pNokonoko->m_rightPoint)) {
		_pNokonoko->m_speed.x = -NOKONOKO_SPIN_SPEED;
		_pNokonoko->m_pStateMachine->changeState(NokonokoStateSpin::instance());
	}

	// スピードの影響を与える
	_pNokonoko->m_position += _pNokonoko->m_speed;

	// 親クラスの更新をする
	_pNokonoko->Enemy::update();
}

void NokonokoStateShell::exit(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_counter = 0;
}

/**
* Nokonoko spin state class.
*/
NokonokoStateSpin* NokonokoStateSpin::instance()
{
	static NokonokoStateSpin instance;
	return &instance;
}

void NokonokoStateSpin::enter(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_state = NOKONOKO_STATE_SPIN;
	g_pSound->play(SOUND_TYPE_SE_KICK);
}

void NokonokoStateSpin::execute(Nokonoko* _pNokonoko)
{
	// スピードの影響を与える
	_pNokonoko->m_position += _pNokonoko->m_speed;

	// 親クラスの更新をする
	_pNokonoko->Enemy::update();

	// クリボーとの当たり判定
	vector<Kuribo>& kuriboes = g_enemyManager.getAllKuribo();
	for (vector<Kuribo>::iterator iter = kuriboes.begin();
		iter != kuriboes.end();
		++iter) {
		if (_pNokonoko->intersect(*iter)
			&& (iter->getState() != KURIBO_STATE_DEAD))
		{
			iter->kill();
			break;
		}
	}

	// ノコノコとの当たり判定
	vector<Nokonoko>& nokonokoes = g_enemyManager.getAllNokonoko();
	for (vector<Nokonoko>::iterator iter = nokonokoes.begin();
		iter != nokonokoes.end();
		++iter) {
		if (_pNokonoko->intersect(*iter)
			&& (iter->getState() != NOKONOKO_STATE_DEAD)
			&& (_pNokonoko->operator!=(*iter))
			) {
			iter->kill();
			break;
		}
	}
}

void NokonokoStateSpin::exit(Nokonoko* _pNokonoko)
{
}


/**
* Nokonoko return state class.
*/
NokonokoStateReturn* NokonokoStateReturn::instance()
{
	static NokonokoStateReturn instance;
	return &instance;
}

void NokonokoStateReturn::enter(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_state = NOKONOKO_STATE_RETURN;
	_pNokonoko->m_animationController.setAnimation(ANIMATION_NOKONOKO_RETURN);
}

void NokonokoStateReturn::execute(Nokonoko* _pNokonoko)
{
	if (_pNokonoko->m_counter++ > 120) {
		_pNokonoko->m_pStateMachine->changeState(NokonokoStateRun::instance());
	}
}

void NokonokoStateReturn::exit(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_counter = 0;
}

NokonokoStateDie* NokonokoStateDie::instance()
{
	static NokonokoStateDie instance;
	return &instance;
}

void NokonokoStateDie::enter(Nokonoko* _pNokonoko)
{
	_pNokonoko->m_state = NOKONOKO_STATE_DEAD;
	_pNokonoko->m_flip = RECT_FLIP_VERTICAL;
	g_pSound->play(SOUND_TYPE_SE_KICK);
}

void NokonokoStateDie::execute(Nokonoko* _pNokonoko)
{
	if (_pNokonoko->m_position.y < SCREEN_HEIGHT) {
		_pNokonoko->m_position.y += 2.4f;
	} else {
		_pNokonoko->m_dead = true;
	}
}

void NokonokoStateDie::exit(Nokonoko* _pNokonoko)
{
}
