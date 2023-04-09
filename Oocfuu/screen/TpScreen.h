#pragma once
#include "Screen.h"
#include "common/Message.h"

class TpScreen : public Screen {
public:
	void init() override;
	void reset() override;
	void update() override;
	void draw() override;

private:
	MessageController m_messageController;
};

