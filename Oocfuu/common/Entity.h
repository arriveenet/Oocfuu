#pragma once

/**
 * @brief �G���e�B�e�B���ۃN���X
 * 
 */
struct Entity {
	/**
	 * @brief �X�V����
	 */
	virtual void update() = 0;

	/**
	 * @brief �`�揈��
	 */
	virtual void draw() = 0;
};