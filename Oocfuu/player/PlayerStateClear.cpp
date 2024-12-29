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
		{"HAPPY BIRTHDAY OOCFUU!",		vec2(40, 64),	0},
		{"FEBRUARY,4,2024",				vec2(120, 88),	144},
		{"THANK YOU FOR A FUN",			vec2(56,104),	288},
		{"LIVESTREAMING ALWAYS.",		vec2(48, 120),	288},
		{"HOPE YOU HAVE A GREAT YEAR!",	vec2(24, 136),	432},
		{"FROM OSHU-FUJIWARA",			vec2(96, 160),	432}
	};

	const MESSAGE message3[] =
	{
		{"HAPPY BIRTHDAY", vec2(72, 123), 0},
	};

	constexpr int messageSize = sizeof(messages) / sizeof(MESSAGE);
	constexpr int message2Size = sizeof(message2) / sizeof(MESSAGE);
	constexpr int message3Size = sizeof(message3) / sizeof(MESSAGE);

	static std::vector<Lyrics> oneLoveLyrics = {
	{u8"", 1998},
	{u8"伝えたくて伝わらなくて", 288},
	{u8"時には素直になれずに", 288},
	{u8"泣いた季節を越えた僕らは", 288},
	{u8"今とても輝いているよ", 252},
	{u8"それぞれ描く", 144},
	{u8"幸せのかたちは重なり", 288},
	{u8"今大きな愛になる", 216},
	{u8"ずっとふたりで生きてゆこう", 396},
	{u8"", 108},
	{u8"百年先も愛を誓うよ", 288},
	{u8"君は僕の全てさ", 288},
	{u8"信じているただ信じてる", 288},
	{u8"同じ時間を刻む人へ", 288},
	{u8"どんな君もどんな僕でも", 288},
	{u8"ひとつひとつが愛しい", 288},
	{u8"君がいれば何もいらない", 288},
	{u8"きっと幸せにするから", 288},
	{u8"", 900},
	{u8"雨の中で君を待ってた", 288},
	{u8"優しさの意味さえ知らず", 288},
	{u8"すれ違いに傷ついた夜", 288},
	{u8"それでもここまで来たんだ", 252},
	{u8"かけがえのない", 144},
	{u8"出逢いは奇跡を繋いでく", 288},
	{u8"思い出重なり合う", 216},
	{u8"はじまりの歌鳴り響いて", 396},
	{u8"", 108},
	{u8"どんなときも支えてくれた", 288},
	{u8"笑い泣いた仲間へ", 288},
	{u8"心込めてただひとつだけ", 288},
	{u8"贈る言葉はありがとう", 270},
	{u8"", 1314},
	{u8"百年先も愛を誓うよ", 288},
	{u8"君は僕の全てさ", 288},
	{u8"愛しているたた愛してる", 288},
	{u8"同じ明日約束しよう", 288},
	{u8"世界中にただ一人だけ", 288},
	{u8"僕は君を選んだ", 288},
	{u8"君といればどんな未来も", 288},
	{u8"ずっと輝いているから", 324},
	{u8"", 108},
	{u8"ラララララララララララララ", 288},
	{u8"ララララララララララ", 288},
	{u8"ラララララララララララララ", 288},
	{u8"ララララララララララララ", 288},
	};
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
	_pPlayer->m_messageController.setMessage(message3, message3Size);

	_pPlayer->m_lyricsController.bindLyrics(oneLoveLyrics);

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
		for (Sprite* object : g_courseManager.getCourse().getCourceObject()) {
			if (object->intersect(_pPlayer->getRect())) {
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
			g_music.setMusic(MusicType::OneLove);
			g_music.play();

			// メッセージを設定
			_pPlayer->m_messageController.reset();
			_pPlayer->m_messageController.setMessage(message2, message2Size);
			_pPlayer->m_messageController.start();

			// 歌詞を設定
			_pPlayer->m_lyricsController.reset();
			_pPlayer->m_lyricsController.start();
		}

		m_lastScrollMax = g_courseManager.isScrollMax();
		m_lastMusicState = g_music.getState();
		break;
	default:
		break;
	}
}
