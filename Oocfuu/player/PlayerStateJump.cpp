#include "PlayerStateJump.h"
#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "../Keyboard.h"
#include "../animation/Animation.h"
#include "../sound/Sound.h"
#include <stdio.h>

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
	, lastJumping(false)
	, PlayerState(PLAYER_STATE_JUMP)
{
}

void PlayerStateJump::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	if (!_pPlayer->m_jumping && lastJumping) {
		//printf("着地\n");
		_pPlayer->m_animeCtr.setAnimation(ANIMATION_PLAYER_RUN);
		_pStateContext->setStete(new PlayerStateRun);
		return;
	}

	// ジャンプ準備
	if (!_pPlayer->m_jumping) {
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

		g_pSound->play(SOUND_SE_HOI);
	}

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
	_pPlayer->m_position.x += _pPlayer->m_speed.x;
	_pPlayer->m_position.y += _pPlayer->m_speed.y + cy;

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

	lastJumping = _pPlayer->m_jumping;
}
