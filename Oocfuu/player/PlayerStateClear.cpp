#include "PlayerStateClear.h"
#include "App.h"
#include "animation/Animation.h"
#include "world/CourseManager.h"
#include "world/Part.h"
#include "enemy/EnemyManager.h"
#include "sound/Sound.h"
#include "sound/Bgm.h"
#include <vector>

using namespace std;
using namespace glm;

namespace {
	const MESSAGE messages[] =
	{
		{"THANK YOU OOCFUU!", vec2(64, 80), 0},
		{"BUT OUR PRINCESS IS IN\n\nANOTHER CASTLE!", vec2(40, 112), 96}
	};

	const MESSAGE message2[] =
	{
		{"HAPPY BIRTHDAY OOCFUU!",		vec2(40, 64),	96},
		{"FEBRUARY,4,2024",				vec2(120, 88),	192},
		{"THANK YOU FOR A FUN",			vec2(56,104),	288},
		{"LIVESTREAMING ALWAYS.",		vec2(48, 120),	288},
		{"HOPE YOU HAVE A GREAT YEAR!",	vec2(24, 136),	384},
		{"FROM OSHU-FUJIWARA",			vec2(96, 160),	480}
	};

	constexpr int messageSize = sizeof(messages) / sizeof(MESSAGE);
	constexpr int message2Size = sizeof(message2) / sizeof(MESSAGE);
}

PlayerStateClear::PlayerStateClear()
	: PlayerState(PLAYER_STATE_CLEAR)
	, m_step(CLEAR_STEP_STOP)
	, m_lastScrollMax(false)
	, m_lastMusicState(g_music.getState())
{
}

void PlayerStateClear::start(Player* _pPlayer)
{
	Bgm::stop();

	_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_RUN);
	_pPlayer->m_speed = { 0.0f, 0.0f };
	_pPlayer->m_messageController.setMessage(messages, messageSize);

	g_courseManager.destroyBridge();
}

void PlayerStateClear::update(PlayerStateContext* _pStateContext, Player* _pPlayer)
{
	switch (m_step) {
	// プレイヤーストップステップ
	case CLEAR_STEP_STOP:
		if (g_enemyManager.koopaIsDead()) {
			m_step = CLEAR_STEP_MOVE;
			//g_pSound->play(SOUND_TYPE_SE_WORLED_CLEAR);
			g_music.setMusic(MusicType::HappyBirthday);
			g_music.play();
		}
		break;
	// プレイヤー移動ステップ
	case CLEAR_STEP_MOVE:
		_pPlayer->m_acceleration = 0.14f;
		_pPlayer->m_speed.x += _pPlayer->m_acceleration;
		_pPlayer->m_speed.y += PLAYER_GRAVITY;
		{
			static int one = 0x100;
			ivec2 fixed = _pPlayer->m_speed * (float)one;
			fixed = fixed * 0xe8 / one;
			_pPlayer->m_speed = (vec2)fixed / (float)one;
		}

		// エンティティとの当たり判定
		for (Sprite* object : g_courseManager.getCourse().m_courseObjects) {
			if (object->intersect(*_pPlayer)) {
				_pPlayer->m_speed = { 0.0f, 0.0f };
				_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
				m_step = CLEAR_STEP_STAY;
			}
		}

		break;
	// プレイヤー停止ステップ
	case CLEAR_STEP_STAY:
		g_courseManager.addScroll(1.0f);
		if (g_courseManager.isScrollMax() && (!m_lastScrollMax)) {
			_pPlayer->m_messageController.start();
		}

		// 前のミュージックが終わった場合
		if ((g_music.getState() == MusicState::Stopped) && (m_lastMusicState == MusicState::Playing)) {
			// タイフーンパレードを再生する
			g_music.reset();
			g_music.setMusic(MusicType::TyphoonParade);
			g_music.play();

			// メッセージを設定
			_pPlayer->m_messageController.reset();
			_pPlayer->m_messageController.setMessage(message2, message2Size);
			_pPlayer->m_messageController.start();
		}

		m_lastScrollMax = g_courseManager.isScrollMax();
		m_lastMusicState = g_music.getState();
		break;
	default:
		break;
	}
}
