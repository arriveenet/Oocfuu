#pragma once
#include "Entity.h"

/**
 * �I�u�U�[�o�[���ۃN���X
 */
class Observer {
public:
	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~Observer() {};

	/**
	 * @brief �ʒm���󂯎��
	 * 
	 * @param[in]	�Ȃ�
	 * 
	 */
	virtual void onNotify() = 0;
};