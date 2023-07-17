#pragma once
#include "Game.h"
#include "Rect.h"
#include "HitBlockController.h"
#include "BridgeController.h"
#include "Kinopio.h"
#include "common/common.h"
#include "Course.h"

#include <gl/glut.h>
#include <glm/glm.hpp>
#include <Windows.h>

#include <vector>
#include <string>

// �R�[�X�̓ǂݍ��ݎ��̃G���[��񋓂���
enum COURSE_ERROR {
	COURSE_NO_ERROR = 0,		// �G���[�Ȃ�
	COURSE_INVALID_VALUE,		// �����ȃp�[�c���w�肳��Ă���
	COURSE_INVALID_SIZE,		// �R�[�X�̃T�C�Y��0�ȉ�
	COURSE_FAILED_OPEN_FILE,	// �R�[�X�t�@�C�����J���Ȃ�����
	COURSE_OUT_OF_MEMORY,		// �R�[�X�f�[�^��ǂݍ��ރ��������c���Ă��Ȃ�
};

/**
* �R�[�X�Ǘ��N���X
*/
class CourseManager {
private:
	float					m_scroll;				// �X�N���[���ʒu
	bool					m_isLoaded;				// �R�[�X�ǂݍ��݃t���O
	std::vector<QUAD>		m_quads;				// �R�[�X�p�[�c�̋�`
	std::vector<glm::ivec2>	m_coins;				// �R�C���̈ʒu
	HitBlockController		m_hitBlockController;	// �@�����Ƃ��̃u���b�N����
	Rect					m_clearAex;				// �N���A��
	BridgeController		m_bridgeController;		// ���̐���
	Kinopio					m_kinopio;				// �L�m�s�I
	COURSE_ERROR			m_courseError;			// �R�[�X�G���[
	std::string				m_errorMsg;				// �G���[���b�Z�[�W
	Course					m_currentCourse;

public:
	// �R���X�g���N�^
	CourseManager();

	// �f�X�g���N�^
	virtual ~CourseManager();

	// �R�[�X�z������
	void release();

	// �R�[�X���N���A����
	void clear();

	// �R�[�X��ǂݍ���
	bool load(const char* _fileName);

	// �R�[�X���X�V
	void update();

	// �R�[�X��`��
	void draw();

	// �X�N���[���ʒu��ݒ�
	void setScroll(const float _scroll) { m_scroll = _scroll; };

	// ���݂̃X�N���[�����擾
	float getScroll() const { return m_scroll; };

	// ���݂̃X�N���[���ʒu������Z����
	void addScroll(const float _scroll) { m_scroll += _scroll; };

	// �R�[�X�̕��܂ŃX�N���[��������
	bool isScrollMax();

	// �p�[�c��ݒ�
	void setParts(glm::ivec2 const& _point, int _parts);

	// �p�[�c���擾
	int getParts(int _x, int _y);

	// �p�[�c���擾
	int getParts(glm::vec2 const& _point);

	// �R�[�X�Ƃ̓����蔻��
	bool intersect(glm::vec2 const& _point);

	// �R�[�X�̃R�C���Ƃ̓����蔻��
	void intersectCoin(Player* _pPlayer);

	// �u���b�N��@��
	void hitBlock(glm::vec2 const& _point);

	// �R�[�X�̕����擾
	int getWidth() const
	{
		// �{�X�X�e�[�W�̏ꍇ�����k�߂�
		if ((g_game.m_world.stage == 4) && (!g_player.m_clear)) {
			return m_currentCourse.m_width - 17;
		}
		return m_currentCourse.m_width;
	};

	// �R�[�X�̍������擾
	int getHeight() const { return m_currentCourse.m_height; };

	// �w�i�F���擾����
	COLORREF getClearColor() const { return m_currentCourse.m_clearColor; };

	// �v���C���[�̃X�^�[�g�ʒu���擾
	glm::ivec2 getStartPosition() const { return m_currentCourse.m_startPosition; };

	// ���̃��[���h���擾
	WORLD getNextWorld() const { return m_currentCourse.m_nextWorld; };

	// �N���A���Ɠ������Ă��邩
	bool getClearAex(Rect& _rect);

	// �{�X�X�e�[�W�̋���j�󂷂�
	void destroyBridge()
	{
		m_bridgeController.destroy();
	};

	// �{�X�X�e�[�W�̋����󂳂ꂽ��
	bool isBridgeDestroyed()
	{
		return m_bridgeController.isDestroyed();
	};

	// �L�m�s�I���Ƃ̓����蔻��
	bool intersectKinopio(const Rect* _rect);

	// ��`����ʓ��ɂ��邩
	bool isInScreen(const Rect& _rect);

	// �G���[�񋓒萔���擾
	COURSE_ERROR getError() const
	{
		return m_courseError;
	};

	// �G���[���b�Z�[�W���擾
	std::string getErrorString() const
	{
		//cout << "m_errorMsg=" << m_errorMsg << endl;
		return m_errorMsg;
	};

	void import(Course* pCourse);
};

extern CourseManager g_courseManager;