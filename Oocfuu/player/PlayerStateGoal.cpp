#include "PlayerStateGoal.h"
#include "sound/Sound.h"
#include "animation/Animation.h"
#include "world/Course.h"
#include "world/Part.h"

#include <vector>
#include <glm/glm.hpp>

#define PLAYER_POLE_OFFSET			9.0f
#define PLAYER_POLE_FALL_SPEED		1.6f
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
	_pPlayer->m_position.x += PLAYER_POLE_OFFSET;
	g_pSound->play(SOUND_FLAGPOLE);
}

void PlayerStateGoal::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	m_counter++;

	switch (m_state) {
	case PLAYERSTATEGOAL_FALL:
		if (m_counter > 64) {
			m_state = PLAYERSTATEGOAL_FLIP;
			m_counter = 0;
			_pPlayer->m_flip = RECT_FLIP_HORIZONTAL;
			_pPlayer->m_position.x += PLAYER_FLIP_OFFSET;
			_pPlayer->m_speed.y = 0;
		} else {
  			_pPlayer->m_speed.y = PLAYER_POLE_FALL_SPEED;
		}
		break;
	case PLAYERSTATEGOAL_FLIP:
		if (m_counter > 32) {
			m_state = PLAYERSTATEGOAL_MOVE;
			g_pSound->setGain(SOUND_FANFARE, 0.1f);
			g_pSound->play(SOUND_FANFARE);
			_pPlayer->m_flip = RECT_FLIP_NONE;
			_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_RUN);
		}
		break;
	case PLAYERSTATEGOAL_MOVE:
		_pPlayer->m_speed.x = PLAYER_GOAL_MOVE_SPEED;
		_pPlayer->m_speed.y += PLAYER_GRAVITY;
		break;
	case PLAYERSTATEGOAL_WAIT:
		if (m_counter > 60 * 3 && (g_game.m_timer.getTime() == 0)) {
			g_game.m_world = g_courseManager.getNextWorld();
			g_game.setScreen(GAME_SCREEN_INTRO);
		}
		break;
	}

	// ‰E‚Ì“–‚½‚è”»’è
	if (m_state == PLAYERSTATEGOAL_MOVE) {
		for (vector<vec2>::iterator iter = _pPlayer->m_rightPoints.begin();
			iter != _pPlayer->m_rightPoints.end();
			iter++) {
			if (g_courseManager.intersect(*iter)) {
				vec2 right = (ivec2)*iter / PART_SIZE * PART_SIZE;
				_pPlayer->m_position.x = right.x - PLAYER_SIZE;
				_pPlayer->m_speed.x = 0;
				_pPlayer->m_falling = true;
				m_state = PLAYERSTATEGOAL_WAIT;
				g_game.m_timer.countDown();
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
