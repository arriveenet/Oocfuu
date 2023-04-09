#pragma once
#include <string>
#include <glm/glm.hpp>

typedef struct tagMESSAGE{
	std::string string;
	glm::vec2 position;
	int startCount;
} MESSAGE;

// ���b�Z�[�W����N���X
class MessageController {
public:
	// �R���X�g���N�^
	MessageController();

	// �f�X�g���N�^
	virtual ~MessageController();

	// �X�V
	void update();

	// �`��
	void draw();

	// ���b�Z�[�W���X�^�[�g
	void start() { m_update = true; m_count = 0; };

	// ���b�Z�[�W�����Z�b�g
	void reset() { m_update = false; m_count = 0; };

	// ���b�Z�[�W��ݒ肷��
	void setMessage(MESSAGE* _messages, int _size);

private:
	unsigned int m_count;	// �J�E���^�[
	int m_messageSize;		// ���b�Z�[�W�T�C�Y
	MESSAGE* m_messages;	// ���b�Z�[�W���X�g
	bool m_update;			// �X�V�t���O
};

