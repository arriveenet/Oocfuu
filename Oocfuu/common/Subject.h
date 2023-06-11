#pragma once
#include "Observer.h"
#include <list>

/**
 * �T�u�W�F�N�g�N���X
 */
class Subject {
private:
	std::list<Observer*> m_observerList;	//!< �I�u�U�[�o�[���X�g

public:
	/**
	 * @brief �I�u�U�[�o�[��ǉ�����
	 * 
	 * @param[in] _pObserver	�ǉ�����I�u�U�[�o�[�̃|�C���^
	 *
	 */
	void addObserver(Observer* _pObserver)
	{
		m_observerList.push_back(_pObserver);
	}

	/**
	 * @brief �I�u�U�[�o�[���폜����
	 *
	 * @param[in] _pObserver	�폜����I�u�U�[�o�[�̃|�C���^
	 *
	 */
	void removeObserver(Observer* _pObserver)
	{
		m_observerList.remove(_pObserver);
	}

protected:
	/**
	 * @brief �I�u�U�[�o�[�ɒʒm����
	 *
	 * @param[in] �Ȃ�
	 *
	 */
	void notify()
	{
		std::list<Observer*>::iterator iter = m_observerList.begin();
		for (; iter != m_observerList.end(); ++iter) {
			(*iter)->onNotify();
		}
	}

};