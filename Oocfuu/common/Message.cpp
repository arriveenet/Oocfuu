#include "Message.h"
#include "font.h"

MessageController::MessageController()
	: m_count(0)
	, m_messageSize(0)
	, m_messages(nullptr)
	, m_update(false)
{
}

MessageController::~MessageController()
{
}

void MessageController::update()
{
	if (m_update) {
		m_count++;
	}
}

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

void MessageController::setMessage(const MESSAGE* _messages, int _size)
{
	m_messages = _messages;
	m_messageSize = _size;
}

