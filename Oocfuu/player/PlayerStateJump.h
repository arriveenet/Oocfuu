#pragma once
#include "PlayerState.h"

#define DOWN_SPEED_MAX	0x04

class PlayerStateJump : public PlayerState {
	int VerticalPositionOrigin;		// �W�����v�J�n���̈ʒu
	int VerticalPosition;			// ���݈ʒu
	//int VerticalSpeed;			// ���x
	int VerticalForce;				// ���݂̉����x
	int VerticalForceFall;			// �~�����̉����x
	int VerticalForceDecimalPart;	// �����x�̑����l
	int CorrectionValue;			// �ݐόv�Z�ł̕␳�l�H
	int HorizontalSpeed = 00;		// ���������x
	bool m_lastFalling;				// 1�t���[���O�̗�����ԃt���O

public:
	PlayerStateJump();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer) override;
};