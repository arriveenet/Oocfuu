#pragma once
#include <chrono>
#include <time.h>
#include <Windows.h>
#include "common/common.h"

class Game;

/**
 * @brief アプリケーションクラス
 */
class App {
private:
	HANDLE m_hConsoleOutput;	//!< コンソールハンドル
	bool m_running;				//!< メインループフラグ
	int m_window;				//!< ウィンドウハンドル
	Game* m_pGame;
	unsigned int m_frames;
	float m_deltaTime;
	float m_accumulator;
	std::chrono::steady_clock::time_point m_currentTime;

public:
	// コンストラクタ
	App();
	// デストラクタ
	virtual ~App();

	// アプリケーションを初期化する
	bool init();

	// アプリケーションのリソースを解放する
	void release();

	// アプリケーションの更新処理
	void update();

	// アプリケーションの描画処理
	void draw();

	// アプリケーションのメインループを開始
	void run();

	// アプリケーションを閉じる
	void close();

	// ウィンドウを設定する
	void setWindow(int _window) noexcept { m_window = _window; }

	// デルタタイムを取得する
	float getDeltaTime() const { return m_deltaTime; }
	
protected:
	// 初期化の結果をコンソールに表示する
	bool printInit(int _result, const char* _str) const;
	bool printInit(bool _result, const char* _str) const;
};

extern App g_app;
