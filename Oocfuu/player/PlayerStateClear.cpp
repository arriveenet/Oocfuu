#include "PlayerStateClear.h"
#include "App.h"
#include "animation/Animation.h"
#include "world/Course.h"
#include "world/Part.h"
#include "enemy/EnemyManager.h"
#include "sound/Music.h"
#include <vector>

using namespace std;
using namespace glm;

PlayerStateClear::PlayerStateClear()
	: PlayerState(PLAYER_STATE_CLEAR)
	, m_lastSpeed(0, 0)
{
}

void PlayerStateClear::start(Player* _pPlayer)
{
	_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_RUN);
	g_courseManager.destroyBridge();
	_pPlayer->m_speed = { 0.0f, 0.0f };
}

void PlayerStateClear::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	if (g_enemyManager.koopaIsDead()) {
		_pPlayer->m_acceleration = 0.12f;
		_pPlayer->m_speed.x += _pPlayer->m_acceleration;
		_pPlayer->m_speed.y += PLAYER_GRAVITY;
		{
			static int one = 0x100;
			ivec2 fixed = _pPlayer->m_speed * (float)one;
			fixed = fixed * 0xe8 / one;
			_pPlayer->m_speed = (vec2)fixed / (float)one;
		}

		Kinopio* pKinopio = g_courseManager.getKinopio();
		if (pKinopio) {
			if (_pPlayer->intersect(*pKinopio)) {
				_pPlayer->m_speed = { 0.0f, 0.0f };
				_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
				g_courseManager.addScroll(1.0f);
			}
		}
		// ‰E‚Ì“–‚½‚è”»’è
		//for (vector<vec2>::iterator iter = _pPlayer->m_rightPoints.begin();
		//	iter != _pPlayer->m_rightPoints.end();
		//	iter++) {
		//	if (g_courseManager.intersect(*iter)) {
		//		_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
		//		g_courseManager.addScroll(1.0f);
		//		break;
		//	}
		//}
	}
}
