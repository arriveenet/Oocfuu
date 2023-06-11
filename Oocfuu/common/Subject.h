#pragma once
#include "Observer.h"
#include <list>

/**
 * サブジェクトクラス
 */
class Subject {
private:
	std::list<Observer*> m_observerList;	//!< オブザーバーリスト

public:
	/**
	 * @brief オブザーバーを追加する
	 * 
	 * @param[in] _pObserver	追加するオブザーバーのポインタ
	 *
	 */
	void addObserver(Observer* _pObserver)
	{
		m_observerList.push_back(_pObserver);
	}

	/**
	 * @brief オブザーバーを削除する
	 *
	 * @param[in] _pObserver	削除するオブザーバーのポインタ
	 *
	 */
	void removeObserver(Observer* _pObserver)
	{
		m_observerList.remove(_pObserver);
	}

protected:
	/**
	 * @brief オブザーバーに通知する
	 *
	 * @param[in] なし
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