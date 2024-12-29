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
	{u8"�`�������ē`���Ȃ���", 288},
	{u8"���ɂ͑f���ɂȂꂸ��", 288},
	{u8"�������G�߂��z�����l���", 288},
	{u8"���ƂĂ��P���Ă����", 252},
	{u8"���ꂼ��`��", 144},
	{u8"�K���̂������͏d�Ȃ�", 288},
	{u8"���傫�Ȉ��ɂȂ�", 216},
	{u8"�����Ƃӂ���Ő����Ă䂱��", 396},
	{u8"", 108},
	{u8"�S�N������𐾂���", 288},
	{u8"�N�͖l�̑S�Ă�", 288},
	{u8"�M���Ă��邽���M���Ă�", 288},
	{u8"�������Ԃ����ސl��", 288},
	{u8"�ǂ�ȌN���ǂ�Ȗl�ł�", 288},
	{u8"�ЂƂЂƂ�������", 288},
	{u8"�N������Ή�������Ȃ�", 288},
	{u8"�����ƍK���ɂ��邩��", 288},
	{u8"", 900},
	{u8"�J�̒��ŌN��҂��Ă�", 288},
	{u8"�D�����̈Ӗ������m�炸", 288},
	{u8"����Ⴂ�ɏ�������", 288},
	{u8"����ł������܂ŗ�����", 252},
	{u8"���������̂Ȃ�", 144},
	{u8"�o�����͊�Ղ��q���ł�", 288},
	{u8"�v���o�d�Ȃ荇��", 216},
	{u8"�͂��܂�̖̉苿����", 396},
	{u8"", 108},
	{u8"�ǂ�ȂƂ����x���Ă��ꂽ", 288},
	{u8"�΂����������Ԃ�", 288},
	{u8"�S���߂Ă����ЂƂ���", 288},
	{u8"���錾�t�͂��肪�Ƃ�", 270},
	{u8"", 1314},
	{u8"�S�N������𐾂���", 288},
	{u8"�N�͖l�̑S�Ă�", 288},
	{u8"�����Ă��邽�������Ă�", 288},
	{u8"���������񑩂��悤", 288},
	{u8"���E���ɂ�����l����", 288},
	{u8"�l�͌N��I��", 288},
	{u8"�N�Ƃ���΂ǂ�Ȗ�����", 288},
	{u8"�����ƋP���Ă��邩��", 324},
	{u8"", 108},
	{u8"��������������������������", 288},
	{u8"��������������������", 288},
	{u8"��������������������������", 288},
	{u8"������������������������", 288},
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
	// �v���C���[�X�g�b�v�X�e�b�v
	case CLEAR_STEP_STOP:
		if (g_enemyManager.koopaIsDead()) {
			m_step = CLEAR_STEP_MOVE;
			//g_pSound->play(SOUND_TYPE_SE_WORLED_CLEAR);
			g_music.setMusic(MusicType::HappyBirthday);
			g_music.play();
		}
		break;
	// �v���C���[�ړ��X�e�b�v
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

		// �G���e�B�e�B�Ƃ̓����蔻��
		for (Sprite* object : g_courseManager.getCourse().getCourceObject()) {
			if (object->intersect(_pPlayer->getRect())) {
				_pPlayer->m_speed = { 0.0f, 0.0f };
				_pPlayer->m_animationController.setAnimation(ANIMATION_PLAYER_IDLE);
				m_step = CLEAR_STEP_STAY;
			}
		}

		break;
	// �v���C���[��~�X�e�b�v
	case CLEAR_STEP_STAY:
		g_courseManager.addScroll(1.0f);
		if (g_courseManager.isScrollMax() && (!m_lastScrollMax)) {
			_pPlayer->m_messageController.start();
		}

		// �O�̃~���[�W�b�N���I������ꍇ
		if ((g_music.getState() == MusicState::Stopped) && (m_lastMusicState == MusicState::Playing)) {
			// �^�C�t�[���p���[�h���Đ�����
			g_music.reset();
			g_music.setMusic(MusicType::OneLove);
			g_music.play();

			// ���b�Z�[�W��ݒ�
			_pPlayer->m_messageController.reset();
			_pPlayer->m_messageController.setMessage(message2, message2Size);
			_pPlayer->m_messageController.start();

			// �̎���ݒ�
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
