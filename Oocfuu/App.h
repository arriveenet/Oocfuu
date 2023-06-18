#pragma once
#include <time.h>
#include <Windows.h>
#include "common/common.h"

/**
 * @brief アプリケーションクラス
 */
class App {
private:
	HANDLE m_hConsoleOutput;	//!< コンソールハンドル
	bool m_running;				//!< メインループフラグ
	int m_window;				//!< ウィンドウハンドル

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
	
protected:
	// 初期化の結果をコンソールに表示する
	bool printInit(int _result, const char* _str);
	bool printInit(bool _result, const char* _str);
};

extern App g_app;
