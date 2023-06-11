#include "Message.h"
#include "font.h"

/**
 * @brief �R���X�g���N�^
 */
MessageController::MessageController()
	: m_count(0)
	, m_messageSize(0)
	, m_messages(nullptr)
	, m_update(false)
{
}

/**
 * @brief �f�X�g���N�^
 */
MessageController::~MessageController()
{
}

/**
 * @brief ���b�Z�[�W�R���g���[�����X�V����
 * 
 * @param[in] �Ȃ�
 */
void MessageController::update()
{
	if (m_update) {
		m_count++;
	}
}

/**
 * @brief ���b�Z�[�W��`�悷��
 * 
 * @param[in] �Ȃ�
 */
void MessageController::draw()
{
	if (!m_update) return;

	fontBegin();
	for (int i = 0; i < m_messageSize; i++) {
		if (m_count < unsigned(m_messages[i].startCount)) {
			break;
		}

		fontPosition(m_messages[i].position.x, m_messages[i].position.y);
		fontDraw(m_messages[i].string.c_str());
	}
	fontEnd();
}

/**
 * @brief ���b�Z�[�W��ݒ肷��
 *
 * @param[in] _messages	���b�Z�[�W�\���̂̃|�C���^
 * @param[in] _size		���b�Z�[�W�̃T�C�Y
 */
void MessageController::setMessage(const MESSAGE* _messages, int _size)
{
	m_messages = _messages;
	m_messageSize = _size;
}

