#pragma once
#include "PlayerState.h"

#define DOWN_SPEED_MAX	0x04

class PlayerStateJump : public PlayerState {
	int VerticalPositionOrigin;		// ジャンプ開始時の位置
	int VerticalPosition;			// 現在位置
	//int VerticalSpeed;			// 速度
	int VerticalForce;				// 現在の加速度
	int VerticalForceFall;			// 降下時の加速度
	int VerticalForceDecimalPart;	// 加速度の増加値
	int CorrectionValue;			// 累積計算での補正値？
	int HorizontalSpeed = 00;		// 横方向速度
	bool m_lastFalling;				// 1フレーム前の落下状態フラグ

public:
	PlayerStateJump();

	void start(Player* _pPlayer) override;
	void update(PlayerStateContext* _pStateContext, Player* _pPlayer) override;
};