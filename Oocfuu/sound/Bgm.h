#pragma once
#include "sound/Sound.h"

/**
 * @brief BGM�N���X
 * 
 */
class Bgm {
public:
	// BGM�̎��
	enum BgmType {
		Unknown,
		Ground,
		Underground,
		Castle,
	};

	// �R���X�g���N�^
	Bgm();
	// �f�X�g���N�^
	virtual ~Bgm();

	// BGM���Đ�
	static void play();

	// BGM���~
	static void stop();

	// BGM��ݒ�
	static void setBgm(BgmType _bgm);

private:
	static BgmType m_bgmType;		//!< �Đ�����BGM�̎��
	static SOUNDTYPE m_soundType;	//!< �T�E���h�̎��
};