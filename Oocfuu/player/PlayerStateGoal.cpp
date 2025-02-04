#include "PlayerStateGoal.h"
#include "sound/Sound.h"
#include "sound/Bgm.h"
#include "animation/Animation.h"
#include "world/CourseManager.h"
#include "world/CourseEffect.h"
#include "world/Part.h"

#include <vector>
#include <glm/glm.hpp>

#define PLAYER_POLE_OFFSET			9.0f
#define PLAYER_FLIP_OFFSET			14.0f
#define PLAYER_GOAL_MOVE_SPEED		0.8f

using namespace std;
using namespace glm;

PlayerStateGoal::PlayerStateGoal()
	: PlayerState(PLAYER_STATE_GOAL)
	, m_state(PLAYERSTATEGOAL_FALL)
	, m_counter(0)
{
}

void PlayerStateGoal::start(Player* _pPlayer)
{
	m_state = PLAYERSTATEGOAL_FALL;
	m_counter = 0;
	_pPlayer->m_speed.x = 0;
	auto pos = _pPlayer->getPosition();
	_pPlayer->setPosition(pos.x + PLAYER_POLE_OFFSET, pos.y);
	_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_FLAG_POLE);

	Bgm::stop();
	g_pSound->play(SOUND_TYPE_SE_FLAGPOLE);

	CourseEffectManager* effectMgr = CourseEffectManager::instance();
	effectMgr->startGoalFlag();
}

void PlayerStateGoal::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	m_counter++;

	switch (m_state) {
	case PLAYERSTATEGOAL_FALL:
		if (m_counter > 76) {
			m_state = PLAYERSTATEGOAL_FLIP;
			m_counter = 0;
			_pPlayer->setFlip(RECT_FLIP_HORIZONTAL);
			auto pos = _pPlayer->getPosition();
			_pPlayer->setPosition(pos.x + PLAYER_FLIP_OFFSET, pos.y);
			_pPlayer->m_speed.y = 0;
            _pPlayer->m_animationController.stopAnimation();
		} else {
  			_pPlayer->m_speed.y = PLAYER_POLE_FALL_SPEED;
		}
		break;
	case PLAYERSTATEGOAL_FLIP:
		if (m_counter > 32) {
			m_state = PLAYERSTATEGOAL_MOVE;
			g_pSound->setGain(SOUND_TYPE_SE_FANFARE, 0.1f);
			g_pSound->play(SOUND_TYPE_SE_FANFARE);
			_pPlayer->setFlip(RECT_FLIP_NONE);
			_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_RUN);
			_pPlayer->m_animationController.startAnimation();
		}
		break;
	case PLAYERSTATEGOAL_MOVE:
		_pPlayer->m_speed.x = PLAYER_GOAL_MOVE_SPEED;
		_pPlayer->m_speed.y += PLAYER_GRAVITY;
		break;
	case PLAYERSTATEGOAL_WAIT:
		if (m_counter > 60 * 3 && (Game::getInstance()->m_timer.getTime() == 0)) {
			Game::getInstance()->m_world = g_courseManager.getNextWorld();
			Game::getInstance()->setScreen(GAME_SCREEN_INTRO);
		}
		break;
	}

	// 右の当たり判定
	if (m_state == PLAYERSTATEGOAL_MOVE) {
		for (vector<vec2>::iterator iter = _pPlayer->m_rightPoints.begin();
			iter != _pPlayer->m_rightPoints.end();
			iter++) {
			if (g_courseManager.intersect(*iter)) {
				vec2 right = (ivec2)*iter / PART_SIZE * PART_SIZE;
				float posY = _pPlayer->getPosition().y;
				_pPlayer->setPosition(right.x - PLAYER_SIZE, posY);
				_pPlayer->m_speed.x = 0;
				_pPlayer->m_falling = true;
				m_state = PLAYERSTATEGOAL_WAIT;
				Game::getInstance()->m_timer.countDown();
				m_counter = 0;
				_pPlayer->m_visible = false;
				break;
			}
		}
	}

	//for (vector<vec2>::iterator iter = _pPlayer->m_bottomPoints.begin();
	//	iter != _pPlayer->m_bottomPoints.end();
	//	iter++) {

	//	if (g_courseManager.intersect(*iter)) {
	//		vec2 bottom = ((ivec2)*iter / PART_SIZE) * PART_SIZE;
	//		_pPlayer->m_position.y = bottom.y - PLAYER_SIZE;
	//		_pPlayer->m_speed.y = 0;
	//		_pPlayer->m_jumping = false;
	//		_pPlayer->m_falling = false;
	//		break;
	//	}
	//}
}
