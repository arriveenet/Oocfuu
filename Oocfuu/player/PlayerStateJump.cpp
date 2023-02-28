#include "PlayerStateJump.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "input/Keyboard.h"
#include "animation/Animation.h"
#include "sound/Sound.h"

#include <stdio.h>

/*
* ジャンプアルゴリズム参考
* https://qiita.com/morian-bisco/items/4c659d9f940c7e3a2099
*/

static const unsigned char VerticalForceDecimalPartData[] = { 0x20, 0x20, 0x1e, 0x28, 0x28 };
static const unsigned char VerticalFallForceData[] =		{ 0x70, 0x70, 0x60, 0x90, 0x90 };
static const char InitalVerticalSpeedData[] = { -4, -4, -4, -5, -5 };
static const unsigned char InitalVerticalForceData[] = { 0x00, 0x00, 0x00, 0x00, 0x00};

PlayerStateJump::PlayerStateJump()
	: VerticalForce(0)
	, VerticalForceFall(0)
	, VerticalForceDecimalPart(0)
	, VerticalPositionOrigin(0)
	, VerticalPosition(0)
	, CorrectionValue(0)
	, m_lastFalling(false)
	, PlayerState(PLAYER_STATE_JUMP)
{
}

void PlayerStateJump::start(Player* _pPlayer)
{
	// ジャンプ準備
	_pPlayer->m_jumping = true;
	_pPlayer->m_falling = false;

	VerticalForceDecimalPart = 0;
	VerticalPosition = VerticalPositionOrigin = (int)_pPlayer->m_position.y;

	int index = 0;
	if (HorizontalSpeed >= 0x1c) index++;
	if (HorizontalSpeed >= 0x19) index++;
	if (HorizontalSpeed >= 0x10) index++;
	if (HorizontalSpeed >= 0x09) index++;

	VerticalForce = VerticalForceDecimalPartData[index];
	VerticalForceFall = VerticalFallForceData[index];
	VerticalForceDecimalPart = InitalVerticalForceData[index];
	_pPlayer->m_speed.y = InitalVerticalSpeedData[index];

	int r = rand() % 2;
	g_pSound->play((SOUNDTYPE)r);
}

void PlayerStateJump::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	// 落下状態が終了した場合走り状態に遷移する
 	if (!_pPlayer->m_falling && m_lastFalling) {
		_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_RUN);
		_pStateContext->setStete(new PlayerStateRun);
		return;
	}

	if (Keyboard::m_pressed[PLAYER_KEY_RIGHT])
		_pPlayer->m_speed.x += 0.04f;
	if (_pPlayer->m_speed.x >= 2.0f)
		_pPlayer->m_speed.x = 2.0f;
	
	if (Keyboard::m_pressed[PLAYER_KEY_LEFT])
		_pPlayer->m_speed.x -= 0.04f;
	if (_pPlayer->m_speed.x <= -2.0f)
		_pPlayer->m_speed.x = -2.0f;

	if (_pPlayer->m_speed.y >= 0) {
		VerticalForce = VerticalForceFall;
		_pPlayer->m_falling = true;
	} else {
		if (Keyboard::m_released[PLAYER_KEY_JUMP]) {	// ジャンプボタンが離されたら
			//if(VerticalPositionOrigin - VerticalPosition >= 1)
			VerticalForce = VerticalForceFall;
			_pPlayer->m_falling = true;
		}
	}

	int cy = 0;
	CorrectionValue += VerticalForceDecimalPart;
	if (CorrectionValue >= 0x100) {
		CorrectionValue -= 0x100;
		cy = 1;
	}

	//VerticalPosition += VerticalSpeed + cy;
	/*_pPlayer->m_position.x += _pPlayer->m_speed.x;
	_pPlayer->m_position.y += _pPlayer->m_speed.y + cy;*/

	VerticalForceDecimalPart += VerticalForce;
	if (VerticalForceDecimalPart >= 0x100) {
		VerticalForceDecimalPart -= 0x100;
		_pPlayer->m_speed.y++;
	}

	if (_pPlayer->m_speed.y >= DOWN_SPEED_MAX) {
		if (VerticalForceDecimalPart >= 0x80) {
			_pPlayer->m_speed.y = DOWN_SPEED_MAX;
			VerticalForceDecimalPart = 0x00;
		}
	}

	m_lastFalling = _pPlayer->m_falling;
}
