#include "KoopaStates.h"
#include "App.h"
#include "player/Player.h"
#include "world/Part.h"
#include "world/CourseManager.h"
#include "animation/Animation.h"
#include "sound/Sound.h"

#include <glm/glm.hpp>

using namespace glm;

// �A�C�h����ԃN���X
KoopaStateIdle* KoopaStateIdle::instance()
{
	static KoopaStateIdle instance;
	return &instance;
}

void KoopaStateIdle::enter(Koopa* _pKoopa)
{
	_pKoopa->m_state = KOOPA_STATE_IDLE;
	_pKoopa->m_counter = 0;
	_pKoopa->nextMovement();
}

void KoopaStateIdle::execute(Koopa* _pKoopa)
{
	if (_pKoopa->m_counter >= 32) {
		int r =rand() % KOOPA_STATE_MAX;
		printf("r=%d\n", r);
		switch (r) {
		case KOOPA_STATE_JUMP:
			_pKoopa->m_pStateMachine->changeState(KoopaStateJump::instance());
			break;
		case KOOPA_STATE_FIRE:
			_pKoopa->m_pStateMachine->changeState(KoopaStateFire::instance());
			break;
		}
		_pKoopa->m_counter = 0;
	}
}

void KoopaStateIdle::exit(Koopa* _pKoopa)
{
	
}

// �W�����v���
KoopaStateJump* KoopaStateJump::instance()
{
	static KoopaStateJump instance;
	return &instance;
}

void KoopaStateJump::enter(Koopa* _pKoopa)
{
	_pKoopa->m_state = KOOPA_STATE_JUMP;
	m_lastFalling = false;
	// �����t���O�����낷
	_pKoopa->m_flag &= ~KOOPA_FLAG_FALLING;
	// �W�����v�t���O�𗧂Ă�
	_pKoopa->m_flag |= KOOPA_FLAG_JUMPING;
	_pKoopa->m_jumpCount = 0;
}

void KoopaStateJump::execute(Koopa* _pKoopa)
{
	if (!(_pKoopa->m_flag & KOOPA_FLAG_FALLING) && m_lastFalling) {
		_pKoopa->m_pStateMachine->changeState(KoopaStateIdle::instance());
	}

	if (_pKoopa->m_flag & KOOPA_FLAG_JUMPING) {
		_pKoopa->m_speed.y = -4.5f;
		if (++_pKoopa->m_jumpCount >= KOOPA_JUMP_COUNT_MAX) {
			_pKoopa->m_flag &= ~KOOPA_FLAG_JUMPING;
		}
	}

	m_lastFalling = _pKoopa->m_flag & KOOPA_FLAG_FALLING;
}

void KoopaStateJump::exit(Koopa* _pKoopa)
{
	_pKoopa->m_counter = 0;
}

// Fire state
KoopaStateFire* KoopaStateFire::instance()
{
	static KoopaStateFire instance;
	return &instance;
}

void KoopaStateFire::enter(Koopa* _pKoopa)
{
	_pKoopa->m_state = KOOPA_STATE_FIRE;
	// �A�j���[�V������ύX
	_pKoopa->m_animationController.setAnimation(ANIMATION_KOOPA_FIRE);
}

void KoopaStateFire::execute(Koopa* _pKoopa)
{
	if (_pKoopa->m_animationController.m_animation != ANIMATION_KOOPA_FIRE) {
		_pKoopa->m_pStateMachine->changeState(KoopaStateIdle::instance());
	}
}

void KoopaStateFire::exit(Koopa* _pKoopa)
{
	// �t�@�C���[�����
	_pKoopa->fire();

	// �A�j���[�V������߂�
	_pKoopa->m_animationController.setAnimation(ANIMATION_KOOPA);
}

// Die state
KoopaStateDie* KoopaStateDie::instance()
{
	static KoopaStateDie instance;
	return &instance;
}

void KoopaStateDie::enter(Koopa* _pKoopa)
{
	_pKoopa->m_state = KOOPA_STATE_DIE;

	m_lastBridge = false;
	// �A�j���[�V�����̑��x�𑁂�����
	g_animations[ANIMATION_KOOPA].m_rate = 6;
	_pKoopa->m_animationController.setAnimation(ANIMATION_KOOPA);
	_pKoopa->m_speed = { 0.0f, 0.0f };
	_pKoopa->m_flag &= ~KOOPA_FLAG_FALLING;
	_pKoopa->m_flag &= ~KOOPA_FLAG_COLLISION;
}

void KoopaStateDie::execute(Koopa* _pKoopa)
{
	// ������ꂽ�痎������
	bool bridge = g_courseManager.isBridgeDestroyed();
	if (!m_lastBridge && bridge) {
		//_pKoopa->m_collision = false;
		g_pSound->play(SOUND_SE_KOOPA_DIE);
		g_animations[ANIMATION_KOOPA].m_rate = 0;
		_pKoopa->m_flag &= ~KOOPA_FLAG_ANIMATION;
		_pKoopa->m_flag |= KOOPA_FLAG_FALLING;
	}

	if (_pKoopa->m_position.y > SCREEN_HEIGHT) {
		_pKoopa->m_flag |= KOOPA_FLAG_DEAD;
		_pKoopa->m_speed.y = 0.0f;
	}

	m_lastBridge = bridge;
}

void KoopaStateDie::exit(Koopa* _pKoopa)
{
	// �A�j���[�V�����̑��x�����Ƃɖ߂�
	g_animations[ANIMATION_KOOPA].m_rate = 32;
	_pKoopa->m_flag &= KOOPA_FLAG_ANIMATION;
	_pKoopa->m_flag &= KOOPA_FLAG_COLLISION;
}
